#include"th_sensor.h"

u16  rh_value=64,t_value;

/*CRC*/
#define POLYNOMIAL             0x131           // P(x) = x^8 + x^5 + x^4 + 1 = 100110001






u8 SHT3X_CalcCrc(u8 *data, u8 nbrOfBytes)
{
	u8 bit;        // bit mask
	u8 crc = 0xFF; // calculated checksum
	u8 byteCtr;    // byte counter

  for(byteCtr = 0; byteCtr < nbrOfBytes; byteCtr++) 
	{
			crc ^= (data[byteCtr]);
			for(bit = 8; bit > 0; --bit) 
			{
					if(crc & 0x80) {
							crc = (crc << 1) ^ POLYNOMIAL;
					}  
					else {
							crc = (crc << 1);
					}
			 }
   }
	return crc;
}


u8 CRC_Check(u8 *puchMsg, u8 usDataLen, u8 a)
{
	u8 crc;
	crc=CRC_Compute(puchMsg, usDataLen);
  if(a==crc)
    return(1);
  else
    return(0);		
}
///////////////////////////////////////////////////////////////////////////////////////////


void sht30_basic(u16 cmd)
{
	iic_start();
	iic_write_byte(write_addr);
	iic_wait_ack();
	iic_write_byte(cmd>>8); //高8位
	iic_wait_ack();
	iic_write_byte(cmd); //低8位
	iic_wait_ack();
}

//写命令
void sht30_cmd(u16 cmd)
{
	sht30_basic(cmd);
	iic_stop();
}

//初始化为周期数据采集模式
void sht30_init(void)
{
	delay_ms(100);
	sht30_cmd(0x2032);
	delay_ms(100);
}

//读寄存器状态
void sht30_read_status(u8 *status)
{
	sht30_basic(0xf32d);
	iic_start();
	iic_write_byte(read_addr);
	iic_wait_ack();
	status[0]=iic_read_byte(1);
	status[1]=iic_read_byte(1);
	status[2]=iic_read_byte(0);
	iic_stop();
}

//读结果
void sht30_read_result(u16 cmd,u8 *p)
{
	sht30_basic(cmd);
	iic_start();
	iic_write_byte(read_addr);
	iic_wait_ack();
	p[0]=iic_read_byte(1);
	p[1]=iic_read_byte(1);
	p[2]=iic_read_byte(1);
	p[3]=iic_read_byte(1);
	p[4]=iic_read_byte(1);
	p[5]=iic_read_byte(0);
	iic_stop();
}

//温湿度计算公式
u16 sht30_calc_t(u16 b)
{
	return(((float)b/65535)*175-45);
}

u16 sht30_calc_rh(u16 a)
{
	return(((float)a/65535)*100);
}



u8 q[6];

//
void sht30_read_data()
{   
	u8 a,b;
	sht30_read_result(0xe000,q);
	//if(CRC_Check( q,  2, q[3]))
	
	  a=sht30_calc_t((u16)q[0]<<8|q[1]);
	if(a!=130)
	  t_value=a;
	  b=sht30_calc_rh((u16)q[3]<<8|q[4]);
  if(b!=100)	
		rh_value=b;
}






