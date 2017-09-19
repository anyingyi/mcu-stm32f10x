#include"LED.h"
#include"delay.h"


int main(void)

{        
    CRS485_Init1();
	  delay_init();
    while(1)
		{
				USART_SendData(UART4,45);
		    delay_ms(500);
		}

}
