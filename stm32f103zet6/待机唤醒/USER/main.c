#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"

 int main(void)
 {	 
		u8 x=0;
		u8 lcd_id[12];			//存放LCD ID字符串
		 
		delay_init();	    	 //延时函数初始化	  
		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
		uart_init(115200);	 	//串口初始化为115200
		LED_Init();			     //LED端口初始化
		LCD_Init();
	  KEY_Init();
		POINT_COLOR=RED;
		 
		
		
//		LCD_ShowString(60,0,200,600,24,"friend"); 
//		LCD_ShowString(30,70,100,200,16,"TFTLCD TESTfdajklwhejirquoiwuhj\nkasdflkwheqirukafhhhhsjk");			   		 
//		//delay_ms(1000);	
		 
			while(1) 
		{	
			 if(KEY_Scan(0)==1)	
			 {
          RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);			
          PWR_WakeUpPinCmd(ENABLE);
				  PWR_EnterSTANDBYMode();//待机模式
			 }	
       LED0=!LED0;
       delay_ms(500);			 

			
		} 
}
 