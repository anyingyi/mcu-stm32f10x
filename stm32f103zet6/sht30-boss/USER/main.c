
#include"LED.h"
#include"delay.h"

 void Delay(u32 count)
 {
   u32 i=0;
   for(;i<count;i++);
 }

 int main(void)
 {	
   led_init();
	 delay_init();
	 
	 
	 while(1)
	 {
		 GPIO_SetBits(GPIOB,GPIO_Pin_5);
		 Delay(300000);
		 GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		 Delay(300000);
	 }
 }
