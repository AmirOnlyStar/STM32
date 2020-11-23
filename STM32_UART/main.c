#include "stm32f10x.h"

#include "string.h"
#include "stdlib.h"
#include "stdarg.h"

#define DEBUG_UART USART1
#define delay for(int i =0 ; i<500000 ; i++)

void serialInit(void);	
void printMsg(char *msg , ...);

uint8_t counter = 0;
int main()
{
	serialInit();
	while(1)
	{
		printMsg("Hi I am Amir %d \r\n",counter);
		counter++;
	}
}

void serialInit(void)
{
	
RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN | RCC_APB2ENR_USART1EN ; //1<<14
GPIOA -> CRH |= GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1; //PA9 output open drain 
GPIOA -> CRH &= ~(GPIO_CRH_CNF9_0);

//AFO -> MAPR |= AFIO_MAPR_USART1_REMAP; //for remap from (TX/PA9, RX/PA10) to (TX/PB6, RX/PB7)
	
//clkPer / (boudR * 16) = 72MHz / (9600 * 16)
//468.75   hex(468) = 0x1D4  hex(0.75*16) = 0xC 
//USART1 -> BRR	= 0x1d4c;
	
//clkPer / (boudR * 16) = 72MHz / (115200 * 16)
//468.75   hex(39) = 0x27  hex(0.0625*16) = 0x1 
USART1 -> BRR	= 0x271;
	
USART1 -> CR1 |= USART_CR1_TE;
USART1 -> CR1 |= USART_CR1_UE;
}
void printMsg(char *msg , ...)
{
	char buff[80];
	#ifdef DEBUG_UART
		va_list args;
		va_start(args,msg);
		vsprintf(buff,msg,args);
		
		for(int i = 0 ; i<strlen(buff) ;i++)
		{
			USART1 ->DR = buff[i];
			while(!( USART1 -> SR & USART_SR_TXE));
	 	}
	#endif
}
