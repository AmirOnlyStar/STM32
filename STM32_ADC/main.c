#include "stm32f10x.h"
unsigned long int msTicks;

void delayMs(int ms);


int main()
{
	SystemCoreClockUpdate();
//	SysTick_Config(SystemCoreClock/1000);
	
	//Change prescaler for ADC to not excead 14MHz
	RCC->CFGR |= (RCC_CFGR_ADCPRE_DIV6);
	//RCC CLK for ADC1 and ALT funtion
	RCC->APB2ENR|=(RCC_APB2ENR_ADC1EN)|(RCC_APB2ENR_IOPAEN)|(RCC_APB2ENR_AFIOEN);
	//Config pins at input push pull
	GPIOA->CRL |= GPIO_CRL_CNF5_1;
	GPIOA->CRL &= ~(GPIO_CRL_CNF5_0);
	
	//Enable end of conversion interupt
	ADC1 ->CR1 |= (ADC_CR1_EOCIE);
	//Enable that interupt in NVIC
	NVIC_EnableIRQ(ADC1_2_IRQn);
	
	
	//Set the sampeling rate
	ADC1 ->SMPR2 |= (ADC_SMPR2_SMP5_2) | (ADC_SMPR2_SMP5_0) | (ADC_SMPR2_SMP5_1) ;
	//Set the chanel you want to convert in the sequence registers 
	ADC1 ->SQR3 |= (ADC_SQR3_SQ1_0) | (ADC_SQR3_SQ1_2);
	//Enable the ADC for the first time and set it to continous mpode
	ADC1 ->CR2 |= (ADC_CR2_ADON) | (ADC_CR2_CONT);
	
	delayMs(1);
	
	//turn on ADC for the second time to actually turn it on 
	ADC1 ->CR2 |= ADC_CR2_ADON;
	delayMs(1);
	//Run the calibration
	ADC1 ->CR2 |= ADC_CR2_CAL;
	delayMs(2);
	
	
	
	while(1)
	{	

	}		
	
}

void delayMs(int ms)
{
	msTicks = 0;
	while(msTicks < ms);
}
		 
void SysTick_Handler(void)
{
	msTicks++;
}


