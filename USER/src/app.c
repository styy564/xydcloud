#include "app.h"
#include "default.h"
#include "gizwits_product.h" // Include for NTP time access

//任务句柄
TaskHandle_t  start_task_handle;
TaskHandle_t  task1_handle;
TaskHandle_t  task2_handle;
TaskHandle_t  task3_handle;


//软件定时器句柄
TimerHandle_t timer1_handle;
TimerHandle_t timer2_handle;
TimerHandle_t timer3_handle;


//队列句柄
QueueHandle_t queue1_handle;


//信号量句柄
SemaphoreHandle_t semp1_handle;//二值
SemaphoreHandle_t semp2_handle;//计数型
SemaphoreHandle_t semp3_handle;//互斥


u8 page_flag = 0;//表示当前页码
u8 ui_flag = 0;//0进入新页面清屏  1清屏完成
u8 led_flag = 1;//选择LED灯
u8 motor_speed = 0;//风扇转速 0 - 5
RGB rgb = {0,0,0,1};
RGB rgb_old = {0,0,0,1};
u8 step_flag = 0;//0窗帘关闭  1打开
u8 oled_ref = 0;
u8 relay_flag = 0;//0继电器关闭  1打开
u8 menu = 4;//主页面要显示哪个图标
u8 remote_clear = 0;

