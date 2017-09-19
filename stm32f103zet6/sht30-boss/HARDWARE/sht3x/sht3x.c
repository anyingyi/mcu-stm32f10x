//==============================================================================
// =============(C) COPYRIGHT 2017 杭州卡丽智能科技股份有限公司=================
//==============================================================================
// Project   :  sht3x Sample Code (V1.0)
// File      :  sht3x.c (V1.0)
// Author    :  RFU
// Date      :  1-6-2017
// Controller:  
// IDE       :  
// Compiler  :  王晶东
// Brief     :  Sensor Layer: Implementation of functions for sensor access.
//==============================================================================

//-- Includes ------------------------------------------------------------------
#include "sht3x.h"
#include "i2c_hal.h"

//-- Defines -------------------------------------------------------------------
// CRC
#define POLYNOMIAL  0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001


//------------------------------------------------------------------------------
// IO-Pins                             /* -- adapt the defines for your uC -- */
//------------------------------------------------------------------------------
// Reset on port B, bit 12
//#define RESET_LOW()  (GPIOB->BSRR = 0x10000000) // set Reset to low
//#define RESET_HIGH() (GPIOB->BSRR = 0x00001000) // set Reset to high

// Alert on port B, bit 10
//#define ALERT_READ   (GPIOB->IDR  & 0x0400)     // read Alert
//------------------------------------------------------------------------------

//-- Global variables ----------------------------------------------------------
unsigned char _i2cWriteHeader;
unsigned char _i2cReadHeader;
unsigned char m_temp_time = 0;

//==============================================================================
void sht3x_Init(unsigned char i2cAdr){              /* -- adapt the init for your uC -- */
	//==============================================================================
	//etError error; // error code
	// init I/O-pins
	//RCC->APB2ENR |= 0x00000008;  // I/O port B clock enabled
	
	// Alert on port B, bit 10
	//GPIOB->CRH   &= 0xFFFFF0FF;  // set floating input for Alert-Pin
	//GPIOB->CRH   |= 0x00000400;  //
	
	// Reset on port B, bit 12
	//GPIOB->CRH   &= 0xFFF0FFFF;  // set push-pull output for Reset pin
	//GPIOB->CRH   |= 0x00010000;  //
	//RESET_LOW();
  
	I2c_Init( SHTx ); // init I2C
	sht3x_SetI2cAdr( i2cAdr );
	
	// release reset
	//RESET_HIGH();
}

//==============================================================================
void sht3x_SetI2cAdr(unsigned char i2cAdr){
//==============================================================================
	_i2cWriteHeader = i2cAdr << 1;
	_i2cReadHeader = _i2cWriteHeader | 0x01;
}

//==============================================================================
etError sht3x_ReadSerialNumber(unsigned int *serialNbr){
//==============================================================================
	etError error; // error code
	unsigned short serialNumWords[2];
	
	error = sht3x_StartWriteAccess();
	
	// write "read serial number" command
	error |= sht3x_WriteCommand(CMD_READ_SERIALNBR);
	// if no error, start read access
	if(error == NO_ERROR) error = sht3x_StartReadAccess();
	// if no error, read first serial number word
	if(error == NO_ERROR) error = sht3x_Read2BytesAndCrc(&serialNumWords[0], ACK, 100);
	// if no error, read second serial number word
	if(error == NO_ERROR) error = sht3x_Read2BytesAndCrc(&serialNumWords[1], NACK, 0);
	
	sht3x_StopAccess();
	
	// if no error, calc serial number as 32-bit integer
	if(error == NO_ERROR)
	{
		*serialNbr = (serialNumWords[0] << 16) | serialNumWords[1];
	}
	
	return error;
}

//==============================================================================
etError sht3x_ReadStatus(unsigned short *status){
//==============================================================================
	etError error; // error code
	
	error = sht3x_StartWriteAccess();
	
	// if no error, write "read status" command
	if(error == NO_ERROR) error = sht3x_WriteCommand(CMD_READ_STATUS);
	// if no error, start read access
	if(error == NO_ERROR) error = sht3x_StartReadAccess(); 
	// if no error, read status
	if(error == NO_ERROR) error = sht3x_Read2BytesAndCrc(status, NACK, 0);
	
	sht3x_StopAccess();
	
	return error;
}

//==============================================================================
etError sht3x_ClearAllAlertFlags(void){
//==============================================================================
	etError error; // error code
	
	error = sht3x_StartWriteAccess();
	
	// if no error, write clear status register command
	if(error == NO_ERROR) error = sht3x_WriteCommand(CMD_CLEAR_STATUS);
	
	sht3x_StopAccess();
	
	return error;
}

