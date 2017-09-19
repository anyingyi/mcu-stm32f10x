#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "usart.h"


int main()
{
	u16 a;
	led_init();
	delay_init();
	uart_init(115200);
	while(1)
	{
		if(a==11)
		SoftReset();
	  PBout(5)=1;
		delay_ms(500);
		PBout(5)=0;
		delay_ms(500);
		printf("a=%d",a);
		a++;
	}

	
}

