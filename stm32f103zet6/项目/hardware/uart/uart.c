#include"uart.h"

void myuart_init()
{
	GPIO_InitTypeDef GPIO_InitT;
	USART_InitTypeDef USART_InitTypeD;
	NVIC_InitTypeDef NVIC_InitTypeD;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_USART1,ENABLE);//时钟使能开启
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3|RCC_APB1Periph_UART4,ENABLE);
	
	//io初始化
	GPIO_InitT.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽
	GPIO_InitT.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_2;
	GPIO_InitT.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitT);
	
	GPIO_InitT.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init(GPIOB,&GPIO_InitT);
	
	GPIO_InitT.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init(GPIOC,&GPIO_InitT);
	
	GPIO_InitT.GPIO_Mode=GPIO_Mode_IPU;//输入上拉
	GPIO_InitT.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitT);
	
	GPIO_InitT.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIO_InitT);
	
	GPIO_InitT.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init(GPIOC,&GPIO_InitT);
	
	
	//串口初始化
	USART_InitTypeD.USART_BaudRate = 115200;
	USART_InitTypeD.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitTypeD.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitTypeD.USART_Parity = USART_Parity_No;
	USART_InitTypeD.USART_StopBits = USART_StopBits_1;
	USART_InitTypeD.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitTypeD);
	USART_Init(USART2,&USART_InitTypeD);
	USART_Init(USART3,&USART_InitTypeD);
	USART_Init(UART4,&USART_InitTypeD);
	
	//
	USART_Cmd(USART1,ENABLE);
	USART_Cmd(USART2,ENABLE);
	USART_Cmd(USART3,ENABLE);
	USART_Cmd(UART4,ENABLE);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
	
	/*优先级配置*/
	NVIC_InitTypeD.NVIC_IRQChannel=USART1_IRQn;//uart4还没写中断
	NVIC_InitTypeD.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitTypeD.NVIC_IRQChannelPreemptionPriority=2; //抢先优先级
	NVIC_InitTypeD.NVIC_IRQChannelSubPriority=2;  //响应优先级
	NVIC_Init(&NVIC_InitTypeD);
	
	NVIC_InitTypeD.NVIC_IRQChannel=USART2_IRQn;//uart4还没写中断
	NVIC_InitTypeD.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitTypeD.NVIC_IRQChannelPreemptionPriority=2; //抢先优先级
	NVIC_InitTypeD.NVIC_IRQChannelSubPriority=0;  //响应优先级
	NVIC_Init(&NVIC_InitTypeD);
	
	NVIC_InitTypeD.NVIC_IRQChannel=USART3_IRQn;//uart4还没写中断
	NVIC_InitTypeD.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitTypeD.NVIC_IRQChannelPreemptionPriority=2; //抢先优先级
	NVIC_InitTypeD.NVIC_IRQChannelSubPriority=1;  //响应优先级
	NVIC_Init(&NVIC_InitTypeD);
	
	NVIC_InitTypeD.NVIC_IRQChannel=UART4_IRQn;//uart4还没写中断
	NVIC_InitTypeD.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitTypeD.NVIC_IRQChannelPreemptionPriority=2; //抢先优先级
	NVIC_InitTypeD.NVIC_IRQChannelSubPriority=0;  //响应优先级
	NVIC_Init(&NVIC_InitTypeD);
}

void USART1_IRQHandler()
{
	u8 res;
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		res=USART_ReceiveData(USART1);
		USART_SendData(USART1,res);
	}
}

void USART2_IRQHandler()
{
	u8 res;
	if(USART_GetITStatus(USART2,USART_IT_RXNE))
	{
		res=USART_ReceiveData(USART2);
		USART_SendData(USART2,res);
	}
}

void USART3_IRQHandler()
{
	u8 res;
	if(USART_GetITStatus(USART3,USART_IT_RXNE))
	{
		res=USART_ReceiveData(USART3);
		USART_SendData(USART3,res);
	}
}

void UART4_IRQHandler()
{
	u8 res;
	if(USART_GetITStatus(UART4,USART_IT_RXNE))
	{
		res=USART_ReceiveData(UART4);
		USART_SendData(UART4,res);
	}
}