#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
#include "timer.h"
#include "master.h"
#include "slave.h"
#include "adc.h"
#include "th_sensor.h"
#include "sensor.h"
#include "computer.h"

extern u8 RS485_RX_BUFF[2048];//接收缓冲区2048字节
u8 PM_BUFF[16];

int main(void)
{

		SystemInit();
		delay_init();	    	 //延时函数初始化
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		key_init();
	  led_init();
		
	  Adc_Init();
	  sht30_init();
	  sht30_read_data();
	  

	  //master
		if(ADR_)
		{
				RS485_Init();
			  CRS485_Init1();
				while(1)
				{
						if(!ADR_)
								SoftReset();
						printf(RS485_RX_BUFF);
						CRS485_Service();
						delay_ms(100);
						
				}
		}
		
		//slave
		if(!ADR_)
		{
				RS485_Init1();
				while(1)
				{
						if(ADR_)
								SoftReset();
					  RS485_Service();
						Sensor_PM_GPIOInit();
						Sensor_PM_SendCommand();
				}
		}			
}






