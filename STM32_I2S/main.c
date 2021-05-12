/**************************************
        PIN      |  REMAP=0  | REMAP=1
-------------------------------------
SPI3_NSS/I2S3_WS     PA15    	  PA4
SPI3_SCK/I2S3_CK     PB3 			  PC10
SPI3_MISO            PB4 			  PC11
SPI3_MOSI/I2S3_SD    PB5 			  PC12

____________________________________________
I2Sx_ WS	
Master 				Alternate function push-pull
Slave  				Input floating

I2Sx_CK	
Master 				Alternate function push-pull
Slave 				Input floating

I2Sx_SD
Transmitter 	Alternate function push-pull
Receiver 			Input floating/ Input pull-up/ Input pull-down

2Sx_MCK
Master 				Alternate function push-pull
Slave 				Not used. Can be used as a GPIO

***************************************/



#include "stm32f10x.h"


void i2c_init(void);

int main()
{
	i2c_init();
	while(1)
	{
	
	
	}
}


void i2c_init(void)
{
RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	
	

}



