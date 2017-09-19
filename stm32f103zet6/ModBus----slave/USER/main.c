#include "sys.h"	
#include "delay.h"	
#include "modbus.h"
#include "led.h"
#include "key.h"
#include "iic.h"
#include "th_sensor.h"

 
/************************************************
 ALIENTEKս��STM32������ʵ��2
 ������ʵ��
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

int main(void)
{
		delay_init();	    	 //��ʱ������ʼ��
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

