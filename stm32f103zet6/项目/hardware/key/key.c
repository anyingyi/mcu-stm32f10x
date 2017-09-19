#include"key.h"

void key_init()
{
	
	GPIO_InitTypeDef GPIO_InitT;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA,ENABLE);//时钟使能开启
	
	GPIO_InitT.GPIO_Mode=GPIO_Mode_IPU;//初始化
	GPIO_InitT.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitT.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitT);
	
	GPIO_InitT.GPIO_Pin=GPIO_Pin_8;
	GPIO_Init(GPIOA,&GPIO_InitT);
	

}