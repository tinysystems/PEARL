#include "pearllib.h"

volatile uint8_t numCores = 1;
uint8_t dmt[] = {1, 2};

void pearl(fn_ptr func) {
    uint16_t power = sampleADC();
    numCores = DMT(power);
    activate(numCores, func);
}

void waitCores(void) {
    if (numCores > 1) {
        __WFI(); // Wait For Interrupt
    }
}

void signalMainCore(void) {
    //NVIC_SetPendingIRQ(MXC_IRQ_EXTI); // Example IRQ for signaling, replace as needed
}

void deepSleep(void) {
    __WFI(); // Cortex-M Wait For Interrupt
}

uint16_t sampleADC(void) {
    uint16_t adc_val = 0;
//    mxc_adc_req_t req;
//    req.channel = 0;
//    req.clock = MXC_ADC_CLK_SCALE_2;
//    req.avg = MXC_ADC_AVG_1;
//    req.cal = 0;
//
//    MXC_ADC_Init(NULL);
//    MXC_ADC_StartConversion(&req);
//    while (MXC_ADC_GetData(&adc_val) == E_BUSY);
    return adc_val;
}

uint8_t DMT(uint16_t power) {
    return (power < 6000) ? dmt[0] : dmt[1];
}

void activate(uint8_t n_cores, fn_ptr func) {
    if (n_cores > 1) {
        //NVIC_SetPendingIRQ(MXC_IRQ_EXTI); // Trigger secondary core
    }
}
