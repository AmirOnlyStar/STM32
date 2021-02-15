#include "stm32f10x.h"


void uart_init(void);


int main()
{

	uart_init();
	
	while(1)
	{
		if(USART1 -> SR & USART_SR_RXNE) //if RX is not empty
		{
			char temp = USART1->DR; // fetch the data recived
			USART1 -> DR = temp;
			while(!(USART1 -> SR & USART_SR_TC));
		}
	}
	
	
}


void uart_init(void)
{
	RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN |  RCC_APB2ENR_USART1EN | RCC_APB2ENR_AFIOEN ; //1<<14
	
	//PA9 and PA10 Alternate function output Push-pull (CNF 10 MODE 11)
	GPIOA -> CRH |= GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1; 
	GPIOA -> CRH &= ~(GPIO_CRH_CNF9_0);

	//AFO -> MAPR |= AFIO_MAPR_USART1_REMAP; //for remap from (TX/PA9, RX/PA10) to (TX/PB6, RX/PB7)
		
	//APB1 36 MHz
	//APB2 72 MHz
	
	//clkPer / (boudR * 16) = 72MHz / (9600 * 16)
	//468.75   hex(468) = 0x1D4  hex(0.75*16) = 0xC 
	//USART1 -> BRR	= 0x1d4c;
		
	//clkPer / (boudR * 16) = 72MHz / (115200 * 16)
	//39.0625   hex(39) = 0x27  hex(0.0625*16) = 0x1 
	USART1 -> BRR	= 0x271;
	
	//------------|   TX enable      RX enable       UART enable   
	USART1 -> CR1 |= USART_CR1_TE | USART_CR1_TE  | USART_CR1_UE;
	
}










