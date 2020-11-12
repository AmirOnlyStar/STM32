#include "stm32f10x.h"

int main(void)
{
	RCC->CR = (RCC_CR_HSEON) |(RCC_CR_PLLON); /*(1<<16)|(1<<24)*/
	RCC->CFGR = (RCC_CFGR_SW_1)|(RCC_CFGR_PPRE1_2)|(RCC_CFGR_PLLSRC)|(RCC_CFGR_PLLMULL_2)|(RCC_CFGR_PLLMULL_1)|(RCC_CFGR_PLLMULL_0);
	RCC->CFGR &= ~(RCC_CFGR_SW_0)|(RCC_CFGR_PPRE1_1)|(RCC_CFGR_PPRE1_0)|(RCC_CFGR_PLLMULL_3);
	
	
	RCC->APB2ENR = (1<<4);
	
	GPIOC->CRH |= ((1<<20)|(1<<21));
	GPIOC->CRH &= ~((1<<22)|(1<<23));
	
	while(1)
	{
//		GPIOC->BSRR = (1<<13);
//		for(int i =0 ;i < 500000;i++);
//		GPIOC->BSRR = (1<<(13+16));
//		for(int i =0 ;i < 500000;i++);
		GPIOC->ODR ^= (1<<13);
		//for(int i =0 ;i < 500000;i++);
		//GPIOC->ODR = 1 << 13;
		//GPIOC->ODR = 0;
	}	
	

}