//==============================================================================
etError sht3x_GetTempAndHumi(float         *temp,
                             float         *humi,
                             etRepeatab repeatab,
                             etMode     mode,
                             unsigned char        timeout){
//==============================================================================
	etError error;
							   
	switch(mode)
	{    
	case MODE_CLKSTRETCH: // get temperature with clock stretching mode
	  error = sht3x_GetTempAndHumiClkStretch(temp, humi, repeatab, timeout); break;
	case MODE_POLLING:    // get temperature with polling mode
	  error = sht3x_GetTempAndHumiPolling(temp, humi, repeatab, timeout); break;
	default:              
	  error = PARM_ERROR; break;
	}
	
	return error;
}


//==============================================================================
etError sht3x_GetTempAndHumiClkStretch(float         *temp,
                                       float         *humi,
                                       etRepeatab repeatab,
                                       unsigned char        timeout){
//==============================================================================
	etError error;        // error code
	unsigned short    rawValueTemp; // temperature raw value from sensor
	unsigned short    rawValueHumi; // humidity raw value from sensor
	
	error = sht3x_StartWriteAccess();
	
	// if no error ...
	if(error == NO_ERROR)
	{
	// start measurement in clock stretching mode
	// use depending on the required repeatability, the corresponding command
	switch(repeatab)
	{
	  case REPEATAB_LOW:    error = sht3x_WriteCommand(CMD_MEAS_CLOCKSTR_L); break;
	  case REPEATAB_MEDIUM: error = sht3x_WriteCommand(CMD_MEAS_CLOCKSTR_M); break;
	  case REPEATAB_HIGH:   error = sht3x_WriteCommand(CMD_MEAS_CLOCKSTR_H); break;
	  default:              error = PARM_ERROR; break;
	}
	}
	
	// if no error, start read access
	if(error == NO_ERROR) error = sht3x_StartReadAccess();
	// if no error, read temperature raw values
	if(error == NO_ERROR) error = sht3x_Read2BytesAndCrc(&rawValueTemp, ACK, timeout);
	// if no error, read humidity raw values
	if(error == NO_ERROR) error = sht3x_Read2BytesAndCrc(&rawValueHumi, NACK, 0);
	
	sht3x_StopAccess();
	
	// if no error, calculate temperature in 癈 and humidity in %RH
	if(error == NO_ERROR)
	{
	*temp = sht3x_CalcTemperature(rawValueTemp);
	*humi = sht3x_CalcHumidity(rawValueHumi);
	}
	
	return error;
}

//==============================================================================
etError sht3x_GetTempAndHumiPolling(float         *temp,
                                    float         *humi,
                                    etRepeatab repeatab,
                                    unsigned char        timeout){
//==============================================================================
	etError error;           // error code
	unsigned short    rawValueTemp;    // temperature raw value from sensor
	unsigned short    rawValueHumi;    // humidity raw value from sensor
	
	error  = sht3x_StartWriteAccess();
	
	// if no error ...
	if(error == NO_ERROR)
	{
	// start measurement in polling mode
	// use depending on the required repeatability, the corresponding command
	switch(repeatab)
	{
	  case REPEATAB_LOW:    error = sht3x_WriteCommand(CMD_MEAS_POLLING_L); break;
	  case REPEATAB_MEDIUM: error = sht3x_WriteCommand(CMD_MEAS_POLLING_M); break;
	  case REPEATAB_HIGH:		error = sht3x_WriteCommand(CMD_MEAS_POLLING_H); break;
	  default:         			error = PARM_ERROR; break;
	}
	}
	
	// if no error, wait until measurement ready
	if(error == NO_ERROR)
	{
	// poll every 1ms for measurement ready until timeout
	while(timeout--)
	{
	  // check if the measurement has finished
	  error = sht3x_StartReadAccess();
	
	  // if measurement has finished -> exit loop
	  if(error == NO_ERROR) break;
	
	  // delay 1ms
	  DelayMicroSeconds(1000);
	}
		
		// check for timeout error
		if(timeout == 0) error = TIMEOUT_ERROR;
	}
	
	// if no error, read temperature and humidity raw values
	if(error == NO_ERROR)
	{
	error |= sht3x_Read2BytesAndCrc(&rawValueTemp, ACK, 0);
	error |= sht3x_Read2BytesAndCrc(&rawValueHumi, NACK, 0);
	}
	
	sht3x_StopAccess();
	
	// if no error, calculate temperature in 癈 and humidity in %RH
	if(error == NO_ERROR)
	{
	*temp = sht3x_CalcTemperature(rawValueTemp);
	*humi = sht3x_CalcHumidity(rawValueHumi);
	}
	
	return error;
}

