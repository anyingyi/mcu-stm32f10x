#include "adc.h"
#include "key.h"


#if ADR_0
#include "computer.h"
#include "led.h"
#include "delay.h"




///////////////////////////////////////////////////////////
u32 CRS485_Baudrate1=9600;//ͨѶ������
u8 CRS485_Parity1=0;//0��У�飻1��У�飻2żУ��
u8 CRS485_Addr=1;//�ӻ���ַ
u16 CRS485_Frame_Distance1=4;//����֡��С�����ms),������ʱ������Ϊ����һ֡

u8 CRS485_RX_BUFF1[2048];//���ջ�����2048�ֽ�
u16 CRS485_RX_CNT1=0;//���ռ�����
u8 CRS485_FrameFlag=0;//֡�������
u8 CRS485_TX_BUFF1[2048];//���ͻ�����
u16 CRS485_TX_CNT1=0;//���ͼ�����

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CModbus�Ĵ����͵�Ƭ���Ĵ�����ӳ���ϵ
vu32 *CModbus_InputIO[100];//���뿪�����Ĵ���ָ��(����ʹ�õ���λ������)
vu32 *CModbus_OutputIO[100];//����������Ĵ���ָ��(����ʹ�õ���λ������)
u16 *CModbus_HoldReg[1000];//���ּĴ���ָ��
u32 Ctest1Data1=1201,Ctest1Data2=1002,Ctest1Data3=2303,Ctest1Data4=8204;