u8 str[30]; // Increased size to handle formatted time strings
/*****************************Task1******************************/
#define  TASK1_SIZE  64
#define	 TASK1_PRI	 2
void Task1(void *ptr)
{
	ptr = ptr;
	printf("进入1\r\n");
	
	while(1)
	{
		key = key_scanf();
		if(key == 1 || remote_key == 0x10)
		{
			if(page_flag == 0)//***********主页面*************
			{
				OLED_DisplayStr(40,6,(u8*)"        ");//清除最后一行的文字
				for(u8 i=0;i<4;i++)//图标移动动画
				{
					if(i==3) OLED_DisplayNewPic(8,2,32,4,0,(u8*)homepage[menu==4?25:menu==9?30:menu-14]);
					else  OLED_DisplayNewPic(0,2,10*(i+1),4,32-10*(i+1),(u8*)homepage[menu==4?25:menu==9?30:menu-14]);
					OLED_DisplayNewPic(18+10*i,2,32,4,0,(u8*)homepage[(menu==4?30:menu-9)+(i+1)]);
					OLED_DisplayNewPic(58+10*i,2,32,4,0,(u8*)homepage[menu-(i+1)]);
					OLED_DisplayNewPic(i>2?128:98+10*i,2,32,4,0,(u8*)homepage[menu==34?0:menu+1]);
					
					//清除两个图标之间空隙
					OLED_DisplayPic(10+10*i,2,8,4,(u8*)other[1]);
					OLED_DisplayPic(50+10*i,2,8,4,(u8*)other[1]);
					OLED_DisplayPic(90+10*i,2,8,4,(u8*)other[1]);
					if(i==3) OLED_DisplayPic(0,2,8,4,(u8*)other[1]);
					vTaskDelay(50);
				}	
				menu -= 5;
				if(menu>=200) menu = 34;
			}					
			else if(page_flag == 3)//**********LED切换*************
			{
				if(--led_flag > 200) led_flag = 0;
				OLED_DisplayStr(90,2,(u8*)"  ");
				OLED_DisplayStr(90,4,(u8*)"  ");
				OLED_DisplayStr(90,6,(u8*)"  ");
				OLED_DisplayStr(20,0,(u8*)"  ");
				//箭头，指向选中目标
				if(led_flag>0) OLED_DisplayPic(90,led_flag*2,16,2,(u8*)other[0]);
				else OLED_DisplayPic(20,0,16,2,(u8*)other[0]);
			}
			else if(page_flag == 4)//*********风扇减速********
			{
				if(--motor_speed > 5) motor_speed = 0;
				currentDataPoint.valuemotor = motor_speed;//上报云端
				ui_flag = 0;
			}
			else if(page_flag == 5)//*********RGB二级页面 *********
			{
				if(--rgb.flag > 3) rgb.flag = 0;
				OLED_DisplayStr(56,2,(u8*)"  ");
				OLED_DisplayStr(56,4,(u8*)"  ");
				OLED_DisplayStr(56,6,(u8*)"  ");
				OLED_DisplayStr(20,0,(u8*)"  ");
				//箭头，指向选中目标
				if(rgb.flag > 0) OLED_DisplayPic(56,rgb.flag*2,16,2,(u8*)other[0]);
				else OLED_DisplayPic(20,0,16,2,(u8*)other[0]);
				oled_ref = 1;
			}
			else if(page_flag == 8)//*********RGB三级页面 *********
			{
				if(rgb.flag == 1)
				{
					if(rgb.r >= 50) rgb.r -= 50;
					else rgb.r = 0;
					currentDataPoint.valuer = rgb.r;
				}
				else if(rgb.flag == 2)
				{
					if(rgb.g >= 50) rgb.g -= 50;
					else rgb.g = 0;
					currentDataPoint.valueg = rgb.g;
				}
				else if(rgb.flag == 3)
				{
					if(rgb.b >= 50) rgb.b -= 50;
					else rgb.b = 0;
					currentDataPoint.valueb = rgb.b;
				}
				oled_ref = 1;
			}
			else if(page_flag == 6)//关闭窗帘
			{
				if(step_flag==1)
				{
					step_flag = 0;
					oled_ref = 1;//窗帘关闭动画
					currentDataPoint.valuestep = 0;
				}
			}
			else if(page_flag == 7)//***********充电关闭*************
			{
				RELAY_OFF;
				currentDataPoint.valuerelay = 0;
			}	
			remote_clear = 1;
		}
		else if(key == 2 || remote_key == 0xd)
		{
			if(page_flag == 0)//***********主页面*************
			{
				ui_flag = 0;
				page_flag = menu / 5 + 1;//进入选中的页面
				led_flag = 1;//默认指向LED1
				rgb.flag = 1;//默认指向RGB R
			}
			else if(page_flag == 1 || page_flag==2 || page_flag==4 || page_flag == 6 || page_flag == 7)//***********返回主页面*************
			{
				//返回主页面
				ui_flag = 0;
				page_flag = 0;
			}
			else if(page_flag == 3)//***********开关LED*************
			{
				if(led_flag==0) 
				{
					//返回主页面
					ui_flag = 0;
					page_flag = 0;
				}
				else if(led_flag==1) 		 
				{
					LED1_TURN;
					currentDataPoint.valueled1 ^= 1;
				}
				else if(led_flag==2) 
				{
					LED2_TURN;
					currentDataPoint.valueled2 ^= 1;
				}
				else if(led_flag==3) 
				{
					LED3_TURN;
					currentDataPoint.valueled3 ^= 1;
				}
			}

			else if(page_flag == 5)//*************R G B ***********
			{
				if(rgb.flag==0)
				{
					//返回主页面	
					ui_flag = 0;
					page_flag = 0;
				}
				else
				{
					ui_flag = 0;
					page_flag = 8;
				}
			}
			else if(page_flag == 8)//*********RGB三级页面 *********
			{
				page_flag = 5;//回到RGB二级页面
				ui_flag = 0;//刷新页面 
			}
			remote_clear = 1;
		}
		else if(key == 3 || remote_key == 0x11)
		{
			if(page_flag == 0)//***********主页面*************
			{
				OLED_DisplayStr(40,6,(u8*)"        ");//清除最后一行的文字
				for(u8 i=0;i<4;i++)//图标移动动画
				{
					OLED_DisplayNewPic(0,2,30-10*i,4,2+10*i,(u8*)homepage[menu==4?30:menu-9]);
					OLED_DisplayNewPic(38-10*i,2,32,4,0,(u8*)homepage[menu-(i+1)]);
					OLED_DisplayNewPic(78-10*i,2,32,4,0,(u8*)homepage[menu==34?0+(i+1):menu+1+(i+1)]);
					OLED_DisplayNewPic(118-10*i,2,32,4,0,(u8*)homepage[menu==29?0:menu==34?5:menu+1+5]);
					//清除两个图标之间空隙
					OLED_DisplayPic(30-10*i,2,8,4,(u8*)other[1]);
					OLED_DisplayPic(70-10*i,2,8,4,(u8*)other[1]);
					OLED_DisplayPic(110-10*i,2,8,4,(u8*)other[1]);
					if(i==3) OLED_DisplayPic(120,2,8,4,(u8*)other[1]);
					vTaskDelay(50);
				}	
				menu += 5;
				if(menu>=35) menu = 4;
			}
			else if(page_flag == 3)//***********LED***********
			{
				if(++led_flag > 3) led_flag = 3;
				OLED_DisplayStr(90,2,(u8*)"  ");
				OLED_DisplayStr(90,4,(u8*)"  ");
				OLED_DisplayStr(90,6,(u8*)"  ");
				OLED_DisplayStr(20,0,(u8*)"  ");
				//箭头，指向选中目标
				if(led_flag>0) OLED_DisplayPic(90,led_flag*2,16,2,(u8*)other[0]);
				else OLED_DisplayPic(20,0,16,2,(u8*)other[0]);
			}
			else if(page_flag == 4)//************风扇加速***********
			{
				if(++motor_speed > 5)	motor_speed = 5;
					currentDataPoint.valuemotor = motor_speed;//上报云端0
					ui_flag = 0;
			}
			else if(page_flag == 5)//*********RGB二页面 *********
			{
				if(++rgb.flag > 3) rgb.flag = 3;
				OLED_DisplayStr(56,2,(u8*)"  ");
				OLED_DisplayStr(56,4,(u8*)"  ");
				OLED_DisplayStr(56,6,(u8*)"  ");
				OLED_DisplayStr(20,0,(u8*)"  ");
				//箭头，指向选中目标
				if(rgb.flag > 0) OLED_DisplayPic(56,rgb.flag*2,16,2,(u8*)other[0]);
				else OLED_DisplayPic(20,0,16,2,(u8*)other[0]);
			}
			else if(page_flag == 8)//*********RGB三级页面 *********
			{
				if(rgb.flag==1)
				{
					if(rgb.r + 50 > 255) rgb.r = 250;
					else rgb.r += 50;
					currentDataPoint.valuer = rgb.r;
				}
				else if(rgb.flag==2)
				{
					if(rgb.g + 50 > 255) rgb.g = 250;
					else rgb.g += 50;
					currentDataPoint.valueg = rgb.g;
				}
				else if(rgb.flag==3)
				{
					if(rgb.b + 50 > 255) rgb.b = 250;
					else rgb.b += 50;
					currentDataPoint.valueb = rgb.b;
				}
				oled_ref = 1;
			}
			else if(page_flag == 6)//**********打开窗帘***********
			{
				if(step_flag==0)
				{
					step_flag = 1;
					oled_ref = 1;//窗帘打开动画
					currentDataPoint.valuestep = 1;
				}
			}
			else if(page_flag == 7)//***********充电打开*************
			{
				RELAY_ON;
				currentDataPoint.valuerelay = 1;
			}
			remote_clear = 1;
		}
		if(remote_clear)//清除按键值和红外接收
		{
			remote_clear = 0;
			remote_key = 0;
			remote_data = 0;
			key = 0;
		}
		vTaskDelay(5);
		//vTaskSuspend(task1_handle);
	}
}


/*****************************Task2***********************************/
#define  TASK2_SIZE  64
#define	 TASK2_PRI	 3
void Task2(void *ptr)
{
	ptr = ptr;
	printf("进入2\r\n");
	while(1)
	{
		if(remote_key != 0)
		{
			if(remote_key < 8) page_flag = remote_key;//进入对应的一级界面
			else if(remote_key == 0x16) page_flag = 0;//返回主页面
			else if(remote_key == 0x1d)
			{
				if(page_flag >= 1 && page_flag <= 7)//返回主页面
				{
					page_flag = 0;
				}
				else page_flag = 5;//返回RGB二级页面
			}
			ui_flag = 0;
			remote_clear = 1;
		}
		vTaskDelay(5);
		//vTaskSuspend(task2_handle);
	}
}


/*****************************Task3***********************************/
#define  TASK3_SIZE  64
#define	 TASK3_PRI	 1
void Task3(void *ptr)
{
	ptr = ptr;
	printf("进入3\r\n");
	while(1)
	{
		if(page_flag == 0)//**********************    主页面     ***************************
		{
			if(ui_flag == 0)//发生了页面切换
			{
				ui_flag = 1;
				OLED_Clear(0);
				OLED_DisplayStr(8,0,(u8*)"第九小组云家居");
				OLED_DisplayPic(8,2,32,4,(u8*)homepage[menu==4?30:menu-9]);
				OLED_DisplayPic(48,2,32,4,(u8*)homepage[menu]);
				OLED_DisplayPic(88,2,32,4,(u8*)homepage[menu==34?0:menu+1]);
			}
			
			//根据选中图标功能，显示相应文字
			switch(menu)
			{
				case 4:	OLED_DisplayStr(48,6,(u8*)"时间");	break;
				case 9:	OLED_DisplayStr(40,6,(u8*)"温湿度");break;
				case 14:OLED_DisplayStr(52,6,(u8*)"LED");		break;
				case 19:OLED_DisplayStr(48,6,(u8*)"风扇");	break;
				case 24:OLED_DisplayStr(52,6,(u8*)"RGB");		break;
				case 29:OLED_DisplayStr(48,6,(u8*)"窗帘");	break;
				case 34:OLED_DisplayStr(48,6,(u8*)"充电");	break;
			}
		}
		else if(page_flag == 1)//*****************    时间页面     *************************
			{
				if(ui_flag == 0)//发生了页面切换
				{
					ui_flag = 1;
					OLED_Clear(0);
					OLED_DisplayStr(40,0,(u8*)"时间");
					OLED_DisplayPic(0,0,16,2,(u8*)other[2]);//退出图标
					OLED_DisplayPic(20,0,16,2,(u8*)other[0]);//箭头，指向选中目标
				}
				// Display NTP time - simplified version
				// Directly format and display time with basic validity check
				// Check if time data is valid
				if(ntp_time.year >= 2020 && ntp_time.year <= 2100 && 
				   ntp_time.month >= 1 && ntp_time.month <= 12 && 
				   ntp_time.day >= 1 && ntp_time.day <= 31) {
					// Format date with trailing spaces to overwrite any old characters
					sprintf((char*)str, "%04d-%02d-%02d      ", 
						ntp_time.year, ntp_time.month, ntp_time.day);
					OLED_DisplayStr(24, 2, str);
					// Format time with trailing spaces to overwrite any old characters
					sprintf((char*)str, "%02d:%02d:%02d        ", 
						ntp_time.hour, ntp_time.minute, ntp_time.second);
					OLED_DisplayStr(32, 4, str);
				} else {
					// Display default time if data is invalid
					OLED_DisplayStr(24, 2, (u8*)"2023-01-01      ");
					OLED_DisplayStr(32, 4, (u8*)"00:00:00        ");
				}
				
				// Short delay to prevent blocking
				vTaskDelay(1000);
		}
		else if(page_flag == 2)//*******************  温湿度    ****************************
		{
			if(ui_flag == 0)//发生了页面切换
			{
				ui_flag = 1;
				OLED_Clear(0);
				delay_ms(10); // 增加延迟确保清屏完成
				OLED_DisplayStr(44,0,(u8*)"DHT11");
				OLED_DisplayPic(0,0,16,2,(u8*)other[2]);//退出图标
				OLED_DisplayPic(20,0,16,2,(u8*)other[0]);//箭头，指向选中目标
				
//				sprintf((char*)str,"温度:%.1f",dht_temp);
//				OLED_DisplayStr(24,2,str);
//				sprintf((char*)str,"湿度:%.1f",dht_humi);
//				OLED_DisplayStr(24,4,str);
			}
		}
		else if(page_flag == 3)//******************   LED页面    **************************
		{
			if(ui_flag == 0)
			{
				ui_flag = 1;
				OLED_Clear(0);
				delay_ms(10); // 增加延迟确保清屏完成
				OLED_DisplayStr(52,0,(u8*)"LED");
				OLED_DisplayStr(10,2,(u8*)"LED1");
				OLED_DisplayStr(10,4,(u8*)"LED2");
				OLED_DisplayStr(10,6,(u8*)"LED3");
				OLED_DisplayPic(0,0,16,2,(u8*)other[2]);//退出图标
				OLED_DisplayPic(90,led_flag*2,16,2,(u8*)other[0]);//箭头，指向选中目标
			}
			
			if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_11)) OLED_DisplayStr(56,2,(u8*)"ON "); else OLED_DisplayStr(56,2,(u8*)"OFF");		
			if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_14)) OLED_DisplayStr(56,4,(u8*)"ON "); else OLED_DisplayStr(56,4,(u8*)"OFF");
			if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_11)) OLED_DisplayStr(56,6,(u8*)"ON "); else OLED_DisplayStr(56,6,(u8*)"OFF");
		}
		else if(page_flag == 4)//********************  风扇   ****************************
		{
			if(ui_flag == 0)
			{
				ui_flag = 1;
				OLED_Clear(0);
				OLED_DisplayStr(48,0,(u8*)"风扇");
				
				OLED_DisplayPic(72,3,48,2,(u8*)motor[motor_speed]);
				sprintf((char*)str,"%d%%  ",motor_speed*20);//转速百分比
				OLED_DisplayStr(84,5,str);
				OLED_DisplayPic(0,0,16,2,(u8*)other[2]);//退出图标
				OLED_DisplayPic(20,0,16,2,(u8*)other[0]);//箭头，指向选中目标
			}
			Morot_Up(200*motor_speed);//电机启动
			if(motor_speed != 0)//风扇动图
				{
					static u8 fan_frame = 0;
					OLED_DisplayPic(16,3,32,4,(u8*)gif32x32[fan_frame]);
					fan_frame = (fan_frame + 1) % 5;
					vTaskDelay(20);
			}
			else OLED_DisplayPic(16,3,32,4,(u8*)gif32x32[0]);
		}
		else if(page_flag == 5)//******************  RGB二级页面    ****************************
		{
			if(ui_flag == 0)
			{
				ui_flag = 1;
				oled_ref = 1;
				OLED_Clear(0);
				OLED_DisplayStr(52,0,(u8*)"RGB");
				OLED_DisplayStr(24,2,(u8*)"R");
				OLED_DisplayStr(24,4,(u8*)"G");
				OLED_DisplayStr(24,6,(u8*)"B");
				OLED_DisplayPic(56,rgb.flag*2,16,2,(u8*)other[0]);//箭头，指向选中目标
				OLED_DisplayPic(0,0,16,2,(u8*)other[2]);//退出图标	
			}
			//只有当RGB值发生变化时才发送数据
			if(rgb.r != rgb_old.r || rgb.g != rgb_old.g || rgb.b != rgb_old.b)
			{
				RGB_SendData(rgb.r,rgb.g,rgb.b);
				rgb_old.r = rgb.r;
				rgb_old.g = rgb.g;
				rgb_old.b = rgb.b;
			}
			
			if(oled_ref)//oled发生了改变
			{
				oled_ref = 0;                
				sprintf((char*)str,"%d  ",rgb.r);
				OLED_DisplayStr(92,2,(u8*)str);
				sprintf((char*)str,"%d  ",rgb.g);
				OLED_DisplayStr(92,4,(u8*)str);
				sprintf((char*)str,"%d  ",rgb.b);
				OLED_DisplayStr(92,6,(u8*)str);
			}
		}
		else if(page_flag == 8)//******************  RGB三级页面    ****************************
		{
			if(ui_flag == 0)
			{
				ui_flag = 1;
				oled_ref = 1;
				OLED_SetClear(23,3,80,1,0x07);
				OLED_SetClear(23,6,80,1,0xE0);
				OLED_SetClear(20,3,3,4,0xFF);
				OLED_SetClear(104,3,3,4,0xFF);
				OLED_SetClear(23,4,80,2,0);
				OLED_DisplayStr(28,4,(u8*)"亮度:");
			}
			//只有当RGB值发生变化时才发送数据
			if(rgb.r != rgb_old.r || rgb.g != rgb_old.g || rgb.b != rgb_old.b)
			{
				RGB_SendData(rgb.r,rgb.g,rgb.b);
				rgb_old.r = rgb.r;
				rgb_old.g = rgb.g;
				rgb_old.b = rgb.b;
			}
			
			if(oled_ref == 1)
			{
				oled_ref = 0;
				if(rgb.flag == 1)
				{
					sprintf((char*)str,"%03d",rgb.r);
					OLED_DisplayStr(76,4,(u8*)str);
				}
				else if(rgb.flag == 2)
				{
					sprintf((char*)str,"%03d",rgb.g);
					OLED_DisplayStr(76,4,(u8*)str);
				}
				else if(rgb.flag == 3)
				{
					sprintf((char*)str,"%03d",rgb.b);
					OLED_DisplayStr(76,4,(u8*)str);
				}
			}
		}
		else if(page_flag == 6)//********************  窗帘   ******************************
		{
			if(ui_flag == 0)
			{
				ui_flag = 1;
				OLED_Clear(0);
				OLED_DisplayStr(48,0,(u8*)"窗帘");
				OLED_DisplayPic(0,0,16,2,(u8*)other[2]);//退出图标
				OLED_DisplayPic(20,0,16,2,(u8*)other[0]);//箭头，指向选中目标
				if(step_flag) OLED_DisplayPic(32,3,64,4,(u8*)step[0]);//打开
				else OLED_DisplayPic(32,3,64,4,(u8*)step[7]);//关闭
			}
			//窗帘动画
			if(oled_ref)
			{
				oled_ref = 0;
				if(step_flag)//打开窗帘
				{
					for(u8 i=8;i>0;i--)
					{
						OLED_DisplayPic(32,3,64,4,(u8*)step[i-1]);
						Step_Soft_Control(5,1,4);
						if((page_flag != 6) || !step_flag) break;//打断动画
					}
				}
				else//关闭窗帘
				{
					for(u8 i=0;i<8;i++)
					{
						OLED_DisplayPic(32,3,64,4,(u8*)step[i]);
						Step_Soft_Control(5,2,4);
						if((page_flag != 6) || step_flag) break;//打断动画
					}
				}
			}
		}
		else if(page_flag == 7)//********************  继电器  *****************************
		{
			if(ui_flag == 0)
			{
				ui_flag = 1;
				OLED_Clear(0);
				OLED_DisplayStr(48,0,(u8*)"充电");
				OLED_DisplayPic(0,0,16,2,(u8*)other[2]);//退出图标
				OLED_DisplayPic(20,0,16,2,(u8*)other[0]);//箭头，指向选中目标
			}
			//开关字样
			OLED_DisplayPic(20,3,32,4,(u8*)relay[RELAY_OUT]);
			//充电动图
				if(RELAY_OUT)
				{
					static u8 charge_frame = 0;
					OLED_DisplayPic(70,4,48,2,(u8*)motor[charge_frame]);
					charge_frame = (charge_frame + 1) % 6;
					vTaskDelay(200);
					if((page_flag != 7) || !RELAY_OUT) charge_frame = 0;//****回到首页直接重置****
			}
			else OLED_DisplayPic(70,4,48,2,(u8*)motor[0]);
		}
		vTaskDelay(10);
//		vTaskResume(task2_handle);
//		vTaskResume(task1_handle);
	}
}




/**********************软件定时器1的回调函数*******************************/
void Timer1_CallBack(void)
{
	gizwitsGetNTP();
	printf("%x\r\n",remote_key);
}

/**********************软件定时器2的回调函数*******************************/
void Timer2_CallBack(void)
{
	taskENTER_CRITICAL();
	DHT11_Read();
	taskEXIT_CRITICAL();
	
	currentDataPoint.valuetemp = dht_temp;
	currentDataPoint.valuehumi = dht_humi;
	
	if(page_flag == 2)
	{
		sprintf((char*)str,"温度:%.1f",dht_temp);
		OLED_DisplayStr(24,2,str);
		sprintf((char*)str,"湿度:%.1f",dht_humi);
		OLED_DisplayStr(24,4,str);
	}
}


/************************起始任务函数*************************************/
void Start_Task(void *ptr)
{
	ptr = ptr;//防止野指针
	printf("进入起始任务\r\n");
	
	/***********创建软件定时器1*************/
	timer1_handle = xTimerCreate("timer1",  //字符串
															 1000,				//软件定时器
															 pdTRUE,   //pdFALSE pdTRUE
															 "1",				//ID,不一样的字符串形式
															 (TimerCallbackFunction_t)Timer1_CallBack);//时间到了执行什么
	
	/***********创建软件定时器2*************/
	timer2_handle = xTimerCreate("timer2",
															 2000,
															 pdTRUE,
															 "2",
															 (TimerCallbackFunction_t)Timer2_CallBack);
	
	/****************创建队列****************/
	queue1_handle = xQueueCreate(10, //队列总长度
														 1); //每个数据大小
	
	/****************创建信号量***************/
	semp1_handle = xSemaphoreCreateBinary();	
	semp2_handle = xSemaphoreCreateCounting(10,3);
	semp3_handle = xSemaphoreCreateMutex();
	
	/****************task1******************/
	xTaskCreate(Task1,   	 //任务函数名
							"task1",   //填入字符串标识函数，作用不大
							TASK1_SIZE,//设置任务栈大小，为输入值的4倍
							NULL, 		 //任务参数
							TASK1_PRI, //优先级
							&task1_handle);//任务句柄
	
	/*****************task3******************/
	xTaskCreate(Task3,
							"task3",
							TASK3_SIZE,
							NULL,
							TASK3_PRI,
							&task3_handle);
	
	/*****************task2*******************/
	xTaskCreate(Task2,
							"task2",
							TASK2_SIZE,
							NULL,
							TASK2_PRI,
							&task2_handle);
							
	xTimerStart(timer2_handle,0);//打开温湿度
	xTimerStart(timer1_handle,0);//打开WIFI获取时间
	vTaskDelete(start_task_handle);//删除任务
	
	while(1)
	{
		
		
	}
}
