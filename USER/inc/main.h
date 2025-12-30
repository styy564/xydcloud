#ifndef MAIN_H
#define MAIN_H



#include "stm32f10x.h"
#include "string.h"
#include "stdio.h"



//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"//任务
#include "app.h"
#include "timers.h"//软件定时器
#include "queue.h"//消息队列
#include "semphr.h"//信号量


//机智云
#include "gizwits_protocol.h"
#include "gizwits_product.h"


#include "led.h"
#include "delay.h"
#include "key.h"
#include "relay.h"
#include "motor.h"
#include "usart1.h"
#include "rgb.h"
#include "dht11.h"
#include "oled.h"
#include "step_motor.h"
#include "remote.h"
#include "wifi.h"
#include "tim1.h"
#include "tim4.h"


extern u8 key;


#endif

