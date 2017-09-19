#include "LED.h"


//��ʼ��UART4
void CRS485_Init1(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;
	
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
        
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//PA2��TX�������������
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
        GPIO_Init(GPIOC,&GPIO_InitStructure);
     //   GPIO_SetBits(GPIOA,GPIO_Pin_9);//Ĭ�ϸߵ�ƽ
        
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//PA3��RX����������
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //�޸�ԭGPIO_Mode_IPU������������->GPIO_Mode_IN_FLOATING(��������)/////////////////////////////////////////////
        GPIO_Init(GPIOC,&GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;//�޸�PG9��RE/DE��ͨ���������->PD7��RE/DE��ͨ���������//////////////////////////////////////////////////////////////////////
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
        GPIO_Init(GPIOD,&GPIO_InitStructure);
        GPIO_ResetBits(GPIOD,GPIO_Pin_7);//Ĭ�Ͻ���״̬
        
        USART_DeInit(UART4);//��λ����2
        USART_InitStructure.USART_BaudRate=9600;
        USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
        USART_InitStructure.USART_WordLength=USART_WordLength_8b;
        USART_InitStructure.USART_StopBits=USART_StopBits_1;
        USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//�շ�ģʽ
        switch(0)
        {
                case 0:USART_InitStructure.USART_Parity=USART_Parity_No;break;//��У��
                case 1:USART_InitStructure.USART_Parity=USART_Parity_Odd;break;//��У��
                case 2:USART_InitStructure.USART_Parity=USART_Parity_Even;break;//żУ��
        }
        USART_Init(UART4,&USART_InitStructure);
        
        USART_ClearITPendingBit(UART4,USART_IT_RXNE);
        USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);//ʹ�ܴ���2�����ж�
        
        NVIC_InitStructure.NVIC_IRQChannel=UART4_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
        NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        
        USART_Cmd(UART4,ENABLE);//ʹ�ܴ���2
        
}



