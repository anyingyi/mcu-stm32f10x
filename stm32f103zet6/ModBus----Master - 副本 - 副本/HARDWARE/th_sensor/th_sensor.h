#ifndef __sensor_H
#define __sensor_H	

#include"stm32f10x.h"
#include"iic.h"
#define write_addr 0x88
#define read_addr 0x89

extern  u16  rh_value,t_value;

void sht30_init(void);//≥ı ºªØ
void sht30_read_data();
void sht30_cmd(u16 cmd);
void sht30_read_status(u8 *status);
void sht30_read_result(u16 cmd,u8 *p);

		 				    
#endif