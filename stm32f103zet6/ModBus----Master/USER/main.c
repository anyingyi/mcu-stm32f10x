#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
#include "timer.h"
#include "master.h"



 int main(void)
 {

	SystemInit();
	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	KEY_Init();
	LED_Init();
	Modbus_RegMap();
	RS485_Init();
	Timer4_enable(25000);	//250ms	 	 
	while(1);
	 
 }

