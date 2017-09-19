#include "sensor.h"
#include "stm32f10x_it.h" 
#include "delay.h"
u16 Frame_Distance=1;//数据帧最小间隔（ms),超过此时间则认为是下一帧
void TIM2_init()
{
	     TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        NVIC_InitTypeDef NVIC_InitStructure;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //TIM7时钟使能 

        //TIM7初始化设置
        TIM_TimeBaseStructure.TIM_Period = Frame_Distance*10; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
        TIM_TimeBaseStructure.TIM_Prescaler =7199; //设置用来作为TIMx时钟频率除数的预分频值 设置计数频率为10kHz
        TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
        TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

        TIM_ITConfig( TIM2, TIM_IT_Update, ENABLE );//TIM7 允许更新中断

        //TIM7中断分组配置
        NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn;  //TIM7中断
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
        NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
}

//PM2.5所用端口和串口的配置
void Sensor_PM_GPIOInit()
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); //USART2时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);  //GPIOA使能
	
//GPIOA配置	
	//USART2   TXD
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;      //GPIO口配置
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;               //USART2复用PA2
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;        //端口复用输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//USART    RXD
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;      //GPIO口配置
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;               //USART2复用PA3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //浮空输入
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
//中断配置	
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
//USART配置
	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//一个字节
	USART_Init(USART2,&USART_InitStructure);//初始化串口
		
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//使能接收中断
	USART_Cmd(USART2,ENABLE);//使能串口2
	TIM2_init();
}



	


//PM2.5传感器
//发送命令

void Sensor_PM_SendCommand()
{
	int i;
	static const u8 Command_PM[7]={0x42,0x4d,0xe2,0x00,0x00,0x01,0x71}; //定义命令数组并初始化
	 for(i=0;i<7;i++)
	   {
	    USART_SendData(USART2,Command_PM[i]);	//给传感器发送命 
	   // delay_ms(1);
		 }
		 
;
 
}

//u16 USART2_BUF_Receive[32];//存储串口2的缓冲数据// Device header
//u16  RX_CNT=0;//接收计数器
////TIM7定时中断服务函数
//void TIM7_IRQHandler(void) //定时器7初始化---功能：判断从机返回的数据是否接受完成
//{ 
//   if(TIM_GetITStatus(TIM7,TIM_IT_Update)!=RESET)
//        {
//             
//           TIM_ClearITPendingBit(TIM7,TIM_IT_Update);//清除中断标志
//           TIM_Cmd(TIM7,DISABLE);//停止定时器
//        }	
//}



////2毫秒接收一次数据
//u16 PM_DATA;
//void USART2_IRQHandler(void)    //中断服务函数
//{ 
//	  u8 res;
//    u8 err;
//	  u8 j;
//	  u16 check_PM_sum;
//   if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
//        {     
//					
//                if(USART_GetFlagStatus(USART2,USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE)) err=1;//检测到噪音、帧错误或校验错误
//                else err=0;
//                res=USART_ReceiveData(USART2); //读接收到的字节，同时相关标志自动清除
//                
//                if((RX_CNT<32)&&(err==0))
//                {
//                        USART2_BUF_Receive[RX_CNT]=res;
//                        RX_CNT++;
//                        
//                        TIM_ClearITPendingBit(TIM7,TIM_IT_Update);//清除定时器溢出中断
//                        TIM_SetCounter(TIM7,0);//当接收到一个新的字节，将定时器7复位为0，重新计时
//                        TIM_Cmd(TIM7,ENABLE);//开始计时
//                }	
//			
//                if( RX_CNT == 32 ) 
//			         {
//					      if((USART2_BUF_Receive[0]==0x42)&&(USART2_BUF_Receive[1]==0x4d))
//				   	      {						
//							    for(j=0;j<30;j++)
//							      {						
//									    check_PM_sum +=USART2_BUF_Receive[j]; 
//							       }					
//							         if(check_PM_sum==((USART2_BUF_Receive[30]<<8)+USART2_BUF_Receive[31]) ) //判断校验位。30,31为校验和
//									    PM_DATA=((USART2_BUF_Receive[6]<<8)+USART2_BUF_Receive[7]);   //PM2.5的值
//				           }
//		   
//					
//			         }	
//		   }
//				
//}

