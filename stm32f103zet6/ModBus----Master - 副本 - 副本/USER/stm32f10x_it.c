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

u16 USART2_BUF_Receive[32];//�洢����2�Ļ�������// Device header
u16  RX_CNT=0;//���ռ�����
//TIM7��ʱ�жϷ�����
void TIM2_IRQHandler() //��ʱ��7��ʼ��---���ܣ��жϴӻ����ص������Ƿ�������
{ 
   if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
        {
             
           TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//����жϱ�־
           TIM_Cmd(TIM2,DISABLE);//ֹͣ��ʱ��
        }	
}


//2�������һ������
u16 PM_DATA;
void USART2_IRQHandler(void)    //�жϷ�����
{ 
	  u8 res;
    u8 err;
	  u8 j;
	  u16 check_PM_sum;
   if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
        {     
					
                if(USART_GetFlagStatus(USART2,USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE)) err=1;//��⵽������֡�����У�����
                else err=0;
                res=USART_ReceiveData(USART2); //�����յ����ֽڣ�ͬʱ��ر�־�Զ����
                
                if((RX_CNT<32)&&(err==0))
                {
                        USART2_BUF_Receive[RX_CNT]=res;
                        RX_CNT++;
                        
                        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//�����ʱ������ж�
                        TIM_SetCounter(TIM2,0);//�����յ�һ���µ��ֽڣ�����ʱ��7��λΪ0�����¼�ʱ
                        TIM_Cmd(TIM2,ENABLE);//��ʼ��ʱ
                }	
			
                if( RX_CNT == 32 ) 
			         {
					      if((USART2_BUF_Receive[0]==0x42)&&(USART2_BUF_Receive[1]==0x4d))
				   	      {						
							    for(j=0;j<30;j++)
							      {						
									    check_PM_sum +=USART2_BUF_Receive[j]; 
							       }					
							         if(check_PM_sum==((USART2_BUF_Receive[30]<<8)+USART2_BUF_Receive[31]) ) //�ж�У��λ��30,31ΪУ���
									    PM_DATA=((USART2_BUF_Receive[6]<<8)+USART2_BUF_Receive[7]);   //PM2.5��ֵ
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
