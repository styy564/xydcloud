#include "main.h"


u8 key = 0;
int main(void)
{
	NVIC_SetPriorityGrouping(4);//设置优先级分组
	Led_Init();
	Key_Init();
	Relay_Init();//继电器
	Morot_Init();//直流电机
	Usart1_Init(115200);
	RGB_Init();
	DHT11_Init();//温湿度
	OLED_Init();
	Step_Motor_Init();//步进电机
	Remote_Init();
	
	
	//TIM4_Init();
	Gizwits_Init();//机智云初始化
	
	//创建起始任务
	xTaskCreate(Start_Task,   //任务函数名
							"start_task", //填入字符串标识函数，作用不大
							ST_STACK_SIZE,//设置任务栈大小，为输入值的4倍
							NULL, 				//任务参数
							ST_PRI,				//优先级
							&start_task_handle);//任务句柄

	//开启系统调度
	vTaskStartScheduler();
	
	
	while(1)
	{
		WIFI_Connect();
		
		
//		printf("%x",data);
//		delay_ms(1000);
	}
}
	



