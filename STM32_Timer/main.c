#include "stm32f10x.h"

#include "string.h"
#include "stdlib.h"
#include "stdarg.h"

#define DEBUG_UART USART1
#define delay for(int i =0 ; i<500000 ; i++)

void serialInit(void);	
void printMsg(char *msg , ...);

uint8_t counter = 0;

uint32_t val;

int main()
{
RCC -> APB2ENR |=  RCC_APB2ENR_IOPBEN |RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN | RCC_APB2ENR_USART1EN ; //1<<14
RCC -> APB1ENR |= RCC_APB1ENR_TIM4EN;
	
GPIOA -> CRH |= GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1; //PA9 output  Alternate function output Push-pull
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

/*config pin as ALT FUNCT OUTPUT PUSH PULL @50 MHz*/
	GPIOB->CRH |= GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1 | GPIO_CRH_CNF9_1; 
	GPIOB->CRH &= ~(GPIO_CRH_CNF9_0);
	
//<<<<<<< HEAD
	//PWM freq = Fclk/PSC/ARR
	//PWM Duty = CCR4/ARR
	
	TIM4->PSC = 65535;
	TIM4->ARR = 6000;
//	TIM4->CR1 |= TIM_CR1_CEN | TIM_CR1_DIR;
	TIM4->CR1 |= TIM_CR1_CEN;

	TIM4->PSC = 65535; /*PRESCALER*/
	TIM4->ARR = 6000;  /*Auto Reload Register reach to this */
//	TIM4->CR1 |=  TIM_CR1_DIR; /*0 UP counter & 1 DOWN counter*/
	TIM4->CR1 |= TIM_CR1_CEN; /*Counter enable*/
	
	/*Center-aligned mode selection*/
//>>>>>>> f8ec9de3dff68fa1f50de6f53377540caf460572
//	TIM4->CR1 |= TIM_CR1_CMS_0;
//	TIM4->CR1 &= ~TIM_CR1_CMS_1;
	
	printMsg("Hi I am Amir %d \r\n",counter);
	while(1)
	{
		val = TIM4->CNT;
		printMsg("CNT:%d\r\n",val);
		counter++;
	}
	
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





