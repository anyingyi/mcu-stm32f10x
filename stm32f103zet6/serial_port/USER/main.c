#include "sys.h"
#include "delay.h"
//#include "usart.h"
#include "led.h"
#include"serial.h"


 int main(void)
 {	
		delay_init();	    //��ʱ������ʼ��	  
		LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	  serial_init();
		while(1)
		{
			
			LED0=0;
			LED1=1;
			delay_ms(300);	 //��ʱ300ms
			LED0=1;
			LED1=0;
			delay_ms(300);	//��ʱ300ms
		}
 }