void CModbus1_RegMap(void)
{
        //���뿪�����Ĵ���ָ��ָ��
        CModbus_InputIO[0]=(vu32*)&PEin(4);//KEY0     //&PEin(4)��ȡPE4�ĵ�ַ��(vu32*)&PEin(4)��PE4��ַǿ��ת��Ϊuw32���͵ĵ�ַ��CModbus_InputIO[0]=(vu32*)&PEin(4); ��ת���õĵ�ַ�͸���ַָ��CModbus_InputIO[0]��
        CModbus_InputIO[1]=(vu32*)&PEin(3);//KEY1     //*CModbus_InputIO[0] ȡ����ַ�е����ݡ�
        CModbus_InputIO[2]=(vu32*)&PEin(2);//KEY2
        CModbus_InputIO[3]=(vu32*)&PAin(0);//KEY3
        
        //����������Ĵ���ָ��ָ��
        CModbus_OutputIO[0]=(vu32*)&PBout(5);//LED_COM
        CModbus_OutputIO[1]=(vu32*)&PEout(5);//LED1
        
        //���ּĴ���ָ��ָ��
        CModbus_HoldReg[0]=(u16*)&Ctest1Data1;//��������1 
        CModbus_HoldReg[1]=(u16*)&Ctest1Data2;//((u16*)&Ctest1Data1)+1;//��������1 
        CModbus_HoldReg[2]=(u16*)&Ctest1Data3;//(u16*)&Ctest1Data2;//��������2
        CModbus_HoldReg[3]=(u16*)&Ctest1Data4;//((u16*)&Ctest1Data2)+1;//��������2 
        CModbus_HoldReg[4]=(u16*)&Ctest1Data1;
		CModbus_HoldReg[5]=(u16*)&Ctest1Data2;
		CModbus_HoldReg[6]=(u16*)&Ctest1Data3;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//CCRCУ�� �Լ�������ӵ�

const u8 auchCCRCHi1[] = { 
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


const u8 auchCCRCLo1[] = { 
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


u16 CCRC_Compute1(u8 *puchMsg, u16 usDataLen) 
{ 
	u8 uchCCRCHi = 0xFF ; 
	u8 uchCCRCLo = 0xFF ; 
	u32 uIndex ; 
	while (usDataLen--) 
	{ 
		uIndex = uchCCRCHi ^ *puchMsg++ ; 
		uchCCRCHi = uchCCRCLo ^ auchCCRCHi1[uIndex] ; 
		uchCCRCLo = auchCCRCLo1[uIndex] ; 
	} 
	return ((uchCCRCHi<< 8)  | (uchCCRCLo)) ; 
}//uint16 CCRC16(uint8 *puchMsg, uint16 usDataLen)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
        USART_InitStructure.USART_BaudRate=CRS485_Baudrate1;
        USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
        USART_InitStructure.USART_WordLength=USART_WordLength_8b;
        USART_InitStructure.USART_StopBits=USART_StopBits_1;
        USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//�շ�ģʽ
        switch(CRS485_Parity1)
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
        //CRS485_TX_EN=0;//Ĭ��Ϊ����ģʽ
        
        Timer5_Init();//��ʱ��7��ʼ�������ڼ��ӿ���ʱ��
        CModbus1_RegMap();//CModbus�Ĵ���ӳ��
}


//��ʱ��5��ʼ��
void Timer5_Init(void)
{
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        NVIC_InitTypeDef NVIC_InitStructure;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //TIM5ʱ��ʹ�� 

        //TIM5��ʼ������
        TIM_TimeBaseStructure.TIM_Period = CRS485_Frame_Distance1*10; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
        TIM_TimeBaseStructure.TIM_Prescaler =7200; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ ���ü���Ƶ��Ϊ10kHz
        TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
        TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

        TIM_ITConfig( TIM5, TIM_IT_Update, ENABLE );//TIM5 ��������ж�
   
        //TIM5�жϷ�������
        NVIC_InitStructure.NVIC_IRQChannel =TIM5_IRQn;  //TIM5�ж�
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
        NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���                                                                  
}



//////////////////////////////////////////////////////////////////////////////
//����n���ֽ�����
//buff:�������׵�ַ
//len�����͵��ֽ���
void CRS485_SendData1(u8 *buff,u8 len)
{ 
        //CRS485_TX_EN=1;//�л�Ϊ����ģʽ
        while(len--)
        {
                while(USART_GetFlagStatus(UART4,USART_FLAG_TXE)==RESET);//�ȴ�������Ϊ��
                USART_SendData(UART4,*(buff++));
        }
        while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET);//�ȴ��������
}


/////////////////////////////////////////////////////////////////////////////////////
void UART4_IRQHandler(void)//����2�жϷ������
{
	   
        u8 res;
        u8 err;
	 
        if(USART_GetITStatus(UART4,USART_IT_RXNE)!=RESET)
        {
                if(USART_GetFlagStatus(UART4,USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE)) err=1;//��⵽������֡�����У�����
                else err=0;
			          LED_COM=0;
                res=USART_ReceiveData(UART4); //�����յ����ֽڣ�ͬʱ��ر�־�Զ����
                
                if((CRS485_RX_CNT1<2047)&&(err==0))
                {
                        CRS485_RX_BUFF1[CRS485_RX_CNT1]=res;
                        CRS485_RX_CNT1++;
                        
                        TIM_ClearITPendingBit(TIM5,TIM_IT_Update);//�����ʱ������ж�
                        TIM_SetCounter(TIM5,0);//�����յ�һ���µ��ֽڣ�����ʱ��7��λΪ0�����¼�ʱ���൱��ι����
                        TIM_Cmd(TIM5,ENABLE);//��ʼ��ʱ
                }
        }
}

///////////////////////////////////////////////////////////////////////////////////////
//�ö�ʱ��7�жϽ��տ���ʱ�䣬������ʱ�����ָ��ʱ�䣬��Ϊһ֡����
//��ʱ��7�жϷ������         
void TIM5_IRQHandler(void)
{                                                                   
        if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)
        {
                TIM_ClearITPendingBit(TIM5,TIM_IT_Update);//����жϱ�־
                TIM_Cmd(TIM5,DISABLE);//ֹͣ��ʱ��
                //CRS485_TX_EN=1;//ֹͣ���գ��л�Ϊ����״̬
                CRS485_FrameFlag=1;//��λ֡�������
        }
}

