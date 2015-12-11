#ifndef __USART_H__
#define __USART_H__
#include "ioCC1110.h"
#include "sys.h"
#include "pwr_clk.h" /* for xosc state in usart0_safe */

void init_usart0(void);
void usart0_send(char* data, int len);
void usart0_safe(char* data, int len);

#endif
