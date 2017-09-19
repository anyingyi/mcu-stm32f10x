#include "smg.h"



/*******************************************************************************
* �� �� ��         : SMG_Init
* ��������		   : ����ܳ�ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void SMG_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;  //����һ���ṹ�������������ʼ��GPIO
	/* ����GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOF,ENABLE);

	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_All;	  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);		/* ��ʼ��GPIO */
	GPIOC->ODR=0xff;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;	  //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOF,&GPIO_InitStructure);		/* ��ʼ��GPIO */
	GPIOF->ODR=0xffff;
	
}

