#ifndef __LED_H__
#define __LED_H__
#include "ioCC1110.h"

#define LED_LOGIC_POS 0

typedef int led_num ;
#define LED_ALL -1
#define LED_FEEDBACK 0 
#define LED_ATHOME 1 
#define LED_NOTATHOME 2



void led_init(void);
void led_TurnOn(led_num led_num);
void led_TurnOff(led_num led_num);
#endif


