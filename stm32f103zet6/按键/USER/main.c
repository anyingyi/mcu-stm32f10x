#include"button.h"
#include"delay.h"
#include"LED.h"
#include"beep.h"

int main()
{
	u8 key;
	
	delay_init();
	button_init();
	led_init();
	BEEP_Init();
	
	while(1)
	{
		key=button();
		
		switch(key)
		{
			case 0:led0=!led0;break;
			case 1:led1=!led1;break;
			case 2:beep=!beep;break;
		}
	
	}
}


