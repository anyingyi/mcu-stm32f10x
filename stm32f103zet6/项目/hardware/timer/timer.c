#include"timer.h"
#include"sys.h"

void timer_init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitT;
	NVIC_InitTypeDef NVIC_InitTypeD;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//定时器时钟使能
	
	/*定时器模式配置*/
	TIM_TimeBaseInitT.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitT.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitT.TIM_Period=arr;
	TIM_TimeBaseInitT.TIM_Prescaler=psc;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitT);
	
	TIM_Cmd(TIM3,ENABLE);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	/*优先级配置*/
	NVIC_InitTypeD.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitTypeD.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitTypeD.NVIC_IRQChannelPreemptionPriority=2; //抢先优先级
	NVIC_InitTypeD.NVIC_IRQChannelSubPriority=2;  //响应优先级
	NVIC_Init(&NVIC_InitTypeD);
	
	
}

/*中断服务函数*/
void TIM3_IRQHandler()
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET){
		PCout(13)=!PCout(13);
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}