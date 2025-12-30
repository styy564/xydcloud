#include "step_motor.h"


/**
  * @brief  步进电机管脚初始化
  * @param  void
  * @retval void
	* @note 
	* PA4 -- ~B
	* PA5 -- B
	* PA6 -- ~A
	* PA7 -- A
  */
void Step_Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	//PB6 PB7 P12
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	STEP_STOP;
}

/**
  * @brief  步进电机正转
  * @param  
						u8 speed  转速   
						u8 dire   方向   0 不转，1正转，2反转
						u8 dis    形程   0-100
  * @retval void
	* @note 
  */
void Step_Soft_Control(u8 speed,u8 dire,u8 dis)
{
	for(u16 i=0;i<8*dis;i++)//转动圈数
	{
		if(dire==1)//正转
		{
			STEP1;vTaskDelay(speed);
			STEP2;vTaskDelay(speed);
			STEP3;vTaskDelay(speed);
			STEP4;vTaskDelay(speed);
		}
		else if(dire==2)//反转
		{
			STEP4;vTaskDelay(speed);
			STEP3;vTaskDelay(speed);
			STEP2;vTaskDelay(speed);
			STEP1;vTaskDelay(speed);
		}
	}
	STEP_STOP;
}

void Step_Motor_Control(u8 speed,u8 dire,u8 dis)
{
	for(u16 i=0;i<8*dis;i++)//转动圈数
	{
		if(dire==1)//正转
		{
			STEP1;delay_ms(speed);
			STEP2;delay_ms(speed);
			STEP3;delay_ms(speed);
			STEP4;delay_ms(speed);
		}
		else if(dire==2)//反转
		{
			STEP4;delay_ms(speed);
			STEP3;delay_ms(speed);
			STEP2;delay_ms(speed);
			STEP1;delay_ms(speed);
		}
	}
	STEP_STOP;
}

