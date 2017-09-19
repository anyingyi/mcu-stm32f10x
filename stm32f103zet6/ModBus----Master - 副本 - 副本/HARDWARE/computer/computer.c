#include "adc.h"
#include "key.h"


#if ADR_0
#include "computer.h"
#include "led.h"
#include "delay.h"




///////////////////////////////////////////////////////////
u32 CRS485_Baudrate1=9600;//通讯波特率
u8 CRS485_Parity1=0;//0无校验；1奇校验；2偶校验
u8 CRS485_Addr=1;//从机地址
u16 CRS485_Frame_Distance1=4;//数据帧最小间隔（ms),超过此时间则认为是下一帧

u8 CRS485_RX_BUFF1[2048];//接收缓冲区2048字节
u16 CRS485_RX_CNT1=0;//接收计数器
u8 CRS485_FrameFlag=0;//帧结束标记
u8 CRS485_TX_BUFF1[2048];//发送缓冲区
u16 CRS485_TX_CNT1=0;//发送计数器

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CModbus寄存器和单片机寄存器的映射关系
vu32 *CModbus_InputIO[100];//输入开关量寄存器指针(这里使用的是位带操作)
vu32 *CModbus_OutputIO[100];//输出开关量寄存器指针(这里使用的是位带操作)
u16 *CModbus_HoldReg[1000];//保持寄存器指针
u32 Ctest1Data1=1201,Ctest1Data2=1002,Ctest1Data3=2303,Ctest1Data4=8204;

void CModbus1_RegMap(void)
{
        //输入开关量寄存器指针指向
        CModbus_InputIO[0]=(vu32*)&PEin(4);//KEY0     //&PEin(4)：取PE4的地址，(vu32*)&PEin(4)将PE4地址强制转换为uw32类型的地址，CModbus_InputIO[0]=(vu32*)&PEin(4); 将转换好的地址送给地址指针CModbus_InputIO[0]；
        CModbus_InputIO[1]=(vu32*)&PEin(3);//KEY1     //*CModbus_InputIO[0] 取出地址中的内容。
        CModbus_InputIO[2]=(vu32*)&PEin(2);//KEY2
        CModbus_InputIO[3]=(vu32*)&PAin(0);//KEY3
        
        //输出开关量寄存器指针指向
        CModbus_OutputIO[0]=(vu32*)&PBout(5);//LED_COM
        CModbus_OutputIO[1]=(vu32*)&PEout(5);//LED1
        
        //保持寄存器指针指向
        CModbus_HoldReg[0]=(u16*)&Ctest1Data1;//测试数据1 
        CModbus_HoldReg[1]=(u16*)&Ctest1Data2;//((u16*)&Ctest1Data1)+1;//测试数据1 
        CModbus_HoldReg[2]=(u16*)&Ctest1Data3;//(u16*)&Ctest1Data2;//测试数据2
        CModbus_HoldReg[3]=(u16*)&Ctest1Data4;//((u16*)&Ctest1Data2)+1;//测试数据2 
        CModbus_HoldReg[4]=(u16*)&Ctest1Data1;
		CModbus_HoldReg[5]=(u16*)&Ctest1Data2;
		CModbus_HoldReg[6]=(u16*)&Ctest1Data3;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//CCRC校验 自己后面添加的

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

//初始化UART4
void CRS485_Init1(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;
	
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
        
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//PA2（TX）复用推挽输出
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
        GPIO_Init(GPIOC,&GPIO_InitStructure);
     //   GPIO_SetBits(GPIOA,GPIO_Pin_9);//默认高电平
        
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//PA3（RX）输入上拉
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //修改原GPIO_Mode_IPU（输入上拉）->GPIO_Mode_IN_FLOATING(浮空输入)/////////////////////////////////////////////
        GPIO_Init(GPIOC,&GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;//修改PG9（RE/DE）通用推挽输出->PD7（RE/DE）通用推挽输出//////////////////////////////////////////////////////////////////////
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
        GPIO_Init(GPIOD,&GPIO_InitStructure);
        GPIO_ResetBits(GPIOD,GPIO_Pin_7);//默认接收状态
        
        USART_DeInit(UART4);//复位串口2
        USART_InitStructure.USART_BaudRate=CRS485_Baudrate1;
        USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
        USART_InitStructure.USART_WordLength=USART_WordLength_8b;
        USART_InitStructure.USART_StopBits=USART_StopBits_1;
        USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//收发模式
        switch(CRS485_Parity1)
        {
                case 0:USART_InitStructure.USART_Parity=USART_Parity_No;break;//无校验
                case 1:USART_InitStructure.USART_Parity=USART_Parity_Odd;break;//奇校验
                case 2:USART_InitStructure.USART_Parity=USART_Parity_Even;break;//偶校验
        }
        USART_Init(UART4,&USART_InitStructure);
        
        USART_ClearITPendingBit(UART4,USART_IT_RXNE);
        USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);//使能串口2接收中断
        
        NVIC_InitStructure.NVIC_IRQChannel=UART4_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
        NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        
        USART_Cmd(UART4,ENABLE);//使能串口2
        //CRS485_TX_EN=0;//默认为接收模式
        
        Timer5_Init();//定时器7初始化，用于监视空闲时间
        CModbus1_RegMap();//CModbus寄存器映射
}


//定时器5初始化
void Timer5_Init(void)
{
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        NVIC_InitTypeDef NVIC_InitStructure;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //TIM5时钟使能 

        //TIM5初始化设置
        TIM_TimeBaseStructure.TIM_Period = CRS485_Frame_Distance1*10; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
        TIM_TimeBaseStructure.TIM_Prescaler =7200; //设置用来作为TIMx时钟频率除数的预分频值 设置计数频率为10kHz
        TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
        TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

        TIM_ITConfig( TIM5, TIM_IT_Update, ENABLE );//TIM5 允许更新中断
   
        //TIM5中断分组配置
        NVIC_InitStructure.NVIC_IRQChannel =TIM5_IRQn;  //TIM5中断
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
        NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器                                                                  
}



//////////////////////////////////////////////////////////////////////////////
//发送n个字节数据
//buff:发送区首地址
//len：发送的字节数
void CRS485_SendData1(u8 *buff,u8 len)
{ 
        //CRS485_TX_EN=1;//切换为发送模式
        while(len--)
        {
                while(USART_GetFlagStatus(UART4,USART_FLAG_TXE)==RESET);//等待发送区为空
                USART_SendData(UART4,*(buff++));
        }
        while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET);//等待发送完成
}