/////////////////////////////////////////////////////////////////////////////////////
//CRS485����������ڴ�����յ�������(������������ѭ������)
u16 CstartRegAddr;
u16 CRegNum;
u16 calCCRC;
 void CRS485_Service(void)
{
	      
        u16 recCCRC;
        if(CRS485_FrameFlag==1)
        {
                if(CRS485_RX_BUFF1[0]==CRS485_Addr)//��ַ��ȷ
                {
                        if((CRS485_RX_BUFF1[1]==01)||(CRS485_RX_BUFF1[1]==02)||(CRS485_RX_BUFF1[1]==03)||(CRS485_RX_BUFF1[1]==05)||(CRS485_RX_BUFF1[1]==06)||(CRS485_RX_BUFF1[1]==15)||(CRS485_RX_BUFF1[1]==16))//��������ȷ
                        {
                                CstartRegAddr=(((u16)CRS485_RX_BUFF1[2])<<8)|CRS485_RX_BUFF1[3];//��ȡ�Ĵ�����ʼ��ַ
                                if(CstartRegAddr<1000)//�Ĵ�����ַ�ڷ�Χ��
                                {
                                        calCCRC=CCRC_Compute1(CRS485_RX_BUFF1,CRS485_RX_CNT1-2);//�������������ݵ�CCRC
                                        recCCRC=CRS485_RX_BUFF1[CRS485_RX_CNT1-1]|(((u16)CRS485_RX_BUFF1[CRS485_RX_CNT1-2])<<8);//���յ���CCRC(���ֽ���ǰ�����ֽ��ں�)
                                        if(calCCRC==recCCRC)//CCRCУ����ȷ
                                        {
												//LED1=0;
                                                /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                switch(CRS485_RX_BUFF1[1])//���ݲ�ͬ�Ĺ�������д���
                                                {
                                                        case 2://�����뿪����
                                                        {
                                                                CModbus_02_1Solve();
                                                                break;
                                                        }
                                                        
                                                        case 1://�����������
                                                        {
                                                                CModbus_01_1Solve();
                                                                break;
                                                        }
                                                                
                                                        case 5://д�������������
                                                        {
                                                                CModbus_05_1Solve();
                                                                break;
                                                        }
                                                                
                                                        case 15://д������������
                                                        {
                                                                CModbus_15_1Solve();
                                                                break;
                                                        }
                                                                
                                                        case 03: //������Ĵ���
                                                        {																
                                                                CModbus_03_1Solve();
                                                                break;
                                                        }
                                                                
                                                        case 06: //д�����Ĵ���
                                                        {
                                                                CModbus_06_1Solve();
                                                                break;
                                                        }
                                                                
                                                        case 16: //д����Ĵ���
                                                        {
                                                                CModbus_16_1Solve();
                                                                break;
                                                        }
                                                                                        
                                                }
                                                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                        }
                                        else//CCRCУ�����
                                        {

                                                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                                                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1]|0x80;
                                                CRS485_TX_BUFF1[2]=0x04; //�쳣��
                                                CRS485_SendData1(CRS485_TX_BUFF1,3);
                                        }        
                                }
                                else//�Ĵ�����ַ������Χ
                                {
                                        CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                                        CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1]|0x80;
                                        CRS485_TX_BUFF1[2]=0x02; //�쳣��
                                        CRS485_SendData1(CRS485_TX_BUFF1,3);
                                }                                                
                        }
                        else//���������
                        {
													      
                                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1]|0x80;
                                CRS485_TX_BUFF1[2]=0x01; //�쳣��
                                CRS485_SendData1(CRS485_TX_BUFF1,3);
                        }
											//LED_COM=!LED_COM;	
                }
                 
                
                CRS485_FrameFlag=0;//��λ֡������־
                CRS485_RX_CNT1=0;//���ռ���������
                //CRS485_TX_EN=0;//��������ģʽ                
        }                
}

