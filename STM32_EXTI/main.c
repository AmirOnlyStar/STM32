#include "stm32f10x.h"

/*EXTERnal interupt test*/
/*STM32F103C8T6 run at 8 MHz and PINC13 OUTPUT and PINC14 interupt INPUT  */
/*end of startup_stm32f10x.h IRQ handeler exist*/

int main()
{
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; /*Enable AFIO CLK*/
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; /*Enable GPIOC CLK*/
	/*GPIOC  13 : output push_pull*/
	GPIOC->CRH |= (GPIO_CRH_MODE13_0)|(GPIO_CRH_MODE13_1);
	GPIOC->CRH &= ~((GPIO_CRH_CNF13_0)|(GPIO_CRH_CNF13_1));
	
	/*GPIOC  14 : EXTI input PULLUP_PULLDOWN*/
	/*input PULLUP_PULLDOWN*/
	GPIOC->CRH &= ~((GPIO_CRH_MODE14_0)|(GPIO_CRH_MODE14_1));
	
	/*INPUT PULLUP_PULLDOWN*/
	GPIOC->CRH |= (GPIO_CRH_CNF14_1);
	GPIOC->CRH &= ~(GPIO_CRH_CNF14_0);
	GPIOC->ODR |= (GPIO_ODR_ODR14); /*PULL_UP*/
  /*unmask register */
	EXTI->IMR |= (EXTI_IMR_MR14);      
	//EXTI->RTSR |= EXTI_RTSR_TR14; /*Rising edge */
	EXTI->FTSR |= EXTI_FTSR_TR14;	  /*Failing edge */
	
	/*AFIO->EXTICR to pin C14*/
	AFIO->EXTICR[3] &= ~(AFIO_EXTICR4_EXTI14); /*clear the EXTI14*/
	AFIO->EXTICR[3] |= (AFIO_EXTICR4_EXTI14_PC);
	
	NVIC_EnableIRQ(EXTI15_10_IRQn);/*enable interupt in core_cm3.h*/
	
	while(1)
	{
	
	}
}

void EXTI15_10_IRQHandler(void)
{
//  if (EXTI->PR & (1<<14))
	if (EXTI->PR & (EXTI_PR_PR14))	
	{                        // EXTI14 interrupt pending?
    EXTI->PR |= (1<<14);      // clear pending interrupt
		GPIOC->ODR ^= (1<<13);
		for(int i =0 ;i < 100000;i++);
  }
}

