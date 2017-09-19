#include"iic.h"


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

void iic_start()
{
	sda_out;
	sda=1;
	scl=1;
	delay_us(4);
	sda=0;
	delay_us(4);
	scl=0;
}

void iic_stop()
{
	sda_out;
	sda=0;
	scl=1;
	delay_us(4);
	sda=1;
	delay_us(4);
//	scl=0;
}

//等待应答
u8 iic_wait_ack()
{
	u8 t=0;
	sda_in;
	scl=0;
	delay_us(1);
	scl=1;
	delay_us(1);
	if(sda_read)
	{
		return(1);
	}
	scl=0;
	return(0);
}

//产生ACK应答
void iic_ack(void)
{
	sda_out;
	scl=0;
	sda=0;
	delay_us(2);
	scl=1;
	delay_us(2);
	scl=0;
}
//不产生ACK应答		    
void iic_nack(void)
{
	sda_out
	scl=0;
	sda=1;
	delay_us(2);
	scl=1;
	delay_us(2);
	scl=0;
}	

//写一个字节
void iic_write_byte(u8 a)
{
	u8 i;
	sda_out;
	scl=0;
	for(i=0;i<8;i++)
	{
		sda=(a&0x80)>>7;
		a<<=1;
		scl=1;
		delay_us(1);
		scl=0;
		delay_us(1);
	}
}

//读一个字节
u8 iic_read_byte(u8 ack)
{
	u8 i,t=0;
	sda_in;
	for(i=0;i<8;i++)
	{
		scl=0;
		delay_us(1);
		scl=1;
		delay_us(1);
		t<<=1;
		if(sda_read) t|= 1;	
	}
	if(ack)
		iic_ack();
	else 
		iic_nack();
	scl=0;
	return(t);
}


/*at24c02写一个字节*/
void at24c02_write_byte(u8 addr,u8 data)
{
	iic_start();
	iic_write_byte(0xa0);
	iic_wait_ack();
	iic_write_byte(addr);
	iic_wait_ack();
	iic_write_byte(data);
	iic_wait_ack();
	iic_stop();
	delay_ms(10);
}

/*at24c02读一个字节*/
u8 at24c02_read_byte(u8 addr)
{
	u8 a;
	iic_start();
	iic_write_byte(0xa0);
	iic_wait_ack();
	iic_write_byte(addr);
	iic_wait_ack();
	iic_start();
	iic_write_byte(0xa1);
	iic_wait_ack();
	a=iic_read_byte(0);
	iic_stop();
	return(a);
}



/*at24c02写数据*/
void at24c02_write(u8 addr,u8 *data)
{
	//u8 size=sizeof(data);
	while(*data)
	{
		at24c02_write_byte(addr,*data);
		addr++;
		data++;
	}
}


extern u8  data[];

/*at24c02读数据*/
void at24c02_read(u8 addr)
{
	u8 i=0;
	while(at24c02_read_byte(addr))
	{
		data[i]=at24c02_read_byte(addr);
		addr++;
		i++;
	}
}







