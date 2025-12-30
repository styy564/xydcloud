#include "relay.h"

/**
  * @brief  继电器管脚初始化
  * @param  void
  * @retval void
	* @note 
	* PB1 -- 高电平有效
  */
void Relay_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		
	//PB1
	GPIO_InitTypeDef	GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}
