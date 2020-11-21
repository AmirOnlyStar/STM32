#include "stm32f10x.h"
volatile uint32_t msTicks = 0;   

int main()
{
	
	SystemCoreClockUpdate();
	uint32_t returnCode;
  returnCode = SysTick_Config(SystemCoreClock / 1000); 	
	while(1)
	{	

	}		

}

void SysTick_Handler(void)
{
	msTicks++;
}

void delayMs(int ms)
{
	msTicks = 0;
	while(msTicks < ms);
}






