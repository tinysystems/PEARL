
/***** Includes *****/

#include "includes/pearllib/pearllib.h"

uint8_t mode = SINGLE_CORE_MODE;
uint16_t POWER = POWER_MIN_LEVEL;
mxc_gpio_cfg_t gpio_out_single = {};
mxc_gpio_cfg_t gpio_out_dual = {};

void switchToSingleCoreMode(mxc_gpio_cfg_t gpio_single, mxc_gpio_cfg_t gpio_dual){
	MXC_GPIO_OutClr(gpio_dual.port, gpio_dual.mask);
	MXC_GPIO_OutSet(gpio_single.port, gpio_single.mask);
	mode = SINGLE_CORE_MODE;
	printf("Switched to SINGLE CORE MODE\n");
}

void switchToDualCoreMode(mxc_gpio_cfg_t gpio_single, mxc_gpio_cfg_t gpio_dual){
	MXC_GPIO_OutClr(gpio_single.port, gpio_single.mask);
	MXC_GPIO_OutSet(gpio_dual.port, gpio_dual.mask);
	mode = DUAL_CORE_MODE;
	printf("Switched to DUAL CORE MODE\n");
}

void start_pearl(fn_ptr func){
	POWER = POWER + POWER_UP_STEP >= POWER_MAX_LEVEL ? POWER_MAX_LEVEL : POWER + POWER_UP_STEP;
	if(mode == SINGLE_CORE_MODE && POWER >= HIGH_LEVEL_POWER) {
		switchToDualCoreMode(gpio_out_single, gpio_out_dual);
		Start_Core1();
	} else if (mode == DUAL_CORE_MODE && POWER <= LOW_LEVEL_POWER) {
		Stop_Core1();
		switchToSingleCoreMode(gpio_out_single, gpio_out_dual);
	}
}

void stop_pearl(){
	after_restore = 0;
	if (sleep_mode){
		if (mode == DUAL_CORE_MODE) Stop_Core1();
		enter_deepsleep();
		if (mode == DUAL_CORE_MODE) Start_Core1();
	} else if(backup_mode){
		if (mode == DUAL_CORE_MODE) Stop_Core1();
		enter_backup();
		if (mode == DUAL_CORE_MODE) Start_Core1();
	}
}

struct backup_data backupData = {};
volatile int after_restore = 0;
volatile uint32_t pow_fails = 0;
volatile int sleep_mode = 0;
volatile int backup_mode = 0;
volatile int adaptive_mode = 0;

void checkpoint(void){
	uint8_t it = 10;
	while(it){
		MXC_FLC_PageErase(FLASH1_BACKUP_ADDR);
		MXC_FLC_Write(FLASH1_BACKUP_ADDR, sizeof(backupData), (uint32_t*)&backupData);
		it--;
	}
	printf("CNT before power failure = %d\n", backupData.count);
	printf("Checkpoint\n");
}

void restore(void){
	uint8_t it = 10;
	while(it){
		MXC_FLC_Read(FLASH1_BACKUP_ADDR, &backupData, sizeof(backupData));
		it--;
	}
	//after_restore = 1;
	printf("Restore\n");
	printf("CNT after restore = %d\n", backupData.count);
}

void switchToHIRCD4(void){
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_PSC, MXC_S_GCR_CLKCN_PSC_DIV4);
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC_EN;
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_CLKSEL, MXC_S_GCR_CLKCN_CLKSEL_HIRC);
    /* Disable unused clocks */
    while (!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CKRDY)) {}
    /* Wait for the switch to occur */
    MXC_GCR->clkcn &= ~(MXC_F_GCR_CLKCN_HIRC96M_EN);
    SystemCoreClockUpdate();
}

void switchToHIRC96(void){
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_PSC, MXC_S_GCR_CLKCN_PSC_DIV1);
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC96M_EN;
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_CLKSEL, MXC_S_GCR_CLKCN_CLKSEL_HIRC96);
    /* Disable unused clocks */
    while (!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CKRDY)) {}
    /* Wait for the switch to occur */
    MXC_GCR->clkcn &= ~(MXC_F_GCR_CLKCN_HIRC_EN);
    SystemCoreClockUpdate();
}

void prepForDeepSleep(void){
    MXC_ICC_Disable();
    MXC_LP_ICache0Shutdown();

    /* Shutdown unused power domains */
    MXC_PWRSEQ->lpcn |= MXC_F_PWRSEQ_LPCN_BGOFF;

    /* Prevent SIMO soft start on wakeup */
    MXC_LP_FastWakeupDisable();

    /* Enable VDDCSWEN=1 prior to enter backup/deepsleep mode */
    MXC_MCR->ctrl |= MXC_F_MCR_CTRL_VDDCSWEN;

    switchToHIRCD4();

    MXC_SIMO_SetVregO_B(810); /* Reduce VCOREB to 0.81v */
}

void recoverFromDeepSleep(void){
    /* Check to see if VCOREA is ready on  */
    if (!(MXC_SIMO->buck_out_ready & MXC_F_SIMO_BUCK_OUT_READY_BUCKOUTRDYC)) {
        /* Wait for VCOREB to be ready */
        while (!(MXC_SIMO->buck_out_ready & MXC_F_SIMO_BUCK_OUT_READY_BUCKOUTRDYB)) {}

        /* Move VCORE switch back to VCOREB */
        MXC_MCR->ctrl = (MXC_MCR->ctrl & ~(MXC_F_MCR_CTRL_VDDCSW)) |
                        (0x1 << MXC_F_MCR_CTRL_VDDCSW_POS);

        /* Raise the VCORE_B voltage */
        while (!(MXC_SIMO->buck_out_ready & MXC_F_SIMO_BUCK_OUT_READY_BUCKOUTRDYB)) {}
        MXC_SIMO_SetVregO_B(1000);
        while (!(MXC_SIMO->buck_out_ready & MXC_F_SIMO_BUCK_OUT_READY_BUCKOUTRDYB)) {}
    }

    MXC_LP_ICache0PowerUp();
    MXC_ICC_Enable();

    switchToHIRC96();
}

void enter_deepsleep(void){
//	if(adaptive_mode)
//		POWER = POWER < POWER_DOWN_STEP ? POWER_MIN_LEVEL : POWER - POWER_DOWN_STEP;
	printf("Entering DEEPSLEEP mode. Count - %d\n", backupData.count);
	//setTrigger(0);
	while (MXC_UART_ReadyForSleep(MXC_UART_GET_UART(CONSOLE_UART)) != E_NO_ERROR) {}
	prepForDeepSleep();
	MXC_LP_EnterDeepSleepMode();
	recoverFromDeepSleep();
	printf("Waking up from DEEPSLEEP mode. Count - %d\n", backupData.count);
	sleep_mode = 0;
}

void enter_backup(void){
	if(adaptive_mode)
		POWER = POWER < POWER_DOWN_STEP ? POWER_MIN_LEVEL : POWER - POWER_DOWN_STEP;
	backup_mode = 0;
	pow_fails++;
	checkpoint();
	printf("Entering BACKUP mode.\n");
	while (MXC_UART_ReadyForSleep(MXC_UART_GET_UART(CONSOLE_UART)) != E_NO_ERROR) {}
    prepForDeepSleep();
    //MXC_LP_EnterDeepSleepMode();
    MXC_LP_EnterBackupMode(0);
	recoverFromDeepSleep();
	printf("Waking up from BACKUP mode.\n");
	restore();
}

