/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 


 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
}

u16 USART2_BUF_Receive[32];//存储串口2的缓冲数据// Device header
u16  RX_CNT=0;//接收计数器
//TIM7定时中断服务函数
void TIM2_IRQHandler() //定时器7初始化---功能：判断从机返回的数据是否接受完成
{ 
   if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
        {
             
           TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除中断标志
           TIM_Cmd(TIM2,DISABLE);//停止定时器
        }	
}


//2毫秒接收一次数据
u16 PM_DATA;
void USART2_IRQHandler(void)    //中断服务函数
{ 
	  u8 res;
    u8 err;
	  u8 j;
	  u16 check_PM_sum;
   if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
        {     
					
                if(USART_GetFlagStatus(USART2,USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE)) err=1;//检测到噪音、帧错误或校验错误
                else err=0;
                res=USART_ReceiveData(USART2); //读接收到的字节，同时相关标志自动清除
                
                if((RX_CNT<32)&&(err==0))
                {
                        USART2_BUF_Receive[RX_CNT]=res;
                        RX_CNT++;
                        
                        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除定时器溢出中断
                        TIM_SetCounter(TIM2,0);//当接收到一个新的字节，将定时器7复位为0，重新计时
                        TIM_Cmd(TIM2,ENABLE);//开始计时
                }	
			
                if( RX_CNT == 32 ) 
			         {
					      if((USART2_BUF_Receive[0]==0x42)&&(USART2_BUF_Receive[1]==0x4d))
				   	      {						
							    for(j=0;j<30;j++)
							      {						
									    check_PM_sum +=USART2_BUF_Receive[j]; 
							       }					
							         if(check_PM_sum==((USART2_BUF_Receive[30]<<8)+USART2_BUF_Receive[31]) ) //判断校验位。30,31为校验和
									    PM_DATA=((USART2_BUF_Receive[6]<<8)+USART2_BUF_Receive[7]);   //PM2.5的值
				           }
		   
					
			         }	
		   }
				
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
