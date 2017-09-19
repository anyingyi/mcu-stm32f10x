#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	
#include "lcd.h"  
#include "key.h"      
#include "sram.h"
#include "protocol.h" 
#include "malloc.h" 
#include "beep.h" 
//ALIENTEKս��STM32������ ʵ��x
//MODBUS ʵ�� 
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 
 
//��ʾmodbus��ز���
//fx:ָ֡��
void modbus_show_result(m_frame_typedef * fx)
{
	u8 buf[100];
	sprintf((char*)buf,"%02X",fx->address);
	LCD_ShowString(30+40,190,200,16,16,buf);
	sprintf((char*)buf,"%02X",fx->function);
	LCD_ShowString(30+40,210,200,16,16,buf); 
	sprintf((char*)buf,"%02X",fx->count);
	LCD_ShowString(30+40,230,200,16,16,buf); 
	sprintf((char*)buf,"%02X",fx->datalen);
	LCD_ShowString(30+40,250,200,16,16,buf); 
	sprintf((char*)buf,"%02X %02X %02X %02X %02X",fx->data[0],fx->data[1],fx->data[2],fx->data[3],fx->data[4]);
	LCD_ShowString(30+40,270,200,16,16,buf);    
}


//ִ����ؿ���
//fx:ָ֡��
void modbus_action(m_frame_typedef * fx)
{
	if(fx->address==0X01)
	{
		if(fx->datalen)
		{
			if(fx->data[0]==0X00)BEEP=!BEEP;	//���Ʒ�������
			if(fx->data[1]==0X01)LED1=!LED1;	//����DS0��˸
			//......
		}
	}
}

const u8* checkname[4]={"CHECK: SUM ","CHECK: XOR ","CHECK:CRC8 ","CHECK:CRC16"};
	
int main(void)
{		
	u8 check=0;
	u8 i;
	u8 res; 
	m_frame_typedef rxframe;
	
 	Stm32_Clock_Init(9);		//ϵͳʱ������
	uart_init(72,115200);		//���ڳ�ʼ��Ϊ115200
	BEEP_Init();
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		//��ʼ���ⲿ�ڴ��
	res=mb_init(0);
	mp_init(36,115200);
	delay_init(72);	   	 		//��ʱ��ʼ��  
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					//��ʼ������
	LCD_Init();			   		//��ʼ��LCD   
 	FSMC_SRAM_Init();			//��ʼ���ⲿSRAM  
	POINT_COLOR=RED;			//��������Ϊ��ɫ 
	
	LCD_ShowString(30,50,200,16,16,"WarShip STM32");	
	LCD_ShowString(30,70,200,16,16,"MODBUS TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2017/8/17"); 
	LCD_ShowString(30,130,200,16,16,"Press KEY_UP to set check��"); 
	
	if(res==0)LCD_ShowString(30,150,200,16,16,"MODBUS OK");
	else LCD_ShowString(30,150,200,16,16,"MODBUS Failed");  
	LCD_ShowString(30,190,200,16,16,"ADDR:"); 
	LCD_ShowString(30,210,200,16,16,"FUNC:"); 
	LCD_ShowString(30,230,200,16,16," CNT:"); 
	LCD_ShowString(30,250,200,16,16," LEN:"); 
	LCD_ShowString(30,270,200,16,16,"DATA:"); 	
	POINT_COLOR=BLUE;			//��������Ϊ��ɫ
	LCD_ShowString(30,170,200,16,16,"CHECK:SUM"); //��ʾ��ǰCRCУ�鷽��
  	while(1)
	{	 	
		if(m_ctrl_dev.frameok)
		{
			res=mb_unpack_frame(&rxframe);
			if(res==MR_OK)	//����ɹ�
			{
				mb_packsend_frame(&rxframe);
				modbus_show_result(&rxframe);
				modbus_action(&rxframe);
				if(rxframe.datalen)myfree(SRAMIN,rxframe.data);	//�ͷ�֮ǰ���ڴ�

			}else 
			{
				LCD_Fill(30+40,190,239,310,WHITE);
				LCD_ShowString(30+40,270,200,16,16,"FRAME ERROR");
			};
		}else delay_ms(5);
		res=KEY_Scan(0);
		if(res==WKUP_PRES)//��KEY UP�л�У��ģʽ
		{
			check++;
			if(check>3)check=0;
			LCD_ShowString(30,170,200,16,16,(u8*)checkname[check]); //��ʾ��ǰCRCУ�鷽��
			mb_destroy();		//ɾ��֮ǰ������
			mb_init(check);		//��������CRCУ�鷽��
		}
		i++;
		if(i==100)LED0=!LED0;	
	}	   
}










