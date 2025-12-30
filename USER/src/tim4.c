#include "tim4.h"

/**
  * @brief  TIM4初始化
  * @param  void
  * @retval void
	* @note 
  */
void TIM4_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	//时基
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	TIM_ARRPreloadConfig(TIM4,ENABLE);
	
	//NVIC
	NVIC_SetPriority(TIM4_IRQn,NVIC_EncodePriority(4,3,0));
	NVIC_EnableIRQ(TIM4_IRQn);
	
	//打开中断
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
}

/**
  * @brief  TIM4中断服务函数
  * @param  void
  * @retval void
	* @note 
  */
void TIM4_IRQHandler(void)
{
  TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}

