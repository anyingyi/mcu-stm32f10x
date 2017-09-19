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
//ALIENTEK战舰STM32开发板 实验x
//MODBUS 实验 
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 
 
//显示modbus相关参数
//fx:帧指针
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


//执行相关控制
//fx:帧指针
void modbus_action(m_frame_typedef * fx)
{
	if(fx->address==0X01)
	{
		if(fx->datalen)
		{
			if(fx->data[0]==0X01)BEEP=!BEEP;	//控制蜂鸣器叫
			if(fx->data[1]==0X02)LED1=!LED1;	//控制DS0闪烁
			//......
		}
	} 
}

//发送一帧数据
//fx:帧指针
void modbus_send_frame(m_frame_typedef * fx)
{
    static u8 fcnt=0;
    u8 i;
    u8 res;
    m_frame_typedef rxframe;
    fx->address=0X01;
    fx->function=0X02;
    fx->count=fcnt;
    fx->datalen=5;
    fx->data=mymalloc(SRAMIN,5);
    for(i=0;i<5;i++)
    {
        fx->data[i]=fcnt+i;
    }
    for(i=0;i<10;i++)
    {
        mb_packsend_frame(fx);
		modbus_show_result(fx);			//显示发送数据 
        delay_ms(20); 					//发送间隔
        if(m_ctrl_dev.frameok)          //解析应答数据
        {
            m_ctrl_dev.frameok=0;
            res=mb_unpack_frame(&rxframe);
			if(res==MR_OK)	            //解包成功
			{
                myfree(SRAMIN,rxframe.data);
				if(rxframe.count==fcnt)
                { 
                    fcnt++;
                    break;//帧计数器和发一样，说明响应正常
                }
			}   
        }
    } 
    myfree(SRAMIN,fx->data);
    if(i==10)LED1=0; 
    else LED1=1;
}




const u8* checkname[4]={"CHECK: SUM ","CHECK: XOR ","CHECK:CRC8 ","CHECK:CRC16"};
	
int main(void)
{		
	u8 check=0;
	u8 i;
	u8 res;  
	m_frame_typedef txframe;
	
 	Stm32_Clock_Init(9);		//系统时钟设置
	uart_init(72,115200);		//串口初始化为115200
	BEEP_Init();
	mp_init(36,115200);
	delay_init(72);	   	 		//延时初始化  
 	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	LCD_Init();			   		//初始化LCD   
 	FSMC_SRAM_Init();			//初始化外部SRAM  
	my_mem_init(SRAMIN);		//初始化内部内存池
	my_mem_init(SRAMEX);		//初始化外部内存池
	POINT_COLOR=RED;			//设置字体为红色 
	
	LCD_ShowString(30,50,200,16,16,"WarShip STM32");	
	LCD_ShowString(30,70,200,16,16,"MODBUS TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2017/8/17"); 
	LCD_ShowString(30,130,200,16,16,"Press KEY_UP to set check！"); 
	
	res=mb_init(0);
	if(res==0)LCD_ShowString(30,150,200,16,16,"MODBUS OK");
	else LCD_ShowString(30,150,200,16,16,"MODBUS Failed");  
	LCD_ShowString(30,190,200,16,16,"ADDR:"); 
	LCD_ShowString(30,210,200,16,16,"FUNC:"); 
	LCD_ShowString(30,230,200,16,16," CNT:"); 
	LCD_ShowString(30,250,200,16,16," LEN:"); 
	LCD_ShowString(30,270,200,16,16,"DATA:"); 	
	POINT_COLOR=BLUE;			//设置字体为蓝色
	LCD_ShowString(30,170,200,16,16,"CHECK:SUM"); //显示当前CRC校验方法
    
  	while(1)
	{	 
        modbus_send_frame(&txframe); //发送一帧数据，并等待返回
        delay_ms(5);
		res=KEY_Scan(0);
		if(res==WKUP_PRES)//按KEY UP切换校验模式
		{
			check++;
			if(check>3)check=0;
			LCD_ShowString(30,170,200,16,16,(u8*)checkname[check]); //显示当前CRC校验方法
			mb_destroy();		//删除之前的设置
			mb_init(check);		//重新设置CRC校验方法
		}
		i++;
		if(i==100)LED0=!LED0;	
	}	   
}










