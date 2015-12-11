#include "pwr_clk.h"


void set_main_clock_source(int source)
{
	if( source ){
		CLKCON |= 0x40;
		while( !HIGH_FREQUENCY_RC_OSC_STABLE );
		SLEEP |= 0x04;
	}else{
		SLEEP &= ~0x04;
		while( !XOSC_STABLE );
		CLKCON &= ~0x47;
		SLEEP |= 0x04;
	}
}

void set_power_mode(int mode)
{
	if( mode == 0 ) SLEEP &= ~0x03;
	else if( mode == 3 ) SLEEP |= 0x03;
	else{
		SLEEP &= ~0x03;
		SLEEP |= mode;
	}
	PCON |= 0x01;
}

void set_main_clock_speed(int frequency)
{
	CLKCON = ((CLKCON & ~0x07) | (frequency & 0x07));
}

void enable_sleep_timer(void)
{
	WORCTL |= 0x00;
	IEN0 |= 0x20;
	/* unmask */
	WORIRQ |= 0x10;
}

void disable_sleep_timer(void)
{
	IEN0 &= ~0x20;	
	/* mask */
	WORIRQ &= ~0x10;
}
