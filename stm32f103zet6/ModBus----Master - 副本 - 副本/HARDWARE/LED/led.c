#include"stm32f10x.h"
#include"led.h"

void led_init()
{
	GPIO_InitTypeDef GPIO_InitT;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA,ENABLE);//时钟使能开启
	
	GPIO_InitT.GPIO_Mode=GPIO_Mode_Out_PP;//初始化
	GPIO_InitT.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_5|GPIO_Pin_1;
	GPIO_InitT.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitT);
	
	GPIO_InitT.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIO_InitT);
	
	LED_RUN=1;
}

 
void SoftReset(void)
{
		__set_FAULTMASK(1);      // 关闭所有中端
		NVIC_SystemReset();// 复位
}
