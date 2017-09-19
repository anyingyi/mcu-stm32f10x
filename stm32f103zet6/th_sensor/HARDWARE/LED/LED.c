#include"LED.h"
#include"stm32f10x.h"

void led_init(void)
 {
	 GPIO_InitTypeDef GPIO_InitType;
	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);// ±÷” πƒ‹
	 
	 GPIO_InitType.GPIO_Mode=GPIO_Mode_Out_PP;
	 GPIO_InitType.GPIO_Pin=GPIO_Pin_5;
	 GPIO_InitType.GPIO_Speed=GPIO_Speed_50MHz;
	 
	 //GPIO_SetBits(GPIOB,GPIO_Pin_5);
	 
	 GPIO_Init(GPIOB,&GPIO_InitType);
}
 
