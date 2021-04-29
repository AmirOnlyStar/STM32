#include "stm32f10x.h"

#define PORT_SS_HIGH  GPIOA ->BSRR =  GPIO_BSRR_BS3
#define PORT_SS_LOW   GPIOA ->BSRR =  GPIO_BSRR_BR3

void spi_init(void);

uint8_t spi_sr_8bit(uint8_t);

int main()
{
	spi_init();
	PORT_SS_HIGH;
	while(1)
	{
		spi_sr_8bit(0x22);
		
//		PORT_SS_LOW;
//		PORT_SS_HIGH;
	
	}

	
	
}


void spi_init(void)
{
//enable RCC SPI1 enable GPIOA clock for SPI ( PA3 SS | PA5 SCK | PA6 MISO | PA7 MOSI )	
RCC -> APB2ENR |= (RCC_APB2ENR_SPI1EN) | (RCC_APB2ENR_IOPAEN);
	
/* CONFIG GPIO FOR SPI1 */
	
/*config for SS pin for Master in 00: General purpose output push-pull*/	
//MODE 11 : OUTPUT 50 MHz 
GPIOA -> CRL |= (GPIO_CRL_MODE3_0) | (GPIO_CRL_MODE3_1);
//CNF  00: General purpose output push-pull
GPIOA -> CRL &= ~((GPIO_CRL_CNF3_0) | (GPIO_CRL_CNF3_1));
 
/*config for SCK pin for Master in Alternate function push-pull	*/
//MODE 11: OUTPUT 50MHz
GPIOA -> CRL |= (GPIO_CRL_MODE5_0) | (GPIO_CRL_MODE5_1);
//CNF 10 : Alternate function output Push-pull
GPIOA -> CRL &= ~(GPIO_CRL_CNF5_0);
GPIOA -> CRL |=  (GPIO_CRL_CNF5_1);
	
/* config for MISO pin for Master in Input pull-up */
//MODE 00:INPUT
GPIOA -> CRL &= ~((GPIO_CRL_MODE6_0) | (GPIO_CRL_MODE6_1));
//CNF 10: Input with pull-up / pull-down
GPIOA -> CRL &= ~(GPIO_CRL_CNF6_0);
GPIOA -> CRL |=  (GPIO_CRL_CNF6_1);	
	
/*config for MOSI pin for Master in Alternate function push-pull*/		
//MODE OUTPUT 11 : 50MHz
GPIOA -> CRL |= (GPIO_CRL_MODE7_0) | (GPIO_CRL_MODE7_1);
//CNF 10 : Alternate function output Push-pull
GPIOA -> CRL &= ~(GPIO_CRL_CNF7_0);
GPIOA -> CRL |=  (GPIO_CRL_CNF7_1);

/*****************************************************************/	

//// BRR 001: fPCLK/4
//SPI1 -> CR1 |= SPI_CR1_BR_0;
//SPI1 -> CR1 &= ~( (SPI_CR1_BR_1) | (SPI_CR1_BR_2) );

////BRR 001: fPCLK/256
SPI1 -> CR1 |= (SPI_CR1_BR_0) | (SPI_CR1_BR_1) | (SPI_CR1_BR_2) ;


// MASTER SELECTION 1:MASTER | 
SPI1 -> CR1 |= (SPI_CR1_MSTR)  ; 
// Set data format to 0:8 bit | 0:MSBFIRST |
SPI1 -> CR1 &= ~( (SPI_CR1_DFF)| (SPI_CR1_LSBFIRST) ); 


SPI1 -> CR1 |= (SPI_CR1_SPE); //enable SPI

}

uint8_t spi_sr_8bit(uint8_t data)
{
	uint8_t temp; 
	
	PORT_SS_LOW;
	SPI1->DR = data;
//	while(((SPI1->SR) & SPI_SR_BSY) );
	while(!((SPI1->SR) & SPI_SR_TXE));
	
//	while(!((SPI1->SR) & SPI_SR_RXNE) );
//	temp = SPI1->DR ;
	PORT_SS_HIGH;
//	return temp;
}




