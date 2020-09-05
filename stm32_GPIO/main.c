#include "stm32f10x.h"

int main(void)
{
	RCC->APB2ENR = (1<<4);
	GPIOC->CRH |= ((1<<20)|(1<<21));
	GPIOC->CRH &= ~((1<<22)|(1<<23));
	
	while(1)
	{
		GPIOC->BSRR = (1<<13);
		for(int i =0 ;i < 500000;i++);
		GPIOC->BSRR = (1<<(13+16));
		for(int i =0 ;i < 500000;i++);
		
		
	}	
	

}

