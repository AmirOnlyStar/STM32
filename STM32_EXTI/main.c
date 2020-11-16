#include "stm32f10x.h"


int main()
{
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; /*Enable AFIO CLK*/
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; /*Enable GPIOC CLK*/
	/*GPIOC  13 : output push_pull*/
	GPIOC->CRH |= (GPIO_CRH_MODE13_0)|(GPIO_CRH_MODE13_1);
	GPIOC->CRH &= ~((GPIO_CRH_CNF13_0)|(GPIO_CRH_CNF13_1));
	
	/*GPIOC  14 : EXTI input flowting state*/
	GPIOC->CRH &= ~(GPIO_CRH_MODE14_0)|(GPIO_CRH_MODE14_1);
	GPIOC->CRH |= (GPIO_CRH_CNF14_0);
	GPIOC->CRH &= ~(GPIO_CRH_CNF14_1);
	
	EXTI->IMR &= ~(EXTI_IMR_MR14);      
	EXTI->RTSR |= EXTI_RTSR_TR14;
	EXTI->FTSR |= EXTI_FTSR_TR14;	
	/*MAP to pin C14*/
	AFIO->EXTICR[3] &= ~(AFIO_EXTICR4_EXTI14); 
	AFIO->EXTICR[3] |= (AFIO_EXTICR4_EXTI14_PC);
	
	while(1)
	{
	
	}
}

void EXTI14_IRQHandler(void)
{
  if (EXTI->PR & (1<<14)) 
	{                        // EXTI14 interrupt pending?
    EXTI->PR |= (1<<14);      // clear pending interrupt

  }
}