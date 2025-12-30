#ifndef APP_H
#define APP_H

#include "main.h"




//起始任务
#define  ST_STACK_SIZE  128
#define	 ST_PRI					4

extern  TaskHandle_t  start_task_handle;
void Start_Task(void *ptr);


//任务1
extern  TaskHandle_t  task1_handle;

//任务2
extern  TaskHandle_t  task2_handle;


#define  RELAY_OUT  GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_1)//判断继电器引脚输出电平




typedef struct{
	u8 r;
	u8 g;
	u8 b;
	u8 flag;
}RGB;
extern u8 page_flag;
extern u8 step_flag;
extern u8 oled_ref;
extern u8 motor_speed;
extern u8 ui_flag;
extern RGB rgb;


#endif
