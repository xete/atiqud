
#ifndef _RF_CC1110_H_
#define _RF_CC1110_H_

#include "typedef.h"
#include "ioCC1110.h"

#define SFSTXON			0x00
#define SCAL			0x01
#define SRX				0x02
#define STX				0x03
#define SIDLE			0x04
#define SAFC			0x05
#define SNOP			0xff

/* set the frequency of the rf radio, KHZ  */
void rf_cc1110_freq( INT32U freq_KHZ );

/* Initialize the device with a frequency ( KHZ )*/
void rf_cc1110_init( INT32U freq_KHZ );

/*receive a packet and returns the count*/
INT8U rf_rec_packet(INT8U* pData, INT8U* pRssi, INT8U* pLqi, INT8U timeOut);

/*send a packet and returns the count*/
INT8U rf_send_packet(INT8U* pData, INT8U length);













#endif //_RF_CC1110_H_


