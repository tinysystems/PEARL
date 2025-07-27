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
//#include "pb.h"
#include "uart.h"
//#include "led.h"
#include "mxc_delay.h"
#include "flc.h"
#include "flc_regs.h"
#include "gcr_regs.h"
#include "sdhc.h"
#include "tmr.h"
#include "rtc.h"
//#include "core1.h"
#include "mxc_device.h"
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

#define _begin_parallel_pearl(func) if(mainCore) pearl(func);
#define _end_parallel_pearl if(mainCore) waitCores(); else { signalMainCore(); deepSleep(); }

typedef void (*fn_ptr)(void);

extern volatile uint8_t numCores;
extern uint8_t dmt[];

/* Function declarations */
void pearl(fn_ptr func);
void waitCores(void);
void signalMainCore(void);
void deepSleep(void);
uint16_t sampleADC(void);
uint8_t DMT(uint16_t power);
void activate(uint8_t n_cores, fn_ptr func);

#endif /* PEARL_H_ */
