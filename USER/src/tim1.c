#include "tim1.h"

/**
  * @brief  TIM1初始化
  * @param  void
  * @retval void
	* @note 
  */
void TIM1_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	//时基
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);
	TIM_ARRPreloadConfig(TIM1,ENABLE);
	
	//NVIC
	NVIC_SetPriority(TIM1_UP_IRQn,NVIC_EncodePriority(4,1,0));
	NVIC_EnableIRQ(TIM1_UP_IRQn);
	
	//打开中断
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM1,ENABLE);
}

/**
  * @brief  TIM1中断服务函数
  * @param  void
  * @retval void
	* @note 
  */
void TIM1_UP_IRQHandler(void)
{
  TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	gizTimerMs();
	
	//上报数据
	gizwitsHandle(&currentDataPoint);
	//userHandle(); 空函数

	
}

