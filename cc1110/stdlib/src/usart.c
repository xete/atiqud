#include "usart.h"
#include "sys.h"

void init_usart0(void)
{
	/* usart0 use alter1 location */
	PERCFG &= 0xfe;
	/* select pins on P0 for usart */
	P0SEL |= 0x3c;
	/* mode selection */
	U0CSR |= 0x80;
	/* 115200: baud_m 34 baud_e 12 err 0.14 LSB */
	U0GCR |= (U0GCR&0xc0)|0x0c;
	U0BAUD = 0x22;
	UTX0IF = 0;

	U0CSR |= 0x40;
	IEN0 |= 0x84;
}

void usart0_safe(char* data, int len)
{
	while( !XOSC_STABLE );
	usart0_send(data, len);
}

void usart0_send(char* data, int len)
{
	do{
		U0DBUF = *data++;	
		while( UTX0IF == FALSE );
		UTX0IF = FALSE;
	}while(--len);
}
