#include"button.h"
#include"delay.h"


void button_init()
{
	GPIO_InitTypeDef GPIO_InitTy;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);  //gpioeʱ��ʹ��
	
	GPIO_InitTy.GPIO_Mode=GPIO_Mode_IPU;  //��������
	GPIO_InitTy.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	//GPIO_InitTy.GPIO_Speed=
	
	
	GPIO_Init(GPIOE,&GPIO_InitTy);       //io��ʼ��
}

u8  button()
{
	if(k0==0|k1==0|k2==0)
	{
		delay_ms(10);
		if(k0==0) return(0);
		else if(k1==0) return(1);
		else if(k2==0) return(2);
	}
}