//CModbus������02�������/////////////////////////////////////////////////////��������֤OK -----����������PE4 PE3 PE2 PA0 ��ʼ�������ſ���OK    KEY_Init();
//�����뿪����
void CModbus_02_1Solve(void)
{
        u16 ByteNum;
        u16 i;
        CRegNum= (((u16)CRS485_RX_BUFF1[4])<<8)|CRS485_RX_BUFF1[5];//��ȡ�Ĵ�������
        if((CstartRegAddr+CRegNum)<100)//�Ĵ�����ַ+�����ڷ�Χ��
        {
                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1];
                ByteNum=CRegNum/8;//�ֽ���
                if(CRegNum%8) ByteNum+=1;//���λ���������������ֽ���+1
                CRS485_TX_BUFF1[2]=ByteNum;//����Ҫ��ȡ���ֽ���
                for(i=0;i<CRegNum;i++)
                {
                        if(i%8==0) CRS485_TX_BUFF1[3+i/8]=0x00;
                        CRS485_TX_BUFF1[3+i/8]>>=1;//��λ�ȷ���
                        CRS485_TX_BUFF1[3+i/8]|=((*CModbus_InputIO[CstartRegAddr+i])<<7)&0x80;
                        if(i==CRegNum-1)//���͵����һ��λ��
                        {
                                if(CRegNum%8) CRS485_TX_BUFF1[3+i/8]>>=8-(CRegNum%8);//������һ���ֽڻ�����������ʣ��MSB���0
                        }
                }
                calCCRC=CCRC_Compute1(CRS485_TX_BUFF1,ByteNum+3);
                CRS485_TX_BUFF1[ByteNum+3]=(calCCRC>>8)&0xFF;
                CRS485_TX_BUFF1[ByteNum+4]=(calCCRC)&0xFF;
                CRS485_SendData1(CRS485_TX_BUFF1,ByteNum+5);
        }
        else//�Ĵ�����ַ+����������Χ
        {
                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1]|0x80;
                CRS485_TX_BUFF1[2]=0x02; //�쳣��
                CRS485_SendData1(CRS485_TX_BUFF1,3);
        }
}

//CModbus������01������� ///////////////////////////////////////////////////////////��������֤OK
//�����������
void CModbus_01_1Solve(void)
{
        u16 ByteNum;
        u16 i;
        CRegNum= (((u16)CRS485_RX_BUFF1[4])<<8)|CRS485_RX_BUFF1[5];//��ȡ�Ĵ�������
        if((CstartRegAddr+CRegNum)<100)//�Ĵ�����ַ+�����ڷ�Χ��
        {
                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1];
                ByteNum=CRegNum/8;//�ֽ���
                if(CRegNum%8) ByteNum+=1;//���λ���������������ֽ���+1
                CRS485_TX_BUFF1[2]=ByteNum;//����Ҫ��ȡ���ֽ���
                for(i=0;i<CRegNum;i++)
                {
                        if(i%8==0) CRS485_TX_BUFF1[3+i/8]=0x00;
                        CRS485_TX_BUFF1[3+i/8]>>=1;//��λ�ȷ���
                        CRS485_TX_BUFF1[3+i/8]|=((*CModbus_OutputIO[CstartRegAddr+i])<<7)&0x80;
                        if(i==CRegNum-1)//���͵����һ��λ��
                        {
                                if(CRegNum%8) CRS485_TX_BUFF1[3+i/8]>>=8-(CRegNum%8);//������һ���ֽڻ�����������ʣ��MSB���0
                        }
                }
                calCCRC=CCRC_Compute1(CRS485_TX_BUFF1,ByteNum+3);
                CRS485_TX_BUFF1[ByteNum+3]=(calCCRC>>8)&0xFF;
                CRS485_TX_BUFF1[ByteNum+4]=(calCCRC)&0xFF;
                CRS485_SendData1(CRS485_TX_BUFF1,ByteNum+5);
        }
        else//�Ĵ�����ַ+����������Χ
        {
                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1]|0x80;
                CRS485_TX_BUFF1[2]=0x02; //�쳣��
                CRS485_SendData1(CRS485_TX_BUFF1,3);
        }
}