//==============================================================================
etError sht3x_StartPeriodicMeasurment(etRepeatab  repeatab,
                                      etFrequency freq){
//==============================================================================
	etError error;        // error code
	
	error = sht3x_StartWriteAccess();
	
	// if no error, start periodic measurement 
	if(error == NO_ERROR)
	{
	// use depending on the required repeatability and frequency,
	  // the corresponding command
		switch(repeatab)
		{
			case REPEATAB_LOW: // low repeatability
				switch(freq)
				{
					case FREQUENCY_HZ5:  // low repeatability,  0.5 Hz
						error |= sht3x_WriteCommand(CMD_MEAS_PERI_05_L); break;					
					case FREQUENCY_1HZ:  // low repeatability,  1.0 Hz
						error |= sht3x_WriteCommand(CMD_MEAS_PERI_1_L); break;					
					case FREQUENCY_2HZ:  // low repeatability,  2.0 Hz
						error |= sht3x_WriteCommand(CMD_MEAS_PERI_2_L); break;					
					case FREQUENCY_4HZ:  // low repeatability,  4.0 Hz
						error |= sht3x_WriteCommand(CMD_MEAS_PERI_4_L); break;					
					case FREQUENCY_10HZ: // low repeatability, 10.0 Hz
						error |= sht3x_WriteCommand(CMD_MEAS_PERI_10_L); break;					
					default:
						error |= PARM_ERROR; break;
				}
			  break;
				
			case REPEATAB_MEDIUM: // medium repeatability
				switch(freq)
				{
					case FREQUENCY_HZ5:  // medium repeatability,  0.5 Hz
						error |= sht3x_WriteCommand(CMD_MEAS_PERI_05_M); break;
					case FREQUENCY_1HZ:  // medium repeatability,  1.0 Hz
						error |= sht3x_WriteCommand(CMD_MEAS_PERI_1_M); break;				
					case FREQUENCY_2HZ:  // medium repeatability,  2.0 Hz
						error |= sht3x_WriteCommand(CMD_MEAS_PERI_2_M); break;				
					case FREQUENCY_4HZ:  // medium repeatability,  4.0 Hz
						error |= sht3x_WriteCommand(CMD_MEAS_PERI_4_M); break;			
					case FREQUENCY_10HZ: // medium repeatability, 10.0 Hz
						error |= sht3x_WriteCommand(CMD_MEAS_PERI_10_M); break;
					default:
						error |= PARM_ERROR; break;
				}
			  break;
				
			case REPEATAB_HIGH: // high repeatability
				switch(freq)
				{
					case FREQUENCY_HZ5:  // high repeatability,  0.5 Hz
						error |= sht3x_WriteCommand(CMD_MEAS_PERI_05_H); break;
					case FREQUENCY_1HZ:  // high repeatability,  1.0 Hz
						error |= sht3x_WriteCommand(CMD_MEAS_PERI_1_H); break;
					case FREQUENCY_2HZ:  // high repeatability,  2.0 Hz
						error |= sht3x_WriteCommand(CMD_MEAS_PERI_2_H); break;
					case FREQUENCY_4HZ:  // high repeatability,  4.0 Hz
						error |= sht3x_WriteCommand(CMD_MEAS_PERI_4_H); break;
					case FREQUENCY_10HZ: // high repeatability, 10.0 Hz
						error |= sht3x_WriteCommand(CMD_MEAS_PERI_10_H); break;
					default:
						error |= PARM_ERROR; break;
				}
			  break;
			default:
				error |= PARM_ERROR; break;
		}
	}
	
	sht3x_StopAccess();
	
	return error;
}

//==============================================================================
etError sht3x_ReadMeasurementBuffer(short *temp, short *humi){
//==============================================================================
	etError  error;        // error code
	unsigned short     rawValueTemp; // temperature raw value from sensor
	unsigned short     rawValueHumi; // humidity raw value from sensor
	
	error = sht3x_StartWriteAccess();
	
	// if no error, read measurements
	if(error == NO_ERROR)	error = sht3x_WriteCommand(CMD_FETCH_DATA);
	if(error == NO_ERROR)	error = sht3x_StartReadAccess();	
	if(error == NO_ERROR)	error = sht3x_Read2BytesAndCrc(&rawValueTemp, ACK, 0);
	if(error == NO_ERROR) 	error = sht3x_Read2BytesAndCrc(&rawValueHumi, NACK, 0);
	
	// if no error, calculate temperature in 癈 and humidity in %RH
	if(error == NO_ERROR)
	{
		*temp = sht3x_CalcTemperature(rawValueTemp);
		*humi = sht3x_CalcHumidity(rawValueHumi);
	}
	
	sht3x_StopAccess();
	
	return error;
}

