#ifndef __iic_H
#define __iic_H	 
#include"sys.h"
#include"delay.h"

#define sda_in {GPIOB->CRL&=0x0FFFFFFF;GPIOB->CRL|=0x80000000;}
#define sda_out {GPIOB->CRL&=0x0FFFFFFF;GPIOB->CRL|=0x30000000;}

#define sda PBout(7)
#define sda_read GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
#define scl PBout(6)

//iic函数
void iic_init(void);//初始化
void iic_start();
void iic_stop();
u8 iic_wait_ack();
void iic_ack(void);
void iic_nack(void);
void iic_write_byte(u8 a);
u8 iic_read_byte(u8 ack);

//at24c02的函数
void at24c02_write_byte(u8 addr,u8 data);
u8 at24c02_read_byte(u8 addr);
void at24c02_write(u8 addr,u8 *data);
void at24c02_read(u8 addr);


		 				    
#endif