#include"uart.h"
#include"adc.h"
#include"key.h"
#include"delay.h"

int main()
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	myuart_init();
	key_init();
	//Adc_Init();
	delay_init();
	
	while(14==0x0f){
		USART_SendData(USART1,0x0f);
	delay_ms(500);
	}
	
}
	