//CModbus������05�������   ///////////////////////////////////////////////////////��������֤OK
//д�������������
void CModbus_05_1Solve(void)
{
        if(CstartRegAddr<100)//�Ĵ�����ַ�ڷ�Χ��
        {
                if((CRS485_RX_BUFF1[4]==0xFF)||(CRS485_RX_BUFF1[5]==0xFF)) *CModbus_OutputIO[CstartRegAddr]=0x01;
                else *CModbus_OutputIO[CstartRegAddr]=0x00;
                
                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1];
                CRS485_TX_BUFF1[2]=CRS485_RX_BUFF1[2];
                CRS485_TX_BUFF1[3]=CRS485_RX_BUFF1[3];
                CRS485_TX_BUFF1[4]=CRS485_RX_BUFF1[4];
                CRS485_TX_BUFF1[5]=CRS485_RX_BUFF1[5];
                
                calCCRC=CCRC_Compute1(CRS485_TX_BUFF1,6);
                CRS485_TX_BUFF1[6]=(calCCRC>>8)&0xFF;
                CRS485_TX_BUFF1[7]=(calCCRC)&0xFF;
                CRS485_SendData1(CRS485_TX_BUFF1,8);
        }
        else//�Ĵ�����ַ������Χ
        {
                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1]|0x80;
                CRS485_TX_BUFF1[2]=0x02; //�쳣��
                CRS485_SendData1(CRS485_TX_BUFF1,3);
        }
}

//CModbus������15�������   //////////////////////////////////////////////////////��������֤OK
//д������������
void CModbus_15_1Solve(void)
{
        u16 i;
        CRegNum=(((u16)CRS485_RX_BUFF1[4])<<8)|CRS485_RX_BUFF1[5];//��ȡ�Ĵ�������
        if((CstartRegAddr+CRegNum)<100)//�Ĵ�����ַ+�����ڷ�Χ��
        {        
                for(i=0;i<CRegNum;i++)
                {
                        if(CRS485_RX_BUFF1[7+i/8]&0x01) *CModbus_OutputIO[CstartRegAddr+i]=0x01;
                        else *CModbus_OutputIO[CstartRegAddr+i]=0x00;
                        CRS485_RX_BUFF1[7+i/8]>>=1;//�ӵ�λ��ʼ
                }
                
                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1];
                CRS485_TX_BUFF1[2]=CRS485_RX_BUFF1[2];
                CRS485_TX_BUFF1[3]=CRS485_RX_BUFF1[3];
                CRS485_TX_BUFF1[4]=CRS485_RX_BUFF1[4];
                CRS485_TX_BUFF1[5]=CRS485_RX_BUFF1[5];
                calCCRC=CCRC_Compute1(CRS485_TX_BUFF1,6);
                CRS485_TX_BUFF1[6]=(calCCRC>>8)&0xFF;
                CRS485_TX_BUFF1[7]=(calCCRC)&0xFF;
                CRS485_SendData1(CRS485_TX_BUFF1,8);
        }
        else//�Ĵ�����ַ+����������Χ
        {
                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1]|0x80;
                CRS485_TX_BUFF1[2]=0x02; //�쳣��
                CRS485_SendData1(CRS485_TX_BUFF1,3);
        }
}

extern u8 c,d;
extern u16  rh_value,t_value;
extern u16 PM_DATA;
//CModbus������03�������///////////////////////////////////////////////////////////////////////////////////////����֤����OK
//�����ּĴ���
void CModbus_03_1Solve(void)
{
        u8 i;
        CRegNum= (((u16)CRS485_RX_BUFF1[4])<<8)|CRS485_RX_BUFF1[5];//��ȡ�Ĵ�������
        if((CstartRegAddr+CRegNum)<1000)//�Ĵ�����ַ+�����ڷ�Χ��
        {
                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1];
                CRS485_TX_BUFF1[2]=CRegNum*2;
                for(i=0;i<CRegNum;i++)
                {
                        CRS485_TX_BUFF1[3+i*2]=c;//PM_DATA>>8;
                        CRS485_TX_BUFF1[4+i*2]=d;//PM_DATA;
									      CRS485_TX_BUFF1[5+i*2]=0;//rh_value;
                        CRS485_TX_BUFF1[6+i*2]=0;//t_value;
									      CRS485_TX_BUFF1[7+i*2]=0;
                        CRS485_TX_BUFF1[8+i*2]=0;
                }
                calCCRC=CCRC_Compute1(CRS485_TX_BUFF1,CRegNum*2+3);
                CRS485_TX_BUFF1[CRegNum*2+7]=(calCCRC>>8)&0xFF;         //CCRC�ߵ�λ������  // �ȸߺ��
                CRS485_TX_BUFF1[CRegNum*2+8]=(calCCRC)&0xFF;
								
                CRS485_SendData1(CRS485_TX_BUFF1,11);
        }
        else//�Ĵ�����ַ+����������Χ
        {
                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1]|0x80;
                CRS485_TX_BUFF1[2]=0x02; //�쳣��
                CRS485_SendData1(CRS485_TX_BUFF1,3);
        }
}


