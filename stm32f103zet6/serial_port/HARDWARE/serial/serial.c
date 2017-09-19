#include"serial.h"

//
void serial_init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 USART_InitTypeDef USART_InitTypeD;
	 NVIC_InitTypeDef NVIC_InitTypeD;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE);	 //ʹ��PB,PE�˿�ʱ��

	 
	//gpio��ʼ��
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //LED0-->PB.5 �˿�����
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //fuyong�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				 //LED0-->PB.5 �˿�����
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //fuyong�������
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��
	 
	 //���ڳ�ʼ��
	 USART_InitTypeD.USART_BaudRate=9600;
	 USART_InitTypeD.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	 USART_InitTypeD.USART_Parity=USART_Parity_No;
	 USART_InitTypeD.USART_StopBits=USART_StopBits_1;
	 USART_InitTypeD.USART_WordLength=USART_WordLength_8b;
	 USART_InitTypeD.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
   USART_Init(USART1,&USART_InitTypeD);
	
	 //
	 NVIC_InitTypeD.NVIC_IRQChannel=USART1_IRQn;
	 NVIC_InitTypeD.NVIC_IRQChannelCmd=ENABLE;
	 NVIC_InitTypeD.NVIC_IRQChannelPreemptionPriority=2;
	 NVIC_InitTypeD.NVIC_IRQChannelSubPriority=2;
	 NVIC_Init(&NVIC_InitTypeD);
	 
	 USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	 
	 USART_Cmd(USART1,ENABLE);
}

void USART1_IRQHandler(void)
{
	 u8 res;
	 if(USART_GetITStatus(USART1,USART_IT_RXNE))
	 {
		 res=USART_ReceiveData(USART1);
		 USART_SendData(USART1,res);
	 }
}


