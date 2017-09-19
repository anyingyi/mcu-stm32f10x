#include "sys.h"	
#include "delay.h"	
#include "modbus.h"
#include "led.h"
#include "key.h"
#include "iic.h"
#include "th_sensor.h"

 
/************************************************
 ALIENTEK战舰STM32开发板实验2
 蜂鸣器实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

int main(void)
{
		delay_init();	    	 //延时函数初始化
		KEY_Init();
		RS485_Init();
		Timer7_Init();
		LED_Init();
		iic_init();
	  sht30_init();
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		while(1)
		{
			sht30_read_data();
			USART_SendData(USART1,rh_value);
			RS485_Service();
		}
}

