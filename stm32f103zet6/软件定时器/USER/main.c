#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "includes.h"
/************************************************
 ALIENTEK 精英版STM32开发板UCOS实验 
 例4-1 UCOSIII UCOSIII移植
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

//UCOSIII中以下优先级用户程序不能使用，ALIENTEK
//将这些优先级分配给了UCOSIII的5个系统内部任务
//优先级0：中断服务服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级2：定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

//任务优先级
#define START_TASK_PRIO 3
//任务堆栈大小
#define START_STK_SIZE 128
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);
 
//任务优先级
#define TASK1_TASK_PRIO 4
//任务堆栈大小
#define TASK1_STK_SIZE 128
//任务控制块
OS_TCB Task1_TaskTCB;
//任务堆栈
CPU_STK TASK1_TASK_STK[TASK1_STK_SIZE]; //typedef  unsigned  int   CPU_STK; 128*4 byte
//任务函数
void task1_task(void *p_arg);

//任务优先级
#define TASK2_TASK_PRIO 5
//任务堆栈大小
#define TASK2_STK_SIZE 128
//任务控制块
OS_TCB Task2_TaskTCB;
//任务堆栈
CPU_STK TASK2_TASK_STK[TASK2_STK_SIZE]; //typedef  unsigned  int   CPU_STK; 128*4 byte
//任务函数
void task2_task(void *p_arg);

//timer
OS_TMR tmr1;
OS_TMR tmr2;
void tmr1_callback(void *p_tmr,void *p_arg); //callback1 function
void tmr2_callback(void *p_tmr,void *p_arg); //callback2 function

//main()
int main()
{
	OS_ERR err;
	CPU_SR_ALLOC();  //调用临界函数时需调用它
	
	delay_init();
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
	LED_Init();
	uart_init(115200);
	
	OSInit(&err);
	OS_CRITICAL_ENTER(); //enter critical
	OSTaskCreate(     (OS_TCB  *)&StartTaskTCB,
                    (CPU_CHAR  *)"start_task",
                    (OS_TASK_PTR)  start_task,
                    (void *)     0,
                    (OS_PRIO)    START_TASK_PRIO,
                    (CPU_STK  *)START_TASK_STK,
                    (CPU_STK_SIZE)   START_STK_SIZE/10,
                    (CPU_STK_SIZE)   START_STK_SIZE,
                    (OS_MSG_QTY)    0,
                    (OS_TICK)       0,
                    (void    *)0,
                    (OS_OPT)    OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                    (OS_ERR   *)&err);
	OS_CRITICAL_EXIT();//EXIT CRITICAL
	OSStart(&err);
	while(1);
}


//start_task
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	CPU_Init();
	#if OS_CFG_STAT_TASK_EN>0u
	   OSStatTaskCPUUsageInit(&err); //统计任务
	#endif
	
	#ifdef CPU_CFG_INT_DIS_MEAS_EN 
	   CPU_IntDisMeasMaxCurReset();	
  #endif
	
	#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	   //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	   OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
  #endif
	
	//create timer1
	OSTmrCreate(     (OS_TMR     *)&tmr1,
                   (CPU_CHAR   *)"tmr1",
                   (OS_TICK)      20,   //20*10ms
                   (OS_TICK)       100,  //100*10ms
                   (OS_OPT)   OS_OPT_TMR_PERIODIC,
                   (OS_TMR_CALLBACK_PTR)   tmr1_callback,  //function pointer
                   (void    *)0,
                   (OS_ERR  *)&err);
									 
	//create timer2
	OSTmrCreate(     (OS_TMR     *)&tmr2,
                   (CPU_CHAR   *)"tmr2",
                   (OS_TICK)      20,   //20*10ms
                   (OS_TICK)       100,  //100*10ms
                   (OS_OPT)   OS_OPT_TMR_ONE_SHOT,
                   (OS_TMR_CALLBACK_PTR) tmr2_callback,  //function pointer
                   (void    *)0,
                   (OS_ERR  *)&err);
	
	OS_CRITICAL_ENTER(); //enter critical
	//task1
	OSTaskCreate(     (OS_TCB  *)&Task1_TaskTCB,
                    (CPU_CHAR  *)"task1_task",
                    (OS_TASK_PTR)  task1_task,
                    (void *)     0,
                    (OS_PRIO)    TASK1_TASK_PRIO,
                    (CPU_STK  *)TASK1_TASK_STK,
                    (CPU_STK_SIZE)   TASK1_STK_SIZE/10,
                    (CPU_STK_SIZE)   TASK1_STK_SIZE,
                    (OS_MSG_QTY)    0,
                    (OS_TICK)       0,
                    (void    *)0,
                    (OS_OPT)    OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                    (OS_ERR   *)&err);
	
	//task2
	OSTaskCreate(     (OS_TCB  *)&Task2_TaskTCB,
                    (CPU_CHAR  *)"task2_task",
                    (OS_TASK_PTR)  task2_task,
                    (void *)     0,
                    (OS_PRIO)    TASK2_TASK_PRIO,
                    (CPU_STK  *)TASK2_TASK_STK,
                    (CPU_STK_SIZE)   TASK2_STK_SIZE/10,
                    (CPU_STK_SIZE)   TASK2_STK_SIZE,
                    (OS_MSG_QTY)    0,
                    (OS_TICK)       0,
                    (void    *)0,
                    (OS_OPT)    OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                    (OS_ERR   *)&err);
	OS_CRITICAL_EXIT();//EXIT CRITICAL
	OSTaskDel((OS_TCB*)&StartTaskTCB,&err);//delete start_task
}

//task1
void task1_task(void *p_arg)
{
	OS_ERR err;
	u8 a=0;
	
	OSTmrStart(&tmr1,&err);
	OSTmrStart(&tmr2,&err);
	
	while(1)
	{
		LED0=~LED0;
		a++;
		if(a==20){
			OSTaskSuspend((OS_TCB*)&Task2_TaskTCB,&err);
		  OSTmrStart(&tmr2,&err);OSTmrStop(&tmr1,OS_OPT_TMR_NONE,0,&err);}
		if(a==40)
			OSTaskResume((OS_TCB*)&Task2_TaskTCB,&err);
		delay_ms(500);
		
		//OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);

	}
}

//task2
void task2_task(void *p_arg)
{
	OS_ERR err;
	while(1){
		LED1=!LED1;
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
  }
}

//timer1 callback function
void tmr1_callback(void *p_tmr,void *p_arg)
{
	static u8 a=0;
	a++;
	printf("定时器1回调函数执行%d次\n",a);
}


//timer2 callback function
void tmr2_callback(void *p_tmr,void *p_arg)
{
	static u8 a=0;
	a++;
	printf("定时器2回调函数执行%d次\n",a);
}

