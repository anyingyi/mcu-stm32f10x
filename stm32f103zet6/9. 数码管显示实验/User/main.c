#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "smg.h"
#include"stm32f10x.h"
/*#define a0 GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define a1 GPIO_ResetBits(GPIOA,GPIO_Pin_1)
#define a2  GPIO_ResetBits(GPIOA,GPIO_Pin_2)
#define a3 GPIO_ResetBits(GPIOA,GPIO_Pin_3)
#define a4 GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define a5 GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define a6 GPIO_ResetBits(GPIOA,GPIO_Pin_6)
#define a7 GPIO_ResetBits(GPIOA,GPIO_Pin_7)                          //{0xc0ff,0xf9ff,0xa4ff,0xb0ff,0x99ff,0x92ff,0x82ff,0xf8ff,0x80ff,0x90ff}0-9

//共阳极数码管					
u8 tube[][]={{a0,a1,a2,a3,a4,a5},   //0
          {a1,a2},                       //1
					{a0,a1,a3,a4,a6},        //2
					{a0,a1,a2,a3,a6},        //3
					{a1,a2,a5,a6},             //4
					{a0,a2,a3,a6,a5},        //5
					{a0,a2,a3,a4,a5,a6},    //6
					{a0,a1,a2},                  //7
          {a0,a1,a2,a3,a4,a5,a6},//8
					{a0,a1,a2,a3,a5,a6}     //9					

};*/				


u8 smgduan[16]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
            0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};//0~F 数码管段选数据


void tube_display(int n)
{
	
	
	int ge,shi,point;
//	shi=(int)(n*10)/100;
//	ge=(int)(n*10)%100/10;
//	point=(int)(n*10)%10;
	shi=n/10;
	ge=n%10;
	
//	GPIOF->ODR=0<<8;
//	GPIOC->ODR=~smgduan[point];
	GPIOF->ODR=0<<8;
//	delay_ms(5);
	GPIOF->ODR=0<<9;
	GPIOC->ODR=~smgduan[ge];
//	GPIOC->ODR=0<<8;
	GPIOF->ODR=1<<9;
	delay_ms(5);
	GPIOF->ODR=0<<10;
	GPIOC->ODR=~smgduan[shi];
	GPIOF->ODR=1<<10;
	delay_ms(5);
}



int main()
{
	u8 i=0;
	SysTick_Init(72);
	//LED_Init();
	SMG_Init();
	
	
	
	while(1)
	{
		for(i=0;i<9;i++)
		{
			tube_display(34);
//	 		GPIO_ResetBits(GPIOF,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10);
//	    GPIO_Write(GPIOC,~smgduan[i]); 
//		delay_ms(500);
		}
			  
	}
}
