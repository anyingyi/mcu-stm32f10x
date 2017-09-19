#ifndef __MASTER_H
#define __MASTER_H
#include "sys.h"

#define READ_COIL     01
#define READ_DI       02
#define READ_HLD_REG  03
#define READ_AI       04
#define SET_COIL      05
#define SET_HLD_REG   06
#define WRITE_COIL    15
#define WRITE_HLD_REG 16


#define HI(n) ((n)>>8)
#define LOW(n) ((n)&0xff)



#define RS485_TX_EN PDout(7)

void RS485_Init(void);
void Timer4_enable(u16 arr);

void Modbus_RegMap(void);

void Modbus_01_Solve(void);
void Modbus_02_Solve(void);
void Modbus_03_Solve(void);
void Modbus_05_Solve(void);
void Modbus_06_Solve(void);
void Modbus_15_Solve(void);
void Modbus_16_Solve(void);
	
void Timer7_Init(void);
void RS485_TX_Service(void);
void Master_Service(u8 SlaverAddr,u8 Fuction,u16 StartAddr,u16 ValueOrLenth);
void RS485_RX_Service(void);
void modbus_rtu(void);
#endif









