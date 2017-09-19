#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
#include "timer.h"
#include "master.h"
#include "slave.h"

extern u8 RS485_RX_BUFF[2048];//���ջ�����2048�ֽ�

int main(void)
{

		SystemInit();
		delay_init();	    	 //��ʱ������ʼ��
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		key_init();
	  led_init();
		
	
	  
	  //master
		if(ADR_)
		{
				RS485_Init();
				while(1){
					if(!ADR_)
						SoftReset();
					printf(RS485_RX_BUFF);
					delay_ms(500);
				}
		}
		
		//slave
		if(!ADR_)
		{
				RS485_Init1();
				while(1){
					if(ADR_)
						SoftReset();
					RS485_Service();
				}				
		}			
}

