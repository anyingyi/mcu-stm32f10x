#include "protocol.h"
#include "mcheck.h" 
#include "mport.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//MODBUS �ӿڲ���������	  
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/8/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2017-2027
//All rights reserved	 
////////////////////////////////////////////////////////////////////////////////// 
 

//����2�жϷ�����
//�����ͨ��������ʽ���ܣ��밴Ҫ��ʵ�����ݴ洢
//��һ֡���������Ĵ洢��m_ctrl_dev.rxbuf����,����֡���ݳ��ȴ洢��m_ctrl_dev.rxlen����
void USART2_IRQHandler(void)
{
	u8 res;	
	if(USART2->SR&(1<<5))			//���յ�����
	{	 
		res=USART2->DR; 
		if(m_ctrl_dev.frameok==0)	//����δ���
		{ 
			m_ctrl_dev.rxbuf[m_ctrl_dev.rxlen]=res;
			m_ctrl_dev.rxlen++;
			if(m_ctrl_dev.rxlen>(M_MAX_FRAME_LENGTH-1))m_ctrl_dev.rxlen=0;	//�������ݴ���,���¿�ʼ����	  
  		}  		 									     
	}else if(USART2->SR&(1<<4))		//�����ж�
	{
		res=USART2->DR; 			//��DR�����IDLEλ
		if(m_ctrl_dev.rxlen>=M_MIN_FRAME_LENGTH)m_ctrl_dev.frameok=1;		//������һ֡���ݽ���
		else m_ctrl_dev.rxlen=0;	//��������ݣ�ֱ�Ӷ��������¿�ʼ��һ�ν���
	}
} 


//��ʼ���ӿ�
//��������ʹ�ô���1���ø����Լ�����Ҫ�޸�
//����2���������ÿ����жϣ���֡�����жϣ���������ӿڣ�û�����ƿ���״̬���绰
//�����Լ�ʵ��֡���ݽ�ȡ�������ó�ʱ����
//pclk1��PCLK1ʱ��;
//bound��������
void mp_init(u32 pclk1,u32 bound)
{  	   
	RCC->APB2ENR|=1<<5;   	//ʹ��PORTD��ʱ��  
 	GPIOD->CRL&=0X0FFFFFFF;	//IO״̬����
	GPIOD->CRL|=0X30000000;	//IO״̬����	 
	
	RCC->APB2ENR|=1<<2;   	//ʹ��PORTA��ʱ��  
	GPIOA->CRL&=0XFFFF00FF;	//IO״̬����
	GPIOA->CRL|=0X00008B00;	//IO״̬����	  

	RCC->APB1ENR|=1<<17;  	//ʹ�ܴ���2ʱ�� 		  
	RCC->APB1RSTR|=1<<17;   //��λ����2
	RCC->APB1RSTR&=~(1<<17);//ֹͣ��λ	   	   
	//����������
 	USART2->BRR=(pclk1*1000000)/bound;;// ����������	 
	USART2->CR1|=0X200C;  	//1λֹͣ,��У��λ.
	USART2->CR1|=1<<4;	  	//�����������߿����ж�.  
	USART2->CR1|=1<<5;    	//���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,3,USART2_IRQn,2);//��2��������ȼ�  
	
	RS485_TX_EN=0;			//Ĭ��Ϊ����ģʽ	 
}


//����ָ�����ȵ����� 
//buf��Ҫ���͵����ݻ������׵�ַ
//len��Ҫ���͵����ݳ���
void mp_send_data(u8* buf, u16 len)		
{
	u16 i=0;
	RS485_TX_EN=1;			//���뷢��״̬
	for(i=0;i<len;i++)
	{
		while((USART2->SR&0X40)==0);	//�ȴ���һ�δ������ݷ������  
		USART2->DR=buf[i];      		//дDR,����1����������
	}		
	while((USART2->SR&0X40)==0);		//�ȴ����һ�����ݷ������  
	RS485_TX_EN=0;						//�������״̬

}
















