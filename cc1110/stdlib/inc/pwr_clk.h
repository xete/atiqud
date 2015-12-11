#ifndef __PW_CLK__
#define __PW_CLK__

#include "ioCC1110.h"
#include "sys.h"


#define POWER_MODE_0	0x00
#define POWER_MODE_1	0x01
#define POWER_MODE_2	0x02
#define POWER_MODE_3	0x03

#define MHZ_26          0x00
#define MHZ_13          0x01
#define MHZ_6_5         0x02
#define MHZ_3_25        0x03
#define MHZ_1_62        0x04
#define MHZ_0_81        0x05
#define MHZ_0_40        0x06
#define MHZ_0_20        0x07


#define HIGH_FREQUENCY_RC_OSC_STABLE    (SLEEP & 0x20)
#define XOSC_STABLE						(SLEEP & 0x40)
#define CRYSTAL			0x00
#define RC				0x01

void set_main_clock_source(int source);
void set_power_mode(int mode);
void set_main_clock_speed(int frequency);
void enable_sleep_timer(void);
void disable_sleep_timer(void);

#endif
