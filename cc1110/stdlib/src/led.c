#include "led.h"

void led_init(void)
{
	P0DIR = 0x38 ;
}


#if LED_LOGIC_POS
void led_TurnOn(led_num num)
#else
void led_TurnOff(led_num num)
#endif
{
	switch(num){
		case LED_FEEDBACK:
			P0_4 = 1 ;
			break ;
		case LED_ATHOME:
			P0_5 = 1 ;
			break ;
		case LED_NOTATHOME:
			P0_3 = 1 ;
			break ;
		case LED_ALL:
			P0 |= 0x38 ;
		default:
			break ;
	}
}

#if LED_LOGIC_POS
void led_TurnOff(led_num num)
#else
void led_TurnOn(led_num num)
#endif
{
	switch(num){
		case LED_FEEDBACK:
			P0_4 = 0 ;
			break ;
		case LED_ATHOME:
			P0_5 = 0 ;
			break ;
		case LED_NOTATHOME:
			P0_3 = 0 ;
			break ;
		case LED_ALL:
			P0 &= ~0x38 ;
		default:
			break ;
	}
}