/////////////////////////////////////////////////////////////////////////////////////
void UART4_IRQHandler(void)//串口2中断服务程序
{
	   
        u8 res;
        u8 err;
	 
        if(USART_GetITStatus(UART4,USART_IT_RXNE)!=RESET)
        {
                if(USART_GetFlagStatus(UART4,USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE)) err=1;//检测到噪音、帧错误或校验错误
                else err=0;
			          LED_COM=0;
                res=USART_ReceiveData(UART4); //读接收到的字节，同时相关标志自动清除
                
                if((CRS485_RX_CNT1<2047)&&(err==0))
                {
                        CRS485_RX_BUFF1[CRS485_RX_CNT1]=res;
                        CRS485_RX_CNT1++;
                        
                        TIM_ClearITPendingBit(TIM5,TIM_IT_Update);//清除定时器溢出中断
                        TIM_SetCounter(TIM5,0);//当接收到一个新的字节，将定时器7复位为0，重新计时（相当于喂狗）
                        TIM_Cmd(TIM5,ENABLE);//开始计时
                }
        }
}

///////////////////////////////////////////////////////////////////////////////////////
//用定时器7判断接收空闲时间，当空闲时间大于指定时间，认为一帧结束
//定时器7中断服务程序         
void TIM5_IRQHandler(void)
{                                                                   
        if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)
        {
                TIM_ClearITPendingBit(TIM5,TIM_IT_Update);//清除中断标志
                TIM_Cmd(TIM5,DISABLE);//停止定时器
                //CRS485_TX_EN=1;//停止接收，切换为发送状态
                CRS485_FrameFlag=1;//置位帧结束标记
        }
}

