#ifndef led_h
#define led_h
#include"sys.h"

//#define LED_RUN_UP GPIO_SetBits(GPIOB,GPIO_Pin_14)
//#define LED_RUN_DOWN GPIO_ResetBits(GPIOB,GPIO_Pin_14)
//#define LED_COM_UP GPIO_SetBits(GPIOB,GPIO_Pin_13)	
//#define LED_COM_DOWN GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define LED_RUN PBout(14)
#define LED_COM PBout(13)
#define LED_ERR PBout(12)
#define LED_TVOC PAout(0)
#define LED_CO2 PBout(1)
#define LED_PM2D5 PAout(6)
#define LED_TH PBout(5)

void led_init();
void SoftReset(void);

#endif


