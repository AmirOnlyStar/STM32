#include "stm32f10x.h"
#include "string.h"

int main()
{
	
	char myString[]="Hello";
	uint8_t len = strlen(myString);
	uint8_t count = 0;
	
	//####################################################
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	USART2->CR3 |= USART_CR3_DMAT;
	
	DMA1_Channel7 ->CNDTR = len;
	DMA1_Channel7 ->CCR |= DMA_CCR4_MINC | DMA_CCR7_DIR | DMA_CCR7_TCIE | DMA_CCR7_CIRC;
	
	DMA1_Channel7 ->CPAR = (uint32_t)&USART2->DR;
	DMA1_Channel7 ->CMAR = (uint32_t)myString;
	//####################################################
	
	USART2 ->BRR = 0xEA6; //config usart boud rate 
	USART2 -> CR1 |= USART_CR1_TE; //TE: Transmiter mode
	USART2 -> CR1 |= USART_CR1_UE; //UE: USART enable
	
	
//	while(1)
//	{
//		for(count = 0 ;count < len ; count++)
//		{
//			USART2 -> DR = myString[count];
//			while((USART2->SR & USART_SR_TC) == 0);
//		}
//	}

}


