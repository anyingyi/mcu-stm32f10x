#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define LED0 PBout(5)// PB5
#define LED1 PEout(5)// PE5	

#define LED_RUN PBout(5)
#define LED_COM PEout(5)	
#define LED_ERR PBout(5)
#define LED_TVOC PEout(5)
#define LED_CO2 PBout(5)
#define LED_PM2D5 PEout(5)
#define LED_TH PEout(5)

void LED_Init(void);//��ʼ��

		 				    
#endif