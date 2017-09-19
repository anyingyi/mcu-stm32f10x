#ifndef _MODBUS_H
#define _MODBUS_H
#include "sys.h"

//#define tx {GPIOA->CRH&=0x0FFFFFFF;GPIOB->CRH|=0x00000030;}  //Õ∆ÕÏ
//#define series {GPIOA->CRH&=0x0FFFFFFF;GPIOB->CRH|=0x000000b0;}  //∏¥”√Õ∆ÕÏ
#define RS485_TX_EN PDout(7) 




void RS485_Init(void);
void Timer7_Init(void);
void RS485_Service(void);
void Modbus_02_Solve(void);
void Modbus_01_Solve(void);
void Modbus_05_Solve(void);
void Modbus_15_Solve(void);
void Modbus_03_Solve(void);
void Modbus_06_Solve(void);
void Modbus_16_Solve(void);

#endif