//CModbus������06�������   //////////////////////////////////////////////////////////////////////////////////����֤����OK
//д�������ּĴ���
void CModbus_06_1Solve(void)
{
        *CModbus_HoldReg[CstartRegAddr]=CRS485_RX_BUFF1[4]<<8;//���ֽ���ǰ                    ////////�޸�Ϊ���ֽ���ǰ�����ֽ��ں�
        *CModbus_HoldReg[CstartRegAddr]|=((u16)CRS485_RX_BUFF1[5]);//���ֽ��ں�
        
        CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
        CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1];
        CRS485_TX_BUFF1[2]=CRS485_RX_BUFF1[2];
        CRS485_TX_BUFF1[3]=CRS485_RX_BUFF1[3];
        CRS485_TX_BUFF1[4]=CRS485_RX_BUFF1[4];
        CRS485_TX_BUFF1[5]=CRS485_RX_BUFF1[5];
        
        calCCRC=CCRC_Compute1(CRS485_TX_BUFF1,6);
        CRS485_TX_BUFF1[6]=(calCCRC>>8)&0xFF;
        CRS485_TX_BUFF1[7]=(calCCRC)&0xFF;
        CRS485_SendData1(CRS485_TX_BUFF1,8);
}

//CModbus������16������� /////////////////////////////////////////////////////////////////////////////////////////////////����֤����OK
//д������ּĴ���
void CModbus_16_1Solve(void)
{
        u8 i;
        CRegNum= (((u16)CRS485_RX_BUFF1[4])<<8)|((CRS485_RX_BUFF1[5]));//��ȡ�Ĵ�������
        if((CstartRegAddr+CRegNum)<1000)//�Ĵ�����ַ+�����ڷ�Χ��
        {
                for(i=0;i<CRegNum;i++)
                {
                        *CModbus_HoldReg[CstartRegAddr+i]=CRS485_RX_BUFF1[7+i*2]; //���ֽ���ǰ                 /////// ���ֽ���ǰ�����ֽ��ں�����
                        *CModbus_HoldReg[CstartRegAddr+i]|=((u16)CRS485_RX_BUFF1[8+i*2])<<8; //���ֽ��ں�
                }
                
                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1];
                CRS485_TX_BUFF1[2]=CRS485_RX_BUFF1[2];
                CRS485_TX_BUFF1[3]=CRS485_RX_BUFF1[3];
                CRS485_TX_BUFF1[4]=CRS485_RX_BUFF1[4];
                CRS485_TX_BUFF1[5]=CRS485_RX_BUFF1[5];
                
                calCCRC=CCRC_Compute1(CRS485_TX_BUFF1,6);
                CRS485_TX_BUFF1[6]=(calCCRC>>8)&0xFF;
                CRS485_TX_BUFF1[7]=(calCCRC)&0xFF;
                CRS485_SendData1(CRS485_TX_BUFF1,8);
        }
        else//�Ĵ�����ַ+����������Χ
        {
                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1]|0x80;
                CRS485_TX_BUFF1[2]=0x02; //�쳣��
                CRS485_SendData1(CRS485_TX_BUFF1,3);
        }
}

#endif

 
