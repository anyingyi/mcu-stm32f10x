#include "sys.h"
#include "TemAndHum.h"


#define  i2cAddWrite_8bit           0x44
#define  i2cAddRead_8bit            0x45

/*CRC*/
#define POLYNOMIAL             0x131           // P(x) = x^8 + x^5 + x^4 + 1 = 100110001



/****************************************************************/
u8  Tem_Value = 0;
u8  RH_Value = 0;

u8 buffer[6];

/************************* Inside io ****************************/


//初始化
void iic_init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;				 //LED0-->PB.5 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5	
}



//void SDA_H(void)
//{
//    I2C_SDA_Write(1);
//}
//void SDA_L(void)
//{
//    I2C_SDA_Write(0);
//}
//u8 SDA_Read(void)
//{
//    return I2C_SDA_Read();
//}
//void SCL_H(void)
//{
//    I2C_SCL_Write(1);
//}
//void SCL_L(void)
//{
//    I2C_SCL_Write(0);
//}

/***************************************************************/

/***************************************************************/

void i2c_delay(void)
{
   delay_us(2);
}

u8 i2c_star(void)
{
    SDA_H();
    SCL_H();
    i2c_delay();
    SDA_L();
    i2c_delay();
    SDA_L();
    SCL_L();
    i2c_delay();
    return 0;
}

void i2c_stop(void)
{
    SCL_L();
    i2c_delay();
    SDA_L();
    i2c_delay();
    SCL_H();
    i2c_delay();
    SDA_H();
    i2c_delay();
}

void i2c_ack(void)
{
    SCL_L();
    i2c_delay();
    SDA_L();
    i2c_delay();
    SCL_H();
    i2c_delay();
    SCL_L();
    i2c_delay();
}

void i2c_noAck(void)
{
    SCL_L();
    i2c_delay();
    SDA_H();
    i2c_delay();
    SCL_H();
    i2c_delay();
    SCL_L();
    i2c_delay();
}

u8 i2c_waitAck(void)
{
    u8 t = 100;
    
    SCL_L(); 
    i2c_delay();
    SDA_H();
    i2c_delay();
    SCL_H();
    i2c_delay();
    sda_in;	
    while( SDA_Read() )
    {
        t --;
        if(t==0)
        {
           SCL_L();
            return 1;
        }
        i2c_delay();
    }

    SCL_L();
    i2c_delay();
    return 0;
}

void i2c_sendByte( u8 byte )
{
    u8 i = 8;
    while (i--) {
        SCL_L();
        i2c_delay();
        if (byte & 0x80)
				{SDA_H();}
        else
        {SDA_L();}
        byte <<= 1;
        i2c_delay();
        SCL_H();
        i2c_delay();
    }
    SCL_L();
    i2c_delay();
}

u8 i2c_readByte(void)
{
    u8 i = 8;
    u8 byte = 0;

    SDA_H();
    while (i--) 
    {
        byte <<= 1;
        SCL_L();
        i2c_delay();
        SCL_H();
        i2c_delay();
			  sda_in;
        if (SDA_Read()) 
        {
            byte |= 0x01;
        }
    }
    SCL_L();
    i2c_delay();
    return byte;
}

/******************************************************/
void SHT3X_WriteCMD(u16 cmd)
{
	i2c_star();
	i2c_sendByte(i2cAddWrite_8bit);
	i2c_waitAck();
	i2c_sendByte(cmd>>8);
	i2c_waitAck();
	i2c_sendByte(cmd);
	i2c_waitAck();
	i2c_stop();
}

/********************************************************/
void SHT3X_SetPeriodicMeasurement()
{
    SHT3X_WriteCMD(CMD_MEAS_PERI_2_H);
}

