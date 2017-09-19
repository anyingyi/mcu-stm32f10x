#include "stm32f10x.h"
#include"delay.h"



 int main(void)
 {	
		GPIO_InitTypeDef  GPIO_InitStructure;
		 
		delay_init();
		 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    //使能PB,PE端口时钟
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;			    //LED0-->PB.5 端口配置
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
		GPIO_Init(GPIOB, &GPIO_InitStructure);			     //初始化GPIOB.5
		GPIO_SetBits(GPIOB,GPIO_Pin_8);					//PB.5 输出高
  
		while(1)
		{
			PBout(8)=1;	
			delay_ms(300);
			PBout(8)=0;
			
			delay_ms(300);
		}
 }
