#ifndef _TIMER_H_
#define _TIMER_H_
#include "stm32f10x.h"


typedef enum {OpenLED =1 ,CloseLED = 2}CtrlLed;

extern void RCC_Config(void);
extern void TIM2_Config(u16 time);
extern void ledctrl(CtrlLed ctrl);
extern void led_Fresh(void);

#endif




