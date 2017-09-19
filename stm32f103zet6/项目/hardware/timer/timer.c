#include"timer.h"
#include"sys.h"

void timer_init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitT;
	NVIC_InitTypeDef NVIC_InitTypeD;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//��ʱ��ʱ��ʹ��
	
	/*��ʱ��ģʽ����*/
	TIM_TimeBaseInitT.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitT.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitT.TIM_Period=arr;
	TIM_TimeBaseInitT.TIM_Prescaler=psc;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitT);
	
	TIM_Cmd(TIM3,ENABLE);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	/*���ȼ�����*/
	NVIC_InitTypeD.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitTypeD.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitTypeD.NVIC_IRQChannelPreemptionPriority=2; //�������ȼ�
	NVIC_InitTypeD.NVIC_IRQChannelSubPriority=2;  //��Ӧ���ȼ�
	NVIC_Init(&NVIC_InitTypeD);
	
	
}

/*�жϷ�����*/
void TIM3_IRQHandler()
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET){
		PCout(13)=!PCout(13);
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}