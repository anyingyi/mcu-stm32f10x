#include"led.h"
#include"stm32f10x.h"

void led_init()
{
	  GPIO_InitTypeDef GPIO_Initn;
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	  
	  GPIO_Initn.GPIO_Mode=GPIO_Mode_Out_PP;
	  GPIO_Initn.GPIO_Pin=GPIO_Pin_5;
		GPIO_Initn.GPIO_Speed=GPIO_Speed_50MHz;
	  
	  GPIO_Init(GPIOB,&GPIO_Initn);
	
	
}

void SoftReset(void)
{
		__set_FAULTMASK(1);      // 关闭所有中端
		NVIC_SystemReset();// 复位
}
