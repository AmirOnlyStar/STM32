#include "stm32f10x.h"

#define BUFFLEN 20

volatile uint32_t msTicks = 0;   

void init_i2c2(void);
void i2c_write_single(uint8_t device_address , uint8_t mem_address , uint8_t data);
void i2c_read_single(uint8_t device_address , uint8_t len);


uint8_t i2c_buff[BUFFLEN];

int main()
{
	SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock / 1000); 	
	
	init_i2c2();
	while(1)
	{
		i2c_write_single(0xA6,0x00,0x00);
		i2c_read_single(0xA6,1);
		__NOP();
	}
	
}


void init_i2c2(void)
{
	RCC->APB1ENR &= ~(RCC_APB1ENR_I2C2EN); //disable i2c_2
	I2C2->CR1 = 0x00;
	I2C2->CR2 = 0x00;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;    //enable i2c2
	
	//NVIC_EnableIRQ(I2C2_EV_IRQn);
	
	I2C2->CR2 |= 36; //freq value of APB1 buss = 36
	/*******************************************************  
	muu_freq : 72 MHz
	APB1: 36 MHz
	TAPB1 = 1/APB1 = 27.7 ns
	i2c = 100 KHz
	Ti2c = 1/i2c  = 10 us 
	
	CCR = (Ti2c/2)/TAPB1  = (10 us / 2) / 27.7 ns = 180
	
	********************************************************/
	
	I2C2->CCR |= 180;

	/*******************************************************
	1000 ns / (1 / 36 MHz) => 36 + 1
	
	*******************************************************/
	I2C2->TRISE |=37; 
	I2C2->CR1 |= I2C_CR1_ACK; //enable acks
	//stretch mode is enable by deffult 
	//7 bit adressing mode enable by deffault(not for F1)
	//config GPIO for i2c use enable internal pull up
	GPIOB ->CRH |= GPIO_CRH_CNF10 | GPIO_CRH_MODE10 |GPIO_CRH_CNF11 | GPIO_CRH_MODE11 ;//Alt funtion output open drain
	
	//GPIOB->ODR |= (1<<10)|(1<<11);//internal pull up 
	
	//I2C2->CR2 |= I2C_CR2_ITBUFEN;// enable corresponding interrupts (buffer interrupts)
	//I2C2->CR2 |= I2C_CR2_ITEVTEN; //(event interupt)
	I2C2->CR1 |= I2C_CR1_PE;//enable peripheral needs to be done last
	
}


void i2c_write_single(uint8_t device_address , uint8_t mem_address , uint8_t data)
{
	uint32_t temp;
	
	I2C2->CR1 |= I2C_CR1_START;//generate start condition
	//MSTICKS = 0;
	while(!(I2C2->SR1 & I2C_SR1_SB)){ //start condition was sent
	
	}
	
	I2C2->DR = device_address;
	while(!(I2C2->SR1 & I2C_SR1_ADDR)){//address was sent 
	}
	
	I2C2->DR = mem_address; //addrress to write to 
	while(!(I2C2->SR1 & I2C_SR1_TXE)){//waite for byte to transfer 
	}
	
	I2C2->DR = data;//data to transfer 
	
	while(!(I2C2->SR1 & I2C_SR1_TXE)){
	}
	I2C2->CR1 |= I2C_CR1_STOP;//send anather start condition to get the data 
	
}

void i2c_read_single(uint8_t device_address , uint8_t len)
{
	uint32_t temp = 0;
	
	RCC -> AHBENR |= RCC_AHBENR_DMA1EN;
	I2C2 ->CR2 |= I2C_CR2_DMAEN;
	I2C2 ->CR1 |= I2C_CR1_ACK; //enable acks
	
	DMA1_Channel5 -> CMAR = (uint32_t)i2c_buff;
	DMA1_Channel5 -> CPAR = (uint32_t)&I2C2->DR;
	DMA1_Channel5 -> CNDTR = len;
	DMA1_Channel5 -> CCR |= DMA_CCR4_TCIE | DMA_CCR5_MINC | DMA_CCR5_EN;//enable i2c2 transfer interupt enable :Memory increment mode
	
	I2C2 -> CR1 |= I2C_CR1_START;//generate start condition
	
	while(!(I2C2->SR1 & I2C_SR1_SB))
	{
	}
	
	I2C2 -> DR = device_address +1 ;//write 0xA0
	while(!(I2C2 -> SR1 & I2C_SR1_ADDR))
	{
	}
	
	temp = I2C2->SR2;
	while(!(DMA1->ISR & DMA_ISR_TCIF5) == 0);
	
	I2C2->CR1 |= I2C_CR1_STOP;
	
	
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


