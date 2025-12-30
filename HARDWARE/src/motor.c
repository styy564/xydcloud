#include "motor.h"

/**
  * @brief  直流电机管脚初始化
  * @param  void
  * @retval void
	* @note 
	* PB4 -- MOTOR_IB -- TIM3_CH1
	* PB5 -- MOTOR_IA -- TIM3_CH2
  */
void Morot_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
		
	//PB4 PB5
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//解除限制
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	//重映射
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
	
	//时基
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 1000;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	//输出比较
	TIM_OCInitTypeDef TIM_OCInitStruct = {0};
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OC1Init(TIM3,&TIM_OCInitStruct);
	
	//输出比较
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OC2Init(TIM3,&TIM_OCInitStruct);
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_ARRPreloadConfig(TIM3,ENABLE);
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM3,ENABLE);
}


/**
  * @brief  直流电机正转
  * @param  void
  * @retval void
	* @note 
  */
void Morot_Up(u16 arr)
{
	TIM_SetCompare1(TIM3,arr);
	TIM_SetCompare2(TIM3,0);
}


/**
  * @brief  直流电机反转
  * @param  void
  * @retval void
	* @note 
  */
void Morot_Down(u16 arr)
{
	TIM_SetCompare1(TIM3,0);
	TIM_SetCompare2(TIM3,arr);
}



