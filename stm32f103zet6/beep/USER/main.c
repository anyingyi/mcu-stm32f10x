#include "stm32f10x.h"
#include"delay.h"



 int main(void)
 {	
		GPIO_InitTypeDef  GPIO_InitStructure;
		 
		delay_init();
		 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    //ʹ��PB,PE�˿�ʱ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;			    //LED0-->PB.5 �˿�����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
		GPIO_Init(GPIOB, &GPIO_InitStructure);			     //��ʼ��GPIOB.5
		GPIO_SetBits(GPIOB,GPIO_Pin_8);					//PB.5 �����
  
		while(1)
		{
			PBout(8)=1;	
			delay_ms(300);
			PBout(8)=0;
			
			delay_ms(300);
		}
 }
