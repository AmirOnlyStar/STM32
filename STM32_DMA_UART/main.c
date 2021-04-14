#include "stm32f10x.h"
#include "string.h"

#include "string.h"
#include "stdlib.h"
#include "stdarg.h"

#define DEBUG_UART USART2
void printMsg(char *msg , ...);

char myString[]="Hello\n";
char myString2[50];
	
uint8_t counter = 0;

int main()
{
	
	uint8_t len = strlen(myString);
//	uint8_t count = 0;
	
	/********* RCC config ************/
	RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN ;
	RCC -> APB1ENR |= RCC_APB1ENR_USART2EN ;
	RCC	->AHBENR |= RCC_AHBENR_DMA1EN;
	
	
	/**** GPIO config UART2 PA2 TX and PA3 RX ****/
	GPIOA -> CRL |= GPIO_CRL_CNF2_1 | GPIO_CRL_MODE2_0 | GPIO_CRL_MODE2_1;//PA2  Alternate function output Push-pull
	GPIOA -> CRL &= ~(GPIO_CRL_CNF2_0);
	
	/******** USART config**********/
	//FOR APB1 CLK/2	
	//clkPer / (boudR * 16) = 36MHz / (115200 * 16) = 234.375
	//234.375   hex(234) = 0xEA  hex(0.375*16) = 0x6 
	USART2 -> BRR = 0xEA6; //config usart boud rate 

	USART2 -> CR1 |= USART_CR1_TE; //TE: Transmiter mode
	USART2 -> CR1 |= USART_CR1_RE; //RE: Reciver mode
	USART2 -> CR1 |= USART_CR1_UE; //UE: USART enable
	USART2 -> CR3 |= USART_CR3_DMAT; //Enable DMA transmirt for usart2 
	USART2 -> CR3 |= USART_CR3_DMAR; //Enable DMA recive for usart2 
	
	/********* DMA config **********/
	//	Memory increment mode| Read from memory | transfer compelit interupt| circular mod 
	DMA1_Channel7 ->CCR |= DMA_CCR7_MINC | DMA_CCR7_DIR | DMA_CCR7_TCIE | DMA_CCR7_CIRC;
	DMA1_Channel7 ->CPAR = (uint32_t)&USART2->DR;
	DMA1_Channel7 ->CMAR = (uint32_t)myString;
	DMA1_Channel7 ->CNDTR = len;
	
  //DMA1_Channel7 -> CCR |= DMA_CCR7_EN;
	
	//Memory increment mode | transfer compelit interupt| circular mod 
	DMA1_Channel6 -> CCR |= DMA_CCR6_MINC  | DMA_CCR6_TCIE | DMA_CCR6_CIRC ;
	//Read from peripheral
	DMA1_Channel6 -> CCR  &= ~DMA_CCR6_DIR;
	DMA1_Channel6 -> CPAR  = (uint32_t)&USART2->DR;
	DMA1_Channel6 -> CMAR  = (uint32_t)&myString2;
	DMA1_Channel6 -> CNDTR = sizeof(myString2);
	
	DMA1_Channel6 -> CCR |= DMA_CCR6_EN;
	
	NVIC_EnableIRQ(DMA1_Channel7_IRQn);/*enable interupt in core_cm3.h*/
	NVIC_EnableIRQ(DMA1_Channel6_IRQn);/*enable interupt in core_cm3.h*/

	while(1)
	{
		/*********** manual transfer and recive ***********/
//		char temp;
//		if(USART2 -> SR & USART_SR_RXNE) //if RX is not empty
//		{
//			char temp = USART2->DR; // fetch the data recived
//			USART2 -> DR = temp;
//			while(!(USART2 -> SR & USART_SR_TC));
//		}
		/**************************************************/
	}

}
void DMA1_Channel7_IRQHandler(void)
{
//	while((DMA1 ->ISR & DMA_ISR_TCIF7) == 0);
	DMA1 ->IFCR = DMA_IFCR_CTCIF7;
//	DMA1_Channel7 -> CCR &=~DMA_CCR7_EN;
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
			USART2 ->DR = buff[i];
			while(!( USART2 -> SR & USART_SR_TXE));
	 	}
	#endif
}
