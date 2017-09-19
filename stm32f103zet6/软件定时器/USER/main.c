#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "includes.h"
/************************************************
 ALIENTEK ��Ӣ��STM32������UCOSʵ�� 
 ��4-1 UCOSIII UCOSIII��ֲ
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//UCOSIII���������ȼ��û�������ʹ�ã�ALIENTEK
//����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
//���ȼ�0���жϷ������������� OS_IntQTask()
//���ȼ�1��ʱ�ӽ������� OS_TickTask()
//���ȼ�2����ʱ���� OS_TmrTask()
//���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
//���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

//�������ȼ�
#define START_TASK_PRIO 3
//�����ջ��С
#define START_STK_SIZE 128
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);
 
//�������ȼ�
#define TASK1_TASK_PRIO 4
//�����ջ��С
#define TASK1_STK_SIZE 128
//������ƿ�
OS_TCB Task1_TaskTCB;
//�����ջ
CPU_STK TASK1_TASK_STK[TASK1_STK_SIZE]; //typedef  unsigned  int   CPU_STK; 128*4 byte
//������
void task1_task(void *p_arg);

//�������ȼ�
#define TASK2_TASK_PRIO 5
//�����ջ��С
#define TASK2_STK_SIZE 128
//������ƿ�
OS_TCB Task2_TaskTCB;
//�����ջ
CPU_STK TASK2_TASK_STK[TASK2_STK_SIZE]; //typedef  unsigned  int   CPU_STK; 128*4 byte
//������
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
	CPU_SR_ALLOC();  //�����ٽ纯��ʱ�������
	
	delay_init();
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
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
	   OSStatTaskCPUUsageInit(&err); //ͳ������
	#endif
	
	#ifdef CPU_CFG_INT_DIS_MEAS_EN 
	   CPU_IntDisMeasMaxCurReset();	
  #endif
	
	#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	   //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
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
	printf("��ʱ��1�ص�����ִ��%d��\n",a);
}


//timer2 callback function
void tmr2_callback(void *p_tmr,void *p_arg)
{
	static u8 a=0;
	a++;
	printf("��ʱ��2�ص�����ִ��%d��\n",a);
}

