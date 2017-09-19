//	�����������    //

#include "key.h"
#if ADR_0
#include"master.h"
#include"usart.h"
#include"delay.h"
#include "led.h"




u32 RS485_Baudrate=9600;//ͨѶ������
u8 RS485_Parity=0;//0��У�飻1��У�飻2żУ��
u16 RS485_Frame_Distance=4;//����֡��С�����ms),������ʱ������Ϊ����һ֡

u8 RS485_RX_BUFF[2048];//���ջ�����2048�ֽ�
u16 RS485_RX_CNT=0;//���ռ�����
u8 RS485_RxFlag=0;//����һ֡�������

u8 RS485_TX_BUFF[2048];//���ͻ�����
u16 RS485_TX_CNT=0;//���ͼ�����
u8 RS485_TxFlag=0;//����һ֡�������

/////////////////////////////////////////////////////////////////////////////////////
//����������
u8   SlaverAddr=0;    //�ӻ���ַ
u8   Fuction=03;      // ������
u16  StartAddr=1;    //��ʼ��ַ
u16  ValueOrLenth=1;  //����or����
//////////////////////////////////////////////////////////////////////////////////////////

u8 TX_RX_SET=0; //���ͣ����������л��� 0 ����ģʽ 1����ģʽ
u8 ComErr=8; //0����ͨѶ����
             //1����CRC����
			// 2����������� 
			
			


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Master�Ĵ����͵�Ƭ���Ĵ�����ӳ���ϵ
u16   Master_InputIO[100];  //���뿪�����Ĵ���(����ʹ�õ���λ������)   ע�⣺ ���ﴢ��ӻ����ص����ݡ�    ������������ֻ����0��1 ���� Master_InputIO[5]=0��Master_InputIO[8]=1��
u16   Master_OutputIO[100]; //����������Ĵ���(����ʹ�õ���λ������)    ������ 05 15

u16  Master_ReadReg[1000]; //ֻ���Ĵ���----�洢�ӻ����ص�����          ������ 03
u16  Master_WriteReg[1000];//д�Ĵ���-------���Ĵ����е������͸��ӻ�   ������ 06 16



u32 testData1=1201,testData2=1002,testData3=2303,testData4=8204;



