#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "sys.h"  
#include "stm32f10x.h"
#include "stm32f10x_flash.h"

#define STM32_FLASH_SIZE 256 
#define STM32_FLASH_BASE 0x08000000
#define STM32_FLASH_BOOTLOADER_SIZE 0x5000
 
 
u16 STMFLASH_ReadHalfWord(u32 faddr);
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);
						   
#endif

