//==============================================================================
etError sht3x_EnableHeater(void){
//==============================================================================
	etError error; // error code
	
	error = sht3x_StartWriteAccess();
	
	// if no error, write heater enable command
	if(error == NO_ERROR) error = sht3x_WriteCommand(CMD_HEATER_ENABLE);
	
	sht3x_StopAccess();
	
	return error;
	}

//==============================================================================
etError sht3x_DisbaleHeater(void){
//==============================================================================
	etError error; // error code
	
	error = sht3x_StartWriteAccess();
	
	// if no error, write heater disable command
	if(error == NO_ERROR) error = sht3x_WriteCommand(CMD_HEATER_DISABLE);
	
	sht3x_StopAccess();
	
	return error;
}

//==============================================================================
etError sht3x_SofloatReset(void){
//==============================================================================
	etError error; // error code
	
	error = sht3x_StartWriteAccess();
	
	// write reset command
	error |= sht3x_WriteCommand(CMD_SOfloat_RESET);
	
	sht3x_StopAccess();
	
	// if no error, wait 50 ms afloater reset
	if(error == NO_ERROR) DelayMicroSeconds(50000); 
	
	return error;
}

//==============================================================================
/*void sht3x_HardReset(void){
//==============================================================================
	// set reset low
	RESET_LOW();
	
	// wait 100 ms
	DelayMicroSeconds(100000);
	
	// release reset
	RESET_HIGH();
	
	// wait 50 ms afloater reset
	DelayMicroSeconds(50000);
}*/

//==============================================================================
etError sht3x_StartWriteAccess(void){
//==============================================================================
	etError error; // error code
	
	// write a start condition
	I2c_StartCondition( SHTx );
	
	// write the sensor I2C address with the write flag
	error = I2c_WriteByte(SHTx, _i2cWriteHeader);
	
	return error;
}

//==============================================================================
etError sht3x_StartReadAccess(void){
//==============================================================================
	etError error; // error code
	
	// write a start condition
	I2c_StartCondition( SHTx );
	
	// write the sensor I2C address with the read flag
	error = I2c_WriteByte(SHTx, _i2cReadHeader);
	
	return error;
}

//==============================================================================
void sht3x_StopAccess(void){
//==============================================================================
	// write a stop condition
	I2c_StopCondition( SHTx );
}

//==============================================================================
etError sht3x_WriteCommand(etCommands cmd){
//==============================================================================
	etError error; // error code
	
	// write the upper 8 bits of the command to the sensor
	error  = I2c_WriteByte(SHTx, cmd >> 8);
	
	// write the lower 8 bits of the command to the sensor
	error |= I2c_WriteByte(SHTx, cmd & 0xFF);
	
	return error;
}

//==============================================================================
etError sht3x_Read2BytesAndCrc(unsigned short *data, etI2cAck finaleAckNack, unsigned char timeout){
//==============================================================================
	etError error;    // error code
	unsigned char     bytes[2]; // read data array
	unsigned char     checksum; // checksum byte
	
	// read two data bytes and one checksum byte
						  error = I2c_ReadByte(SHTx, &bytes[0], ACK, timeout);
	if(error == NO_ERROR) error = I2c_ReadByte(SHTx, &bytes[1], ACK, 0);
	if(error == NO_ERROR) error = I2c_ReadByte(SHTx, &checksum, finaleAckNack, 0);
	
	// verify checksum
	if(error == NO_ERROR) error = sht3x_CheckCrc(bytes, 2, checksum);
	
	// combine the two bytes to a 16-bit value
	*data = (bytes[0] << 8) | bytes[1];
	
	return error;
}

//==============================================================================
etError sht3x_Write2BytesAndCrc(unsigned short data){
//==============================================================================
	etError error;    // error code
	unsigned char     bytes[2]; // read data array
	unsigned char     checksum; // checksum byte
	
	bytes[0] = data >> 8;
	bytes[1] = data & 0xFF;
	checksum = sht3x_CalcCrc(bytes, 2);
	
	// write two data bytes and one checksum byte
						  error = I2c_WriteByte(SHTx, bytes[0]); // write data MSB
	if(error == NO_ERROR) error = I2c_WriteByte(SHTx, bytes[1]); // write data LSB
	if(error == NO_ERROR) error = I2c_WriteByte(SHTx, checksum); // write checksum
	
	return error;
}

