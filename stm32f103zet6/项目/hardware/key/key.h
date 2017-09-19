#ifndef key_h
#define key_h
#include"sys.h"

#define ADR_0 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)
#define ADR_1 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)
#define ADR_2 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)
#define ADR_3 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)
#define ADR_4 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)
#define value ADR_1|ADR_2<<1|ADR_3<<2|ADR_4<<3

void key_init();

#endif