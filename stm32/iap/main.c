#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define ApplicationAddress    0x8005000
typedef  void (*pFunction)(void);
pFunction Jump_To_Application;
uint32_t JumpAddress;


GPIO_InitTypeDef GPIO_InitStructure;

void LED_Init(void)
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		// configurates specific pins
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void LED_blink(void)
{
		int i = 0 ;
		i = 0 ;
	
		LED_Init();
	
		GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3) ;
		GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9) ;
		for( i = 0 ; i < 5000000 ; i ++ ) ;
		GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3) ;
		GPIO_ResetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9) ;
		for( i = 0 ; i < 5000000 ; i ++ ) ;
		GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3) ;
		GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9) ;
		for( i = 0 ; i < 5000000 ; i ++ ) ;
		GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3) ;
		GPIO_ResetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9) ;
		
		GPIO_DeInit(GPIOA);
		GPIO_DeInit(GPIOB);
}

int main()
{
	LED_blink() ;
	/* Jump to user application */
	if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000) { 
		JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
		Jump_To_Application = (pFunction) JumpAddress;
		__set_MSP(*(__IO uint32_t*) ApplicationAddress);
		Jump_To_Application();
	}
}

