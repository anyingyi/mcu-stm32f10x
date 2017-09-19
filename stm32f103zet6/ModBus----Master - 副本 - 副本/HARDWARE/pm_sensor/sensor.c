#include "sensor.h"
#include "stm32f10x_it.h" 
#include "delay.h"
u16 Frame_Distance=1;//����֡��С�����ms),������ʱ������Ϊ����һ֡
void TIM2_init()
{
	     TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        NVIC_InitTypeDef NVIC_InitStructure;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //TIM7ʱ��ʹ�� 

        //TIM7��ʼ������
        TIM_TimeBaseStructure.TIM_Period = Frame_Distance*10; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
        TIM_TimeBaseStructure.TIM_Prescaler =7199; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ ���ü���Ƶ��Ϊ10kHz
        TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
        TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

        TIM_ITConfig( TIM2, TIM_IT_Update, ENABLE );//TIM7 ��������ж�

        //TIM7�жϷ�������
        NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn;  //TIM7�ж�
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
        NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
}

//PM2.5���ö˿ںʹ��ڵ�����
void Sensor_PM_GPIOInit()
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); //USART2ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);  //GPIOAʹ��
	
//GPIOA����	
	//USART2   TXD
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;      //GPIO������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;               //USART2����PA2
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;        //�˿ڸ������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//USART    RXD
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;      //GPIO������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;               //USART2����PA3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //��������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
//�ж�����	
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
//USART����
	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//һ���ֽ�
	USART_Init(USART2,&USART_InitStructure);//��ʼ������
		
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//ʹ�ܽ����ж�
	USART_Cmd(USART2,ENABLE);//ʹ�ܴ���2
	TIM2_init();
}



	


//PM2.5������
//��������

void Sensor_PM_SendCommand()
{
	int i;
	static const u8 Command_PM[7]={0x42,0x4d,0xe2,0x00,0x00,0x01,0x71}; //�����������鲢��ʼ��
	 for(i=0;i<7;i++)
	   {
	    USART_SendData(USART2,Command_PM[i]);	//�������������� 
	   // delay_ms(1);
		 }
		 
;
 
}

//u16 USART2_BUF_Receive[32];//�洢����2�Ļ�������// Device header
//u16  RX_CNT=0;//���ռ�����
////TIM7��ʱ�жϷ�����
//void TIM7_IRQHandler(void) //��ʱ��7��ʼ��---���ܣ��жϴӻ����ص������Ƿ�������
//{ 
//   if(TIM_GetITStatus(TIM7,TIM_IT_Update)!=RESET)
//        {
//             
//           TIM_ClearITPendingBit(TIM7,TIM_IT_Update);//����жϱ�־
//           TIM_Cmd(TIM7,DISABLE);//ֹͣ��ʱ��
//        }	
//}



////2�������һ������
//u16 PM_DATA;
//void USART2_IRQHandler(void)    //�жϷ�����
//{ 
//	  u8 res;
//    u8 err;
//	  u8 j;
//	  u16 check_PM_sum;
//   if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
//        {     
//					
//                if(USART_GetFlagStatus(USART2,USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE)) err=1;//��⵽������֡�����У�����
//                else err=0;
//                res=USART_ReceiveData(USART2); //�����յ����ֽڣ�ͬʱ��ر�־�Զ����
//                
//                if((RX_CNT<32)&&(err==0))
//                {
//                        USART2_BUF_Receive[RX_CNT]=res;
//                        RX_CNT++;
//                        
//                        TIM_ClearITPendingBit(TIM7,TIM_IT_Update);//�����ʱ������ж�
//                        TIM_SetCounter(TIM7,0);//�����յ�һ���µ��ֽڣ�����ʱ��7��λΪ0�����¼�ʱ
//                        TIM_Cmd(TIM7,ENABLE);//��ʼ��ʱ
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
//							         if(check_PM_sum==((USART2_BUF_Receive[30]<<8)+USART2_BUF_Receive[31]) ) //�ж�У��λ��30,31ΪУ���
//									    PM_DATA=((USART2_BUF_Receive[6]<<8)+USART2_BUF_Receive[7]);   //PM2.5��ֵ
//				           }
//		   
//					
//			         }	
//		   }
//				
//}

