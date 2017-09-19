#ifndef button_h
#define button_h

#include"stm32f10x.h"

#define k0 PEin(4)
#define k1 PEin(3)
#define k2 PEin(2)

void button_init();
u8  button();



#endif

