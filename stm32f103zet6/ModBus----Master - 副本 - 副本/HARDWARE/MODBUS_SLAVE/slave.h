#ifndef _MODBUS_H
#define _MODBUS_H
#include "sys.h"
#include "key.h"


#if ADR_0

//#define tx {GPIOA->CRH&=0x0FFFFFFF;GPIOB->CRH|=0x00000030;}  //Õ∆ÕÏ
//#define series {GPIOA->CRH&=0x0FFFFFFF;GPIOB->CRH|=0x000000b0;}  //∏¥”√Õ∆ÕÏ
#define RS485_TX_EN PDout(7) 




void RS485_Init1(void);
void Timer6_Init(void);
void RS485_Service(void);
void Modbus_02_1Solve(void);
void Modbus_01_1Solve(void);
void Modbus_05_1Solve(void);
void Modbus_15_1Solve(void);
void Modbus_03_1Solve(void);
void Modbus_06_1Solve(void);
void Modbus_16_1Solve(void);

#endif

#endif