/////////////////////////////////////////////////////////////////////////////////////
//CRS485服务程序，用于处理接收到的数据(请在主函数中循环调用)
u16 CstartRegAddr;
u16 CRegNum;
u16 calCCRC;
 void CRS485_Service(void)
{
	      
        u16 recCCRC;
        if(CRS485_FrameFlag==1)
        {
                if(CRS485_RX_BUFF1[0]==CRS485_Addr)//地址正确
                {
                        if((CRS485_RX_BUFF1[1]==01)||(CRS485_RX_BUFF1[1]==02)||(CRS485_RX_BUFF1[1]==03)||(CRS485_RX_BUFF1[1]==05)||(CRS485_RX_BUFF1[1]==06)||(CRS485_RX_BUFF1[1]==15)||(CRS485_RX_BUFF1[1]==16))//功能码正确
                        {
                                CstartRegAddr=(((u16)CRS485_RX_BUFF1[2])<<8)|CRS485_RX_BUFF1[3];//获取寄存器起始地址
                                if(CstartRegAddr<1000)//寄存器地址在范围内
                                {
                                        calCCRC=CCRC_Compute1(CRS485_RX_BUFF1,CRS485_RX_CNT1-2);//计算所接收数据的CCRC
                                        recCCRC=CRS485_RX_BUFF1[CRS485_RX_CNT1-1]|(((u16)CRS485_RX_BUFF1[CRS485_RX_CNT1-2])<<8);//接收到的CCRC(低字节在前，高字节在后)
                                        if(calCCRC==recCCRC)//CCRC校验正确
                                        {
												//LED1=0;
                                                /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                switch(CRS485_RX_BUFF1[1])//根据不同的功能码进行处理
                                                {
                                                        case 2://读输入开关量
                                                        {
                                                                CModbus_02_1Solve();
                                                                break;
                                                        }
                                                        
                                                        case 1://读输出开关量
                                                        {
                                                                CModbus_01_1Solve();
                                                                break;
                                                        }
                                                                
                                                        case 5://写单个输出开关量
                                                        {
                                                                CModbus_05_1Solve();
                                                                break;
                                                        }
                                                                
                                                        case 15://写多个输出开关量
                                                        {
                                                                CModbus_15_1Solve();
                                                                break;
                                                        }
                                                                
                                                        case 03: //读多个寄存器
                                                        {																
                                                                CModbus_03_1Solve();
                                                                break;
                                                        }
                                                                
                                                        case 06: //写单个寄存器
                                                        {
                                                                CModbus_06_1Solve();
                                                                break;
                                                        }
                                                                
                                                        case 16: //写多个寄存器
                                                        {
                                                                CModbus_16_1Solve();
                                                                break;
                                                        }
                                                                                        
                                                }
                                                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                        }
                                        else//CCRC校验错误
                                        {

                                                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                                                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1]|0x80;
                                                CRS485_TX_BUFF1[2]=0x04; //异常码
                                                CRS485_SendData1(CRS485_TX_BUFF1,3);
                                        }        
                                }
                                else//寄存器地址超出范围
                                {
                                        CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                                        CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1]|0x80;
                                        CRS485_TX_BUFF1[2]=0x02; //异常码
                                        CRS485_SendData1(CRS485_TX_BUFF1,3);
                                }                                                
                        }
                        else//功能码错误
                        {
													      
                                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1]|0x80;
                                CRS485_TX_BUFF1[2]=0x01; //异常码
                                CRS485_SendData1(CRS485_TX_BUFF1,3);
                        }
											//LED_COM=!LED_COM;	
                }
                 
                
                CRS485_FrameFlag=0;//复位帧结束标志
                CRS485_RX_CNT1=0;//接收计数器清零
                //CRS485_TX_EN=0;//开启接收模式                
        }                
}

