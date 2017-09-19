#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "iic.h"
#include "th_sensor.h"
#include "lcd.h"

//u16 rh_value,t_value;
u8 data[9];


int main(void)
{	
  u8 i=0;
			
	delay_init();	    //延时函数初始化	
  iic_init();  
	LED_Init();		  	//初始化与LED连接的硬件接口
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(9600);
	sht30_init();

	 
	while(1)
	{
		
		sht30_read_data();
//		at24c02_write(1,"xiaowen");
//		at24c02_read(1);
		if(t_value!=130)
		USART_SendData(USART1,t_value);
		delay_ms(200);
		if(rh_value!=100)
		USART_SendData(USART1,rh_value);
		led_th=!led_th;
		delay_ms(200);
//		delay_ms(500);
	}
}

