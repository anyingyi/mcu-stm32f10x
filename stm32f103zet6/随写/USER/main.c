#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include"iic.h"


int main(void)
{	
	delay_init();	    //延时函数初始化	
  IIC_Init();  
	LED_Init();		  	//初始化与LED连接的硬件接口
	uart_init(9600);
	 
	while(1)
	{
		at24c02_write_byte(0,0x45);
		USART_SendData(USART1,AT24CXX_ReadOneByte(0));
		delay_ms(500);
	}
}