void SHT3X_ReadState(u8 *temp)
{
    i2c_star();
	i2c_sendByte(i2cAddWrite_8bit);
	i2c_waitAck();
	i2c_sendByte(0xf3);
	i2c_waitAck();
	i2c_sendByte(0X2d);
	i2c_waitAck();
    
    i2c_star();
	i2c_sendByte(i2cAddRead_8bit);
	i2c_waitAck();

	temp[0] = i2c_readByte();
	i2c_ack();
	temp[1] = i2c_readByte();
	i2c_ack();
	temp[2] = i2c_readByte();
	i2c_noAck();
    
    i2c_stop(); 
}


void SHX3X_ReadResults(u16 cmd,  u8 *p)
{
	i2c_star();
	i2c_sendByte(i2cAddWrite_8bit);
	i2c_waitAck();
	i2c_sendByte(cmd>>8);
	i2c_waitAck();
	i2c_sendByte(cmd);
	i2c_waitAck();

    i2c_star();
	i2c_sendByte(i2cAddRead_8bit);
	i2c_waitAck();

	p[0] = i2c_readByte();
	i2c_ack();
	p[1] = i2c_readByte();
	i2c_ack();
	p[2] = i2c_readByte();
	i2c_ack();
	p[3] = i2c_readByte();
	i2c_ack();
	p[4] = i2c_readByte();
	i2c_ack();
	p[5] = i2c_readByte();
	i2c_noAck();
	i2c_stop();
}

/********************************************************************************/
/********************************************************************************/


void SHT_Init(void)
{
    delay_ms(250); /* Must add delay */

    SHT3X_SetPeriodicMeasurement();
    delay_ms(150); /* Must add delay */
}


u8 SHT3X_CalcCrc(uint8_t *data, uint8_t nbrOfBytes)
{
	uint8_t bit;        // bit mask
    uint8_t crc = 0xFF; // calculated checksum
    uint8_t byteCtr;    // byte counter

    // calculates 8-Bit checksum with given polynomial
    for(byteCtr = 0; byteCtr < nbrOfBytes; byteCtr++) {
        crc ^= (data[byteCtr]);
        for(bit = 8; bit > 0; --bit) {
            if(crc & 0x80) {
                crc = (crc << 1) ^ POLYNOMIAL;
            }  else {
                crc = (crc << 1);
            }
        }
    }
	return crc;
}

u8 SHT3X_CheckCrc(uint8_t *pdata, uint8_t nbrOfBytes, uint8_t checksum)
{
    uint8_t crc;
	crc = SHT3X_CalcCrc(pdata, nbrOfBytes);// calculates 8-Bit checksum
    if(crc != checksum) 
    {   
        return 1;           
    }
    return 0;              
}

u16 SHT3X_CalcTemperature(uint16_t rawValue)
{
    // calculate temperature 
    u16 temp;
    temp = (175 * (float)rawValue / 65535 - 45) ; // T = -45 + 175 * rawValue / (2^16-1)
    return temp;
}

u8 SHT3X_CalcRH(uint16_t rawValue)
{
    // calculate relative humidity [%RH]
    u8 temp1 = (100 * (float)rawValue / 65535) ;  // RH = rawValue / (2^16-1) * 10

    return temp1;
}

void SHT_GetValue(void)
{
    u8 temp = 0;
    u16 dat;
    u8 p[3];
    
    SHX3X_ReadResults(CMD_FETCH_DATA, buffer);
    /* check tem */
    p[0] = buffer[0];
    p[1] = buffer[1];
    p[2] = buffer[2];
    temp = SHT3X_CheckCrc(p,2,p[2]);
   if( !temp ) /* value is ture */
    {
        dat = ((u16)buffer[0] << 8) | buffer[1];
        Tem_Value = SHT3X_CalcTemperature( dat );    
    }
    /* check humidity */
    p[0] = buffer[3];
    p[1] = buffer[4];
    p[2] = buffer[5];
    temp = SHT3X_CheckCrc(p,2,p[2]);
    if( !temp )
    {
        dat = ((u16)p[0] << 8) | p[1];
        RH_Value = SHT3X_CalcRH( dat ); 
    }
}







