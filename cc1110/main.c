#include "ioCC1110.h"
#include "typedef.h"
#include "rf.h"
#include "usart.h"
#include "pwr_clk.h"
#include "ext_int.h"

#define RF_SENDER_RECEIVER 0
#define FRIENDLY_TEST 1				/* button pressed represent reply */
#define TIMEOUT 10

INT8U timeout_count = 0;
INT8U reply_timeout = TRUE;
char new_reply = 0;
char timeout = 0;


const char TIME_OUT[] = "timeout\r\n";
const char USART_INIT[] = "usart inited\r\n";
const char RF_INIT[] = "rf inited\r\n";
const char EN_EXTERN_INT[] = "enable ext int\r\n";
const char EXTERN_INT[] = "extern interrupt\r\n";
const char SENDER[] = "sender..\r\n";
const char RECEIVER[] = "receiver\r\n";
const char WAKE_UP[] = "wake up\r\n";
const char ENTER_PM3[] = "enter pm3\r\n";
#if FRIENDLY_TEST
const char FRIENDLY_REPLY[] = "friendly reply\r\n";
#endif

#pragma vector = ST_VECTOR
__interrupt void sleep_timer_ISR(void)
{

	if( timeout_count < TIMEOUT ){
		P1_0 = ~P1_0;
		timeout_count ++;
	}else{
		usart0_send((char*)TIME_OUT, sizeof(TIME_OUT));
		timeout = 1;
		reply_timeout = TRUE;
		timeout_count = 0;
		disable_sleep_timer();
		/* works well when enable here */
		/* but trapped in main loop */
		usart0_send((char*)EN_EXTERN_INT,sizeof(EN_EXTERN_INT));
		ext_int_on_p1_2();
	}
	/* clear sleep timer interrupt flag */
	IRCON &= ~(1<<7);
	WORIRQ &= ~ 0x01;
}

char recv_resend;
#pragma vector = URX0_VECTOR
__interrupt void USART0_ISR(void)
{
	EA = 0;
	URX0IF = 0 ;
	recv_resend = U0DBUF;
	new_reply = 1;
	EA = 1;
}

#pragma vector = RF_VECTOR
__interrupt void rf_ISR(void)
{
	/* clear rf interrupt flags */
	S1CON &= ~0x03;
	usart0_safe((char*)RF_INIT, sizeof(RF_INIT));
}


#pragma vector = P1INT_VECTOR
__interrupt void p1_int_ISR(void)
{
	IEN2 &= ~0x10;

#if RF_SENDER_RECEIVER
	set_main_clock_source(CRYSTAL);
	while( !XOSC_STABLE );
#endif
	usart0_send((char*)EXTERN_INT, sizeof(EXTERN_INT));

	if( P1IFG & 0x04){
		P1IFG &= ~0x04;
		usart0_send("P1_2\r\n",6);
	}
	
	/* wait for rf idle */
	RFST = SIDLE;
	RFST = SCAL;
	while( MARCSTATE != 0x01);

#if RF_SENDER_RECEIVER
	rf_send_packet((INT8U*)SENDER, sizeof(SENDER));
	rf_send_packet((INT8U*)SENDER, sizeof(SENDER));
	rf_send_packet((INT8U*)SENDER, sizeof(SENDER));
	usart0_send((char*)WAKE_UP, sizeof(WAKE_UP));
#else
	usart0_send((char*)FRIENDLY_REPLY, sizeof(FRIENDLY_REPLY));
	rf_send_packet("a", 1);
	rf_send_packet("a", 1);
	rf_send_packet("a", 1);
#endif

	/* clear all interrupts on P1 */
	P1IFG = 0x00;
	P1IF = 0;
#if RF_SENDER_RECEIVER
	ext_int_on_p1_2();
#endif
}

volatile INT8U len;
int main( void )
{

/* start of the first power on */
	INT8U buffer[10], rssi, lqi;
	set_main_clock_source(CRYSTAL);
	while( !XOSC_STABLE );

	init_usart0();
	usart0_send((char*)USART_INIT, sizeof(USART_INIT));
	RFIM = 0xFF;
	rf_cc1110_init( 433000 );
	usart0_send((char*)RF_INIT, sizeof(RF_INIT));
	/* ues '|=', not '=' */
	IEN0 |= 0x81;
	/* led output */
	P1DIR |= 0x03;

#if RF_SENDER_RECEIVER
	usart0_send((char*)SENDER, sizeof(SENDER));
	enable_sleep_timer();
	ext_int_on_p1_2();
	
#else
	usart0_send((char*)RECEIVER, sizeof(RECEIVER));
#if FRIENDLY_TEST
	ext_int_on_p1_2();
#endif
#endif
/* end of the first power on */



	while( 1 ){
#if RF_SENDER_RECEIVER
		/* RF_SENDER */
		if( timeout ){
			usart0_send((char*)ENTER_PM3, sizeof(ENTER_PM3));
			/* clear memory data or not when entered pm3 */
			set_power_mode(3);
			timeout = 0;
			enable_sleep_timer();

			/* try receiving till timeout */
			while( timeout == 0){
				len = rf_rec_packet(buffer, &rssi, &lqi, 240) ;
				/* if not timeout, and no packet received  
				 * trpped in this loop 
				 * if timeout, break, and fall into pm3 
				 * if received but not timeout, break, and 
				 * do nothing till trapped when timeout 
				 */
				if( len > 0 ){
					P1_1 = ~P1_1;
					usart0_send("got: ", 5);
					usart0_send(buffer, len);
					usart0_send("\r\n", 2);
					len = 0;
					break;
				}
#if 0
				/* 'athome\r\n' */
				if( len == 8 ){
					usart0_send(buffer, len);
					rf_send_packet( buffer, 10 );
				}
				/* 'notathome\r\n' */
				else if( len == 11 ){
					usart0_send(buffer, len);	
				}
#endif
			}
		}
#else	/* RF_RECEIVER */
		if( reply_timeout == TRUE ){
			len = rf_rec_packet(buffer, &rssi, &lqi, 240) ;
			if( len > 0 ) P1_1 = ~P1_1;
			if( len == sizeof(SENDER) ){
				/* set timer to receive timeout */
				enable_sleep_timer();	
				reply_timeout = FALSE;
				/* info stm32 controller */
				usart0_send((char*)SENDER, sizeof(SENDER));
			}
		}else{
			while( reply_timeout == FALSE ){
				if( new_reply ){	
					if( recv_resend == 'a' ) {
						P1_1 = ~P1_1;
						rf_send_packet("a",1);
						usart0_send("a",1);	
						reply_timeout = TRUE;
						new_reply = 0;
					}
					else if( recv_resend == 'n' ){
						P1_1 = ~P1_1;
						rf_send_packet("n",1);
						usart0_send("n",1);	
						reply_timeout = TRUE;
						new_reply = 0;
					}
				}
			}
				
		}
#endif
	}




}