void Modbus_RegMap(void)
{
	Master_WriteReg[0]=1;
	Master_WriteReg[1]=8;
	Master_WriteReg[2]=9;
	Master_WriteReg[3]=235;
	Master_WriteReg[4]=8690;
	Master_WriteReg[5]=23578;
	Master_WriteReg[6]=125;
	
	Master_OutputIO[20]=1;
	Master_OutputIO[21]=0;
	Master_OutputIO[22]=1;
	Master_OutputIO[23]=1;
	Master_OutputIO[24]=0;
	Master_OutputIO[25]=0;
	Master_OutputIO[26]=1;
	Master_OutputIO[27]=1;
	
	Master_OutputIO[28]=1;
	Master_OutputIO[29]=0;		
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//CRCУ�� �Լ�������ӵ�

const u8 auchCRCHi[] = { 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40} ; 


const u8 auchCRCLo[] = { 
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,0x43, 0x83, 0x41, 0x81, 0x80, 0x40} ;


u16 CRC_Compute(u8 *puchMsg, u16 usDataLen) 
{ 
	u8 uchCRCHi = 0xFF ; 
	u8 uchCRCLo = 0xFF ; 
	u32 uIndex ; 
	while (usDataLen--) 
	{ 
		uIndex = uchCRCHi ^ *puchMsg++ ; 
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
		uchCRCLo = auchCRCLo[uIndex] ; 
	} 
	return ((uchCRCHi<< 8)  | (uchCRCLo)) ; 
}//uint16 crc16(uint8 *puchMsg, uint16 usDataLen)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//��ʼ��USART3
void RS485_Init(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;
	
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD,ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
        
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//PA2��TX�������������
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
        GPIO_Init(GPIOB,&GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//PA3��RX����������
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //�޸�ԭGPIO_Mode_IPU������������->GPIO_Mode_IN_FLOATING(��������)/////////////////////////////////////////////
        GPIO_Init(GPIOB,&GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;//�޸�PG9��RE/DE��ͨ���������->PD7��RE/DE��ͨ���������//////////////////////////////////////////////////////////////////////
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
        GPIO_Init(GPIOD,&GPIO_InitStructure);
        GPIO_ResetBits(GPIOD,GPIO_Pin_7);//Ĭ�Ͻ���״̬
        
        USART_DeInit(USART3);//��λ����2
        USART_InitStructure.USART_BaudRate=RS485_Baudrate;
        USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
        USART_InitStructure.USART_WordLength=USART_WordLength_8b;
        USART_InitStructure.USART_StopBits=USART_StopBits_1;
        USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//�շ�ģʽ
        switch(RS485_Parity)
        {
                case 0:USART_InitStructure.USART_Parity=USART_Parity_No;break;//��У��
                case 1:USART_InitStructure.USART_Parity=USART_Parity_Odd;break;//��У��
                case 2:USART_InitStructure.USART_Parity=USART_Parity_Even;break;//żУ��
        }
        USART_Init(USART3,&USART_InitStructure);
        
        USART_ClearITPendingBit(USART3,USART_IT_RXNE);
        USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//ʹ�ܴ���2�����ж�
        
        NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
        NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        
        USART_Cmd(USART3,ENABLE);//ʹ�ܴ���2
        RS485_TX_EN=1;//Ĭ��Ϊ����ģʽ
        
        Timer7_Init();//��ʱ��7��ʼ�������ڼ��ӿ���ʱ��
				Modbus_RegMap();
				uart_init(115200);
			  Timer4_enable(2500);	//250ms	
				
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ʱ��4��ʼ��
//��ʱ1s����ͨѶ����2����ǰ500ms���з��͹��ܣ���500ms����ӻ����ص�����
void Timer4_enable(u16 arr)	   	//TIM4ʹ��
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7199; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim,72MHZ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig( TIM4,TIM_IT_Update|TIM_IT_Trigger,ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����							 
}


///////////////////////////////////////////////////////////////////////////////////////////////
//��ʱ��7��ʼ��---���ܣ��жϴӻ����ص������Ƿ�������

void Timer7_Init(void)
{
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        NVIC_InitTypeDef NVIC_InitStructure;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //TIM7ʱ��ʹ�� 

        //TIM7��ʼ������
        TIM_TimeBaseStructure.TIM_Period = RS485_Frame_Distance*10; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
        TIM_TimeBaseStructure.TIM_Prescaler =7199; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ ���ü���Ƶ��Ϊ10kHz
        TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
        TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

        TIM_ITConfig( TIM7, TIM_IT_Update, ENABLE );//TIM7 ��������ж�

        //TIM7�жϷ�������
        NVIC_InitStructure.NVIC_IRQChannel =TIM7_IRQn;  //TIM7�ж�
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
        NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���                                                                  
}




extern u8 RS485_RX_BUFF1[2048];//���ջ�����2048�ֽ�
extern u16 RS485_RX_CNT1;//���ռ�����
extern u8 PM_BUFF[16];


/////////////////////////////////////////////////////////////////////////////////////
void USART3_IRQHandler(void)//����3�жϷ������
{
	   
	 u8 res;
	 u8 err;
	 if(ADR_)
		{
        if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
        {
                if(USART_GetFlagStatus(USART3,USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE)) err=1;//��⵽������֡�����У�����
                else err=0;
                res=USART_ReceiveData(USART3); //�����յ����ֽڣ�ͬʱ��ر�־�Զ����
                
                if((RS485_RX_CNT<2047)&&(err==0))
                {
                        RS485_RX_BUFF[RS485_RX_CNT]=res;
                        RS485_RX_CNT++;
                        TIM_ClearITPendingBit(TIM7,TIM_IT_Update);//�����ʱ������ж�
                        TIM_SetCounter(TIM7,0);//�����յ�һ���µ��ֽڣ�����ʱ��7��λΪ0�����¼�ʱ���൱��ι����
                        TIM_Cmd(TIM7,ENABLE);//��ʼ��ʱ
                }
        }
		}	
			
	 else
	 {
        if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
        {
                if(USART_GetFlagStatus(USART3,USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE)) err=1;//��⵽������֡�����У�����
                else err=0;
			          LED_COM=0;
                res=USART_ReceiveData(USART3); //�����յ����ֽڣ�ͬʱ��ر�־�Զ����
                
                if((RS485_RX_CNT1<2047)&&(err==0))
                {
                        RS485_RX_BUFF1[RS485_RX_CNT1]=res;
                        RS485_RX_CNT1++;
                        
                        TIM_ClearITPendingBit(TIM6,TIM_IT_Update);//�����ʱ������ж�
                        TIM_SetCounter(TIM6,0);//�����յ�һ���µ��ֽڣ�����ʱ��7��λΪ0�����¼�ʱ���൱��ι����
                        TIM_Cmd(TIM6,ENABLE);//��ʼ��ʱ
                }
        }
		}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////
void TIM4_IRQHandler(void)   //TIM4�ж�
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ ;
		modbus_rtu();	
	}
}

///////////////////////////////////////////////////////////////////////////////////////

//�ö�ʱ��7�жϽ��տ���ʱ�䣬������ʱ�����ָ��ʱ�䣬��Ϊһ֡����
//��ʱ��7�жϷ������         
void TIM7_IRQHandler(void)
{                                                                   
        if(TIM_GetITStatus(TIM7,TIM_IT_Update)!=RESET)
        {
                TIM_ClearITPendingBit(TIM7,TIM_IT_Update);//����жϱ�־
                TIM_Cmd(TIM7,DISABLE);//ֹͣ��ʱ��
                RS485_TX_EN=1;//ֹͣ���գ��л�Ϊ����״̬
                RS485_RxFlag=1;//��λ֡�������
        }
}

//////////////////////////////////////////////////////////////////////////////
//����n���ֽ�����
//buff:�������׵�ַ
//len�����͵��ֽ���
void RS485_SendData(u8 *buff,u8 len)
{ 
        RS485_TX_EN=1;//�л�Ϊ����ģʽ
        while(len--)
        {
                while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);//�ȴ�������Ϊ��
                USART_SendData(USART3,*(buff++));
        }
        while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);//�ȴ��������
				TX_RX_SET=1; //����������ɣ���ʱ��T4������յ�������
				RS485_TX_EN=0;
}


/////////////////////////////////////////////////////////////////////////////////////
void modbus_rtu(void)
{	
	static u8 i=0;
	static u8 j=0;
	switch(i)
	{
		case 0:
				RS485_TX_Service();
		    LED_COM=!LED_COM;
		    LED_TH=!LED_TH;
				if(TX_RX_SET){ 
					i=1;
					if(SlaverAddr<values+1)
					SlaverAddr++;
					if(SlaverAddr==values+1)
					SlaverAddr=0;
				}
				break;
		case 1:
				RS485_RX_Service();
		    if(!TX_RX_SET) 
					i=0;
				else
				{					
					i++;
				}
		    break;
		case 2:
			i=0;
				break;			
	}
	
}
	

//Modbus������01������� ///////////////////////////////////////////////////////////��������֤OK
//�����������
void Master_01_Slove( u8 board_adr,u16 start_address, u16 lenth )
{
	u16 calCRC;
	RS485_TX_BUFF[0] = board_adr;
    RS485_TX_BUFF[1] = READ_COIL;    //modbus ָ����01
    RS485_TX_BUFF[2] = HI(start_address);  
    RS485_TX_BUFF[3] = LOW(start_address); 
    RS485_TX_BUFF[4] = HI(lenth);
    RS485_TX_BUFF[5] = LOW(lenth);
    calCRC=CRC_Compute(RS485_TX_BUFF,6);
    RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
    RS485_TX_BUFF[7]=(calCRC)&0xFF;
	RS485_SendData(RS485_TX_BUFF,8);	
}

//Modbus������02�������/////////////////////////////////////////////////////��������֤OK -----����������PE4 PE3 PE2 PA0 ��ʼ�������ſ���OK    KEY_Init();
//�����뿪����
void Master_02_Slove(u8 board_adr,u16 start_address, u16 lenth)
{	
	u16 calCRC;
	RS485_TX_BUFF[0] = board_adr;
    RS485_TX_BUFF[1] = READ_DI;    //modbus ָ����02
    RS485_TX_BUFF[2] = HI(start_address);  
    RS485_TX_BUFF[3] = LOW(start_address); 
    RS485_TX_BUFF[4] = HI(lenth);
    RS485_TX_BUFF[5] = LOW(lenth);
    calCRC=CRC_Compute(RS485_TX_BUFF,6);
    RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
    RS485_TX_BUFF[7]=(calCRC)&0xFF;
	RS485_SendData(RS485_TX_BUFF,8);
}

//Modbus������03�������///////////////////////////////////////////////////////////////////////////////////////
//�����ּĴ���
void Master_03_Slove( u8 board_adr,u16 start_address,u16 lenth )
{ 
	  u16 calCRC;
    RS485_TX_BUFF[0] = board_adr;
    RS485_TX_BUFF[1] = READ_HLD_REG;    //modbus ָ����03
    RS485_TX_BUFF[2] = HI(start_address);  
    RS485_TX_BUFF[3] = LOW(start_address); 
    RS485_TX_BUFF[4] = HI(lenth);
    RS485_TX_BUFF[5] = LOW(lenth);
    calCRC=CRC_Compute(RS485_TX_BUFF,6);
    RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
    RS485_TX_BUFF[7]=(calCRC)&0xFF;
	  RS485_SendData(RS485_TX_BUFF,8);
}
//Modbus������04�������/////////////////////////////////////////////////////////////////////////////////////// δʹ��
//������Ĵ���
void Master_04_Slove( u8 board_adr,u16 start_address,u16 lenth )
{ 
	u16 calCRC;
    RS485_TX_BUFF[0] = board_adr;
    RS485_TX_BUFF[1] = READ_AI;    //modbus ָ����04
    RS485_TX_BUFF[2] = HI(start_address);  
    RS485_TX_BUFF[3] = LOW(start_address); 
    RS485_TX_BUFF[4] = HI(lenth);
    RS485_TX_BUFF[5] = LOW(lenth);
    calCRC=CRC_Compute(RS485_TX_BUFF,6);
    RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
    RS485_TX_BUFF[7]=(calCRC)&0xFF;
	RS485_SendData(RS485_TX_BUFF,8);
}

//Modbus������05�������   ///////////////////////////////////////////////////////��������֤OK
//д������������� ���ֻ����0XFF00->1 ON; 0X0000->0 OFF;
void Master_05_Slove(u8 board_adr,u16 start_address, u16 value)
{
	
	u16 i;
	u16 calCRC;
	if(value==1)
	{
	if(Master_OutputIO[start_address]) i=0xff00;
	else  i=0x0000;
	RS485_TX_BUFF[0] = board_adr;
    RS485_TX_BUFF[1] = SET_COIL;   //modbus ָ����05
    RS485_TX_BUFF[2] = HI(start_address);  
    RS485_TX_BUFF[3] = LOW(start_address); 
    RS485_TX_BUFF[4] = HI(i);
    RS485_TX_BUFF[5] = LOW(i);
    calCRC=CRC_Compute(RS485_TX_BUFF,6);
	RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
    RS485_TX_BUFF[7]=(calCRC)&0xFF;
	RS485_SendData(RS485_TX_BUFF,8);
	}

}


//Modbus������06�������   //////////////////////////////////////////////////////////////////////////////////
//д�������ּĴ���
void Master_06_Slove(u8 board_adr,u16 start_address, u16 value)
{
	u16 calCRC;
	//if(value==1)
	{
			  RS485_TX_BUFF[0] = board_adr;
				RS485_TX_BUFF[1] = SET_HLD_REG;   //modbus ָ����06
				RS485_TX_BUFF[2] = HI(start_address);  
				RS485_TX_BUFF[3] = LOW(start_address); 
				RS485_TX_BUFF[4] = HI(Master_WriteReg[start_address]);
				RS485_TX_BUFF[5] = LOW(Master_WriteReg[start_address]);
				calCRC=CRC_Compute(RS485_TX_BUFF,6);
			  RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
				RS485_TX_BUFF[7]=(calCRC)&0xFF;
			  RS485_SendData(RS485_TX_BUFF,8);
	}		
	
}
//Modbus������15�������   //////////////////////////////////////////////////////��������֤OK
//д������������
void Master_15_Slove(u8 board_adr,u16 start_address, u16 lenth) 
{	
		u16 calCRC;
	    u16 ByteNum;
        u16 i;
        if((start_address+lenth)<100)//�Ĵ�����ַ+�����ڷ�Χ��
        {

                ByteNum=lenth/8;//�ֽ���
                if(lenth%8) ByteNum+=1;//���λ���������������ֽ���+1
                for(i=0;i<lenth;i++)
                {
                         if(i%8==0) RS485_TX_BUFF[7+i/8]=0x00;
						 RS485_TX_BUFF[7+i/8]=RS485_TX_BUFF[7+i/8]>>1;
                         if(Master_OutputIO[start_address+i]) RS485_TX_BUFF[7+i/8]=RS485_TX_BUFF[7+i/8]|0x80;//��λ�ȷ���
                         if(i==lenth-1)//���͵����һ��λ��
                         {
                                if(lenth%8) RS485_TX_BUFF[7+i/8]>>=8-(lenth%8);//������һ���ֽڻ�����������ʣ��MSB���0
                         }
                }
				
			}	      
	RS485_TX_BUFF[0] = board_adr;
    RS485_TX_BUFF[1] = WRITE_COIL;   //modbus ָ����15
    RS485_TX_BUFF[2] = HI(start_address);  
    RS485_TX_BUFF[3] = LOW(start_address); 
    RS485_TX_BUFF[4] = HI(lenth);
    RS485_TX_BUFF[5] = LOW(lenth);	
	RS485_TX_BUFF[6] = ByteNum;
    calCRC=CRC_Compute(RS485_TX_BUFF,7+ByteNum);
	RS485_TX_BUFF[7+ByteNum]=(calCRC>>8)&0xFF; 
    RS485_TX_BUFF[8+ByteNum]=(calCRC)&0xFF;
	RS485_SendData(RS485_TX_BUFF,9+ByteNum);	


		

}
//Modbus������16������� /////////////////////////////////////////////////////////////////////////////////////////////////����֤����OK
//д������ּĴ���
void Master_16_Slove(u8 board_adr,u16 start_address, u16 lenth) ///
{
		u16 calCRC;
	    u8 i;
        if((start_address+lenth)<1000)//�Ĵ�����ַ+�����ڷ�Χ��
        {
                for(i=0;i<lenth;i++)
                {
                      RS485_TX_BUFF[7+i*2]= Master_WriteReg[start_address+i]>>8;  //���ֽ���ǰ               
                      RS485_TX_BUFF[8+i*2]= Master_WriteReg[start_address+i]; //���ֽ��ں�
                }
		}       //
	RS485_TX_BUFF[0] = board_adr;
    RS485_TX_BUFF[1] = WRITE_HLD_REG;   //modbus ָ����16
    RS485_TX_BUFF[2] = HI(start_address);  
    RS485_TX_BUFF[3] = LOW(start_address); 
    RS485_TX_BUFF[4] = HI(lenth);
    RS485_TX_BUFF[5] = LOW(lenth);
	RS485_TX_BUFF[6] = 2*lenth;
    calCRC=CRC_Compute(RS485_TX_BUFF,7+2*lenth);
	RS485_TX_BUFF[7+2*lenth]=(calCRC>>8)&0xFF; 
    RS485_TX_BUFF[8+2*lenth]=(calCRC)&0xFF;
	RS485_SendData(RS485_TX_BUFF,9+2*lenth);
}



void Master_Service(u8 SlaverAddr,u8 Fuction,u16 StartAddr,u16 ValueOrLenth)
{    

	switch(Fuction)
	{
		case 01:
				Master_01_Slove(SlaverAddr,StartAddr,ValueOrLenth);
				
				break;
		case 02:
				Master_02_Slove(SlaverAddr,StartAddr,ValueOrLenth);
				break;
		case 03:
				Master_03_Slove(SlaverAddr,StartAddr,ValueOrLenth);
				break;
		case 05:
				Master_05_Slove(SlaverAddr,StartAddr,ValueOrLenth);
				break;
		case 06:
				Master_06_Slove(SlaverAddr,StartAddr,ValueOrLenth);
				break;
		case 15:
				Master_15_Slove(SlaverAddr,StartAddr,ValueOrLenth);
				break;
		case 16:
				Master_16_Slove(SlaverAddr,StartAddr,ValueOrLenth);
				break;

	}
		
}

void RS485_TX_Service(void)
{
	Master_Service( SlaverAddr, Fuction, StartAddr, ValueOrLenth);
}


/////////////////////////////////////////////////////////////////////////////////////
//RS485����������ڴ�����յ�������(������������ѭ������)
u8 i;

void RS485_RX_Service(void)
{
		    u16 calCRC;
        u16 recCRC;
        if(RS485_RxFlag==1)
        {									
						RS485_RxFlag=0;//��λ֡������־
						RS485_RX_CNT=0;//���ռ���������
						RS485_TX_EN=1;//��������ģʽ  
						TX_RX_SET=0; //�������
					  a();
					  
        } 				
}

u8 c,d;

void a()
{
	  
		PM_BUFF[2*i]=RS485_RX_BUFF[3];
		PM_BUFF[1+2*i]=RS485_RX_BUFF[4];
	  i++;
		if(i==4)
		{
			c=(PM_BUFF[0]+PM_BUFF[2]+PM_BUFF[4]+PM_BUFF[6])/4;
		  d=(PM_BUFF[1]+PM_BUFF[3]+PM_BUFF[5]+PM_BUFF[7])/4;
			i=0;
		}
}


#endif

