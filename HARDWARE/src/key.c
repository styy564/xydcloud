#include "key.h"



/**
  * @brief  按键管脚初始化
  * @param  void
  * @retval void
	* @note 
	* KEY1 -- PA0  --  高电平
	* KEY2 -- PA8
	* KEY3 -- PB10
  */
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//PA11
	GPIO_InitTypeDef	GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//PB11 PB14
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}


/**
  * @brief  按键扫描
  * @param  void
  * @retval u8 返回按键值
	* @note 
  */
u8 key_scanf(void)
{
	static u8 key_flag=0;
	
	if((KEY1 || KEY2 || KEY3) && !key_flag)
	{
		delay_ms(30);
		key_flag = 1;
		if(KEY1)
			return 1;
		if(KEY2)
			return 2;
		if(KEY3)
			return 3;
	}
	else if(!KEY1 && !KEY2 && !KEY3 && key_flag)
	{
		key_flag=0;
	}
	return 0;
}


