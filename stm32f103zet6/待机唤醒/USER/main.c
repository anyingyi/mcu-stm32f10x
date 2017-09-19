#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"

 int main(void)
 {	 
		u8 x=0;
		u8 lcd_id[12];			//���LCD ID�ַ���
		 
		delay_init();	    	 //��ʱ������ʼ��	  
		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
		uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
		LED_Init();			     //LED�˿ڳ�ʼ��
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
				  PWR_EnterSTANDBYMode();//����ģʽ
			 }	
       LED0=!LED0;
       delay_ms(500);			 

			
		} 
}
 