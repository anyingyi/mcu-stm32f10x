#ifndef computer_h
#define computer_h

#include "sys.h"

void CRS485_Init1(void);
void Timer5_Init(void);
void CRS485_Service(void);
void CModbus_02_1Solve(void);
void CModbus_01_1Solve(void);
void CModbus_05_1Solve(void);
void CModbus_15_1Solve(void);
void CModbus_03_1Solve(void);
void CModbus_06_1Solve(void);
void CModbus_16_1Solve(void);

#endif

