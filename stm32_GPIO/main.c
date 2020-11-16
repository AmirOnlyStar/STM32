#include "stm32f10x.h"

int main(void)
{

	/**************************** 72 MHz**************************/
	//turn on external crystal
	RCC->CR |= RCC_CR_HSEON;
	//wait for HSE crystal be stable
	while(!(RCC->CR & RCC_CR_HSERDY));

	//activate prefetch buffer but it should already be on
	FLASH->ACR |= FLASH_ACR_PRFTBE;

	// Flash 1 wait state 
	FLASH->ACR &= ~(FLASH_ACR_LATENCY); //reset just to be sure
	FLASH->ACR |= 0x2;   

	//configure RCC and PLL settings while PLL is off
	RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE |  RCC_CFGR_PLLMULL); //reset

	RCC->CFGR &= ~(RCC_CFGR_PLLXTPRE);  //PLLXTPRE bit set to 0
	RCC->CFGR |= RCC_CFGR_PLLSRC;     	//pll source
	RCC->CFGR |= RCC_CFGR_PLLMULL9;   	//pll miultiplier 
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;  //AHB prescaler
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;  //APB1 presacaler 
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;  //APB2 prescaler

 //turn on PLL
  RCC->CR |= RCC_CR_PLLON; 
  while (!(RCC->CR & RCC_CR_PLLRDY));

 //set pll as clock source
  RCC->CFGR &= ~(RCC_CFGR_SW);
  RCC->CFGR |= RCC_CFGR_SW_PLL;
  while (!(RCC->CFGR & RCC_CFGR_SWS_PLL));
/******************************************************/

/**************************** 8 MHz**************************/
//	//turn on external crystal
//	RCC->CR |= RCC_CR_HSEON;
//	//wait for HSE crystal be stable
//	while(!(RCC->CR & RCC_CR_HSERDY));
//	
//  RCC->CFGR &= ~(RCC_CFGR_SW);
//	RCC->CFGR |= (RCC_CFGR_SW_0);
//  while (!(RCC->CFGR & RCC_CFGR_SWS_HSE));
/******************************************************/

//	SystemCoreClockUpdate();
//	SysTick_Config(SystemCoreClock/1000);
	
	
	RCC->APB2ENR |= (RCC_APB2ENR_IOPCEN);/*1<<4)*/
//	GPIOC->CRH |= ((1<<20)|(1<<21));
//	GPIOC->CRH &= ~((1<<22)|(1<<23));
		GPIOC->CRH |= (GPIO_CRH_MODE13_0)|(GPIO_CRH_MODE13_1);
		GPIOC->CRH &= ~((GPIO_CRH_CNF13_0)|(GPIO_CRH_CNF13_1));
	
	while(1)
	{
		//GPIOC->BSRR = (1<<13);
//		GPIOC->BSRR = (8192);
//		for(int i =0 ;i < 500000;i++);
//		GPIOC->BSRR = (1<<(13+16));
//		GPIOC->BSRR = (536870912);
//		GPIOC->BSRR = (8192);
//		GPIOC->BSRR = (536870912);
//		GPIOC->BSRR = (8192);
//		GPIOC->BSRR = (536870912);
//		for(int i =0 ;i < 500000;i++);
		
		GPIOC->ODR ^= (1<<13);

	__nop();
	}	
	

}