//CModbus功能码02处理程序/////////////////////////////////////////////////////程序已验证OK -----必须先配置PE4 PE3 PE2 PA0 初始化按键才可以OK    KEY_Init();
//读输入开关量
void CModbus_02_1Solve(void)
{
        u16 ByteNum;
        u16 i;
        CRegNum= (((u16)CRS485_RX_BUFF1[4])<<8)|CRS485_RX_BUFF1[5];//获取寄存器数量
        if((CstartRegAddr+CRegNum)<100)//寄存器地址+数量在范围内
        {
                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1];
                ByteNum=CRegNum/8;//字节数
                if(CRegNum%8) ByteNum+=1;//如果位数还有余数，则字节数+1
                CRS485_TX_BUFF1[2]=ByteNum;//返回要读取的字节数
                for(i=0;i<CRegNum;i++)
                {
                        if(i%8==0) CRS485_TX_BUFF1[3+i/8]=0x00;
                        CRS485_TX_BUFF1[3+i/8]>>=1;//低位先发送
                        CRS485_TX_BUFF1[3+i/8]|=((*CModbus_InputIO[CstartRegAddr+i])<<7)&0x80;
                        if(i==CRegNum-1)//发送到最后一个位了
                        {
                                if(CRegNum%8) CRS485_TX_BUFF1[3+i/8]>>=8-(CRegNum%8);//如果最后一个字节还有余数，则剩余MSB填充0
                        }
                }
                calCCRC=CCRC_Compute1(CRS485_TX_BUFF1,ByteNum+3);
                CRS485_TX_BUFF1[ByteNum+3]=(calCCRC>>8)&0xFF;
                CRS485_TX_BUFF1[ByteNum+4]=(calCCRC)&0xFF;
                CRS485_SendData1(CRS485_TX_BUFF1,ByteNum+5);
        }
        else//寄存器地址+数量超出范围
        {
                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1]|0x80;
                CRS485_TX_BUFF1[2]=0x02; //异常码
                CRS485_SendData1(CRS485_TX_BUFF1,3);
        }
}

//CModbus功能码01处理程序 ///////////////////////////////////////////////////////////程序已验证OK
//读输出开关量
void CModbus_01_1Solve(void)
{
        u16 ByteNum;
        u16 i;
        CRegNum= (((u16)CRS485_RX_BUFF1[4])<<8)|CRS485_RX_BUFF1[5];//获取寄存器数量
        if((CstartRegAddr+CRegNum)<100)//寄存器地址+数量在范围内
        {
                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1];
                ByteNum=CRegNum/8;//字节数
                if(CRegNum%8) ByteNum+=1;//如果位数还有余数，则字节数+1
                CRS485_TX_BUFF1[2]=ByteNum;//返回要读取的字节数
                for(i=0;i<CRegNum;i++)
                {
                        if(i%8==0) CRS485_TX_BUFF1[3+i/8]=0x00;
                        CRS485_TX_BUFF1[3+i/8]>>=1;//低位先发送
                        CRS485_TX_BUFF1[3+i/8]|=((*CModbus_OutputIO[CstartRegAddr+i])<<7)&0x80;
                        if(i==CRegNum-1)//发送到最后一个位了
                        {
                                if(CRegNum%8) CRS485_TX_BUFF1[3+i/8]>>=8-(CRegNum%8);//如果最后一个字节还有余数，则剩余MSB填充0
                        }
                }
                calCCRC=CCRC_Compute1(CRS485_TX_BUFF1,ByteNum+3);
                CRS485_TX_BUFF1[ByteNum+3]=(calCCRC>>8)&0xFF;
                CRS485_TX_BUFF1[ByteNum+4]=(calCCRC)&0xFF;
                CRS485_SendData1(CRS485_TX_BUFF1,ByteNum+5);
        }
        else//寄存器地址+数量超出范围
        {
                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1]|0x80;
                CRS485_TX_BUFF1[2]=0x02; //异常码
                CRS485_SendData1(CRS485_TX_BUFF1,3);
        }
}

//CModbus功能码05处理程序   ///////////////////////////////////////////////////////程序已验证OK
//写单个输出开关量
void CModbus_05_1Solve(void)
{
        if(CstartRegAddr<100)//寄存器地址在范围内
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
        else//寄存器地址超出范围
        {
                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1]|0x80;
                CRS485_TX_BUFF1[2]=0x02; //异常码
                CRS485_SendData1(CRS485_TX_BUFF1,3);
        }
}

