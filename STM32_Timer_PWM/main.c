/*********************************
timer 4 output 4 PB9 
**********************************/

#include "stm32f10x.h"
uint32_t temp_timer;

int main()
{
	
	RCC -> APB2ENR |= (RCC_APB2ENR_IOPBEN)| (RCC_APB2ENR_AFIOEN) ;
	RCC -> APB1ENR |= (RCC_APB1ENR_TIM4EN);
	
	//PWM freq = Fclk/PSC/ARR  (72000000/72/1000) pwmoutput 1KHz
	//PWM Duty = CCR4/ARR
//	TIM4 -> PSC = 72; /*PRESCALER*/
//	TIM4 -> ARR = 1000;  /*Auto Reload Register reach to this */
//	TIM4 -> CCR4 = 250;
	
		//PWM freq = Fclk/PSC/ARR  (72000000/144/10000) pwmoutput 50Hz
	//PWM Duty = CCR4/ARR
	TIM4 -> PSC = 144; /*PRESCALER*/
	TIM4 -> ARR = 10000;  /*Auto Reload Register reach to this */
	
	
	/*************** PWM ON CHANEL 4************/
	/*config pin as ALT FUNCT OUTPUT PUSH PULL @50 MHz*/
	GPIOB->CRH |= GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1 | GPIO_CRH_CNF9_1; 
	GPIOB->CRH &= ~(GPIO_CRH_CNF9_0);
	//capture/compare enable register
	TIM4 -> CCER |= TIM_CCER_CC4E;
	/*
	PWM mode 1 - In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1
	else inactive. In downcounting, channel 1 is inactive (OC1REF=‘0) as long as
	TIMx_CNT>TIMx_CCR1 else active (OC1REF=1)
	*/
	TIM4 -> CCMR2 |= (TIM_CCMR2_OC4M_2) | (TIM_CCMR2_OC4M_1) ;
	TIM4 -> CCMR2 |= (TIM_CCMR2_OC4PE);
	TIM4 -> CCR4 = 2500;
	/*******************************************/
	/*************** PWM ON CHANEL 3************/
	/*config pin as ALT FUNCT OUTPUT PUSH PULL @50 MHz*/
	GPIOB->CRH |= GPIO_CRH_MODE8_0 | GPIO_CRH_MODE8_1 | GPIO_CRH_CNF8_1; 
	GPIOB->CRH &= ~(GPIO_CRH_CNF8_0);
	
	//capture/compare enable register
	TIM4 -> CCER |= TIM_CCER_CC3E;
	TIM4 -> CCMR2 |= (TIM_CCMR2_OC3M_2) | (TIM_CCMR2_OC3M_1) ;
	TIM4 -> CCMR2 |= (TIM_CCMR2_OC3PE);
	TIM4 -> CCR3 = 1000;
	/*******************************************/
	
	/*************** PWM ON CHANEL 2************/
	/*config pin as ALT FUNCT OUTPUT PUSH PULL @50 MHz*/
	GPIOB->CRL |= GPIO_CRL_MODE7_0 | GPIO_CRL_MODE7_1 | GPIO_CRL_CNF7_1; 
	GPIOB->CRL &= ~(GPIO_CRL_CNF7_0);
	
	//capture/compare enable register
	TIM4 -> CCER |= TIM_CCER_CC2E;
	TIM4 -> CCMR1 |= (TIM_CCMR1_OC2M_2) | (TIM_CCMR1_OC2M_1) ;
	TIM4 -> CCMR1 |= (TIM_CCMR1_OC2PE);
	TIM4 -> CCR2 = 3000;
	/*******************************************/
	
	
	//Auto-reload preload enable
//	TIM4 -> CR1  |= TIM_CR1_ARPE;
//	TIM4 -> EGR |= TIM_EGR_UG;
	
	TIM4->CR1 |= TIM_CR1_CEN; /*Counter enable*/
	
	while(1)
	{
		temp_timer = TIM4-> CNT;	
	}
}











