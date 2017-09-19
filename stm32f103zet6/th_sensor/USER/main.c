#include"sys.h"
#include "TemAndHum.h"
#include "usart.h"	



u8 ezi2cBuf[4];

int main(void)
{
	delay_init();
	SHT_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	iic_init();
	uart_init(9600);
	while(1)
	{
		USART_SendData(USART1,Tem_Value);
		USART_SendData(USART1,RH_Value);
		delay_ms(500);
	}
    //CyGlobalIntEnable; /* Enable global interrupts. */
    
//    ezi2cBuf[0] = 0x0B;
//    
//    EZI2C_Start();
//    EZI2C_EzI2CSetBuffer1(sizeof(ezi2cBuf),sizeof(ezi2cBuf),ezi2cBuf); 
//    
//    SHT_Init();

//    for(;;)
//    {
//        if( ++delay >= 100 )
//        {
//            delay = 0;
//            SHT_GetValue();
//            ezi2cBuf[1] = Tem_Value;
//            ezi2cBuf[2] = RH_Value;
//        }
//        
//        CyDelay(15);
//    }
}


