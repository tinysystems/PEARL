#ifndef PEARL_H_
#define PEARL_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mxc_device.h"
#include "mxc_errors.h"
#include "nvic_table.h"
#include "board.h"
#include "gpio.h"
#include "lp.h"
#include "rtc.h"
#include "icc.h"
#include "simo.h"
#include "uart.h"
#include "mxc_delay.h"
#include "flc.h"
#include "flc_regs.h"
#include "gcr_regs.h"
#include "sdhc.h"
#include "tmr.h"
#include "rtc.h"
#include "config.h"

#define MAINCORE // Comment out this line when compiling for the secondary core

#ifdef MAINCORE
#define mainCore 1
#define coreid 0
#else
#define mainCore 0
#define coreid 1
#endif

#define EXEC_CMD 0xE0
#define FIN_CMD  0xF0
#define FLASH1_BACKUP_ADDR 	MXC_FLASH1_MEM_BASE+4

#define DUAL_CORE_MODE 1
#define SINGLE_CORE_MODE 0

#define LOW_LEVEL_POWER 200
#define HIGH_LEVEL_POWER 800
#define POWER_UP_STEP 2
#define POWER_DOWN_STEP 600
#define POWER_MAX_LEVEL 1000
#define POWER_MIN_LEVEL 0

extern uint16_t POWER;
extern uint8_t mode;
extern mxc_gpio_cfg_t gpio_out_single;
extern mxc_gpio_cfg_t gpio_out_dual;
struct backup_data{
	uint32_t count;
	uint8_t i;
	uint8_t j;
	uint8_t n;
	uint8_t m;
};
extern struct backup_data backupData;

extern volatile int after_restore;
extern volatile uint32_t pow_fails;
extern volatile int sleep_mode;
extern volatile int backup_mode;
extern volatile int adaptive_mode;

#define _begin_parallel_pearl(func) start_pearl(func);
#define _end_parallel_pearl stop_pearl();

typedef void (*fn_ptr)(void);

/* Function declarations */
void start_pearl(fn_ptr func);
void stop_pearl(void);
void switchToSingleCoreMode(mxc_gpio_cfg_t gpio_single, mxc_gpio_cfg_t gpio_dual);
void switchToDualCoreMode(mxc_gpio_cfg_t gpio_single, mxc_gpio_cfg_t gpio_dual);
void checkpoint(void);
void restore(void);
void switchToHIRCD4(void);
void switchToHIRC96(void);
void prepForDeepSleep(void);
void recoverFromDeepSleep(void);
void enter_deepsleep(void);
void enter_backup(void);

#endif /* PEARL_H_ */
