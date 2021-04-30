#include "stm32f10x.h"
volatile uint32_t msTicks = 0;   
void sysTick_myinit(void);
void delayMs(int ms);

int main()
{
	
	sysTick_myinit();
	while(1)
	{	

	}		

}

void sysTick_myinit(void)
{
	SystemCoreClockUpdate();
	uint32_t returnCode;
  returnCode = SysTick_Config(SystemCoreClock / 1000); 	
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