//==============================================================================
unsigned char sht3x_CalcCrc(unsigned char data[], unsigned char nbrOfBytes){
//==============================================================================
	unsigned char bit;        // bit mask
	unsigned char crc = 0xFF; // calculated checksum
	unsigned char byteCtr;    // byte counter
	
	// calculates 8-Bit checksum with given polynomial
	for(byteCtr = 0; byteCtr < nbrOfBytes; byteCtr++)
	{
		crc ^= (data[byteCtr]);
		for(bit = 8; bit > 0; --bit)
		{
	  		if(crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
	  		else           crc = (crc << 1);
		}
	}
	
	return crc;
}

//==============================================================================
etError sht3x_CheckCrc(unsigned char data[], unsigned char nbrOfBytes, unsigned char checksum){
//==============================================================================
	unsigned char crc;     // calculated checksum
	
	// calculates 8-Bit checksum
	crc = sht3x_CalcCrc(data, nbrOfBytes);
	
	// verify checksum
	if(crc != checksum) return CHECKSUM_ERROR;
	else                return NO_ERROR;
}

//==============================================================================
short sht3x_CalcTemperature(unsigned short rawValue){
//==============================================================================
  // calculate temperature [癈]
  // T = -45 + 175 * rawValue / (2^16-1)
  //return (175 * (float)rawValue / 65535 - 45);
  //return (short)((175 * (float)rawValue / 65535 - 45)*100+0.5);
  return (short)(0.267033 * (float)rawValue  - 4500 + 0.5);
}

//==============================================================================
short sht3x_CalcHumidity(unsigned short rawValue){
//==============================================================================
	// calculate relative humidity [%RH]
	// RH = rawValue / (2^16-1) * 100
    //return (100 * (float)rawValue / 65535);
  	//return (short)(10000 * (float)rawValue / 65535 + 0.5);
	return (short)(0.152590 * (float)rawValue + 0.5);
}

//==============================================================================
unsigned short sht3x_CalcRawTemperature(float temperature){
//==============================================================================
	// calc raw value from a temperature [癈]
	// rawValue = (T + 45) / 175 * (2^16-1)
	return (unsigned short)((temperature + 45) / 175 * 65535);
}

//==============================================================================
unsigned short sht3x_CalcRawHumidity(float humidity){
//==============================================================================
	// calc raw value from a relative humidity [%RH]
	// rawValue = RH / 100 * (2^16-1)
	return (unsigned short)(humidity / 100 * 65535);
}


//==============================================================================
/*bt sht3x_ReadAlert(void){
//==============================================================================
	// read alert pin
	return (ALERT_READ != 0) ? TRUE : FALSE;
}*/

unsigned char sht3x_GetTempAndHumiPeriod( short *temp, short *humi )
{
	unsigned char   error = 0;       // error code
	unsigned short status;
	
	error = sht3x_ReadStatus(&status);

	// check if the reset bit is set after a reset or power-up
	if( (status & 0x0010) == 0x0010 )
	{
		// clear reset flag
		error = sht3x_ClearAllAlertFlags();
		//DelayMicroSeconds(10);
		//start periodic measurement, with high repeatability and 1 measurements per second
		error = sht3x_StartPeriodicMeasurment(REPEATAB_HIGH, FREQUENCY_HZ5);
	}
	error = sht3x_ReadMeasurementBuffer(temp, humi);	
	
	return error;
}


//==============================================================================

//==============================================================================
void sht3x_read( void )
{
 	signed short m_temp, m_humi;
	
	m_temp_time++;
	if( m_temp_time == READ_TIME )
	{
		sht3x_GetTempAndHumiPeriod( &m_temp, &m_humi );
		//温湿度传感器故障
		if( (m_temp == 0) && (m_humi == 0) )
		{
		  	m_RunData.temp = 0;
			m_RunData.humi = 0;
            m_RunData.status |= 0x06;
            m_RunData.warnsensor |= 0x02;    
		}
		else
		{
		  	m_RunData.temp = m_temp;
			m_RunData.humi = m_humi;
            m_RunData.status &= ~0x06;
            m_RunData.warnsensor &= ~0x02;  
		}
		m_temp_time = 0;
	}		
}

// =============(C) COPYRIGHT 2017 杭州卡丽智能科技股份有限公司=================