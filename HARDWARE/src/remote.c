#include "remote.h"


/**
  * @brief  超声波测距模块初始化
  * @param  void
  * @retval void
	* @note 
	* PA1  --  TIM2_CH2（输入捕获）
	* 定时器频率72MHz
  */
void Remote_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//PB9  --  TIM2_CH4（输入捕获）
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	

	//TIM_InternalClockConfig(TIM2);
	
	//定时器时基
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 12000 - 1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);

	//输入捕获
	TIM_ICInitTypeDef TIM_ICInitStruct = {0};
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStruct.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM2,&TIM_ICInitStruct);
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	//NVIC
	NVIC_SetPriority(TIM2_IRQn,NVIC_EncodePriority(4,6,0));
	NVIC_EnableIRQ(TIM2_IRQn);
	
	//打开中断
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_ITConfig(TIM2,TIM_IT_CC2,ENABLE);
	
	TIM_Cmd(TIM2,ENABLE);
}

/**
  * @brief  接受数据
  * @param  void
  * @retval void
	* @note
  */
u8 remote_flag = 0;
u32 cnt = 0;
u32 remote_data = 0;
u8 remote_key = 0;
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update))//更新中断
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除标志位
		remote_key = (remote_data & 0x00ff0000)>>16;
		remote_flag=0;	
	}
	
	if(TIM_GetITStatus(TIM2,TIM_IT_CC2))//通道中断
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_CC2);//清除标志位
		
		//上升沿捕获
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1))
		{
			TIM_OC2PolarityConfig(TIM2,TIM_OCPolarity_Low);//切换为下降沿触发
			TIM_SetCounter(TIM2,0);//CNT清零		
		}
		//下降沿捕获,CNT清零
		else if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1))
		{
			TIM_OC2PolarityConfig(TIM2,TIM_OCPolarity_High);//切换为上升沿触发
			cnt = TIM_GetCapture2(TIM2);//获取高电平时间
			//接收数据
			if(remote_flag)
			{
				remote_data >>= 1;
				if(cnt>1500)//&& cnt<1800
				{
					remote_data |= 0x80000000;
				}
			}
			else
			{
				//起始信号
//				if(cnt>4000 && cnt<5000)//
//				{
					remote_flag = 1;
//					remote_data = 0;
//				}
			}
		}
	}
}