//CModbus功能码15处理程序   //////////////////////////////////////////////////////程序已验证OK
//写多个输出开关量
void CModbus_15_1Solve(void)
{
        u16 i;
        CRegNum=(((u16)CRS485_RX_BUFF1[4])<<8)|CRS485_RX_BUFF1[5];//获取寄存器数量
        if((CstartRegAddr+CRegNum)<100)//寄存器地址+数量在范围内
        {        
                for(i=0;i<CRegNum;i++)
                {
                        if(CRS485_RX_BUFF1[7+i/8]&0x01) *CModbus_OutputIO[CstartRegAddr+i]=0x01;
                        else *CModbus_OutputIO[CstartRegAddr+i]=0x00;
                        CRS485_RX_BUFF1[7+i/8]>>=1;//从低位开始
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
        else//寄存器地址+数量超出范围
        {
                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1]|0x80;
                CRS485_TX_BUFF1[2]=0x02; //异常码
                CRS485_SendData1(CRS485_TX_BUFF1,3);
        }
}

extern u8 c,d;
extern u16  rh_value,t_value;
extern u16 PM_DATA;
//CModbus功能码03处理程序///////////////////////////////////////////////////////////////////////////////////////已验证程序OK
//读保持寄存器
void CModbus_03_1Solve(void)
{
        u8 i;
        CRegNum= (((u16)CRS485_RX_BUFF1[4])<<8)|CRS485_RX_BUFF1[5];//获取寄存器数量
        if((CstartRegAddr+CRegNum)<1000)//寄存器地址+数量在范围内
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
                CRS485_TX_BUFF1[CRegNum*2+7]=(calCCRC>>8)&0xFF;         //CCRC高地位不对吗？  // 先高后低
                CRS485_TX_BUFF1[CRegNum*2+8]=(calCCRC)&0xFF;
								
                CRS485_SendData1(CRS485_TX_BUFF1,11);
        }
        else//寄存器地址+数量超出范围
        {
                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1]|0x80;
                CRS485_TX_BUFF1[2]=0x02; //异常码
                CRS485_SendData1(CRS485_TX_BUFF1,3);
        }
}


//CModbus功能码06处理程序   //////////////////////////////////////////////////////////////////////////////////已验证程序OK
//写单个保持寄存器
void CModbus_06_1Solve(void)
{
        *CModbus_HoldReg[CstartRegAddr]=CRS485_RX_BUFF1[4]<<8;//高字节在前                    ////////修改为高字节在前，低字节在后
        *CModbus_HoldReg[CstartRegAddr]|=((u16)CRS485_RX_BUFF1[5]);//低字节在后
        
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

//CModbus功能码16处理程序 /////////////////////////////////////////////////////////////////////////////////////////////////已验证程序OK
//写多个保持寄存器
void CModbus_16_1Solve(void)
{
        u8 i;
        CRegNum= (((u16)CRS485_RX_BUFF1[4])<<8)|((CRS485_RX_BUFF1[5]));//获取寄存器数量
        if((CstartRegAddr+CRegNum)<1000)//寄存器地址+数量在范围内
        {
                for(i=0;i<CRegNum;i++)
                {
                        *CModbus_HoldReg[CstartRegAddr+i]=CRS485_RX_BUFF1[7+i*2]; //低字节在前                 /////// 低字节在前，高字节在后正常
                        *CModbus_HoldReg[CstartRegAddr+i]|=((u16)CRS485_RX_BUFF1[8+i*2])<<8; //高字节在后
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
        else//寄存器地址+数量超出范围
        {
                CRS485_TX_BUFF1[0]=CRS485_RX_BUFF1[0];
                CRS485_TX_BUFF1[1]=CRS485_RX_BUFF1[1]|0x80;
                CRS485_TX_BUFF1[2]=0x02; //异常码
                CRS485_SendData1(CRS485_TX_BUFF1,3);
        }
}

#endif

 
