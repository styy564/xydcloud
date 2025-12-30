#include "wifi.h"


/**
  * @brief  USART2初始化
  * @param  void
  * @retval void
	* @note 
	* PA9  -- USART2_TX -- 复用模式
	* PA10 -- USART2_RX -- 浮空输入
  */
void Usart2_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	//PA9 -- 复用模式
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//PA10 -- 浮空输入
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	USART_InitTypeDef USART_InitStruct = {0};
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStruct);
	
	//NVIC
	NVIC_SetPriority(USART2_IRQn,NVIC_EncodePriority(4,2,0));
	NVIC_EnableIRQ(USART2_IRQn);
	
	//中断
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	//USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);  
	
	USART_Cmd(USART2,ENABLE);
}


/**
  * @brief  USART2中断服务函数
  * @param  void
  * @retval void
	* @note 
  */
void USART2_IRQHandler(void)
{
	//接收中断
	static u8 data;
	if(USART_GetITStatus(USART2,USART_IT_RXNE))
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		data = USART_ReceiveData(USART2);
		gizPutData(&data,1);
	}
}

/**
  * @brief  联网
  * @param  void
  * @retval void
	* @note 
  */
void WIFI_Connect(void)
{
	if(KEY1)//设置配网模式
	{
		gizwitsSetMode(WIFI_AIRLINK_MODE);
		printf("WIFI_AIRLINK_MODE OK\r\n");
	}
	if(KEY2)//热点配网
	{
		gizwitsSetMode(WIFI_SOFTAP_MODE);
		printf("WIFI_SOFTAP_MODE OK\r\n");
	}
	if(KEY3)//恢复出厂设置
	{
		gizwitsSetMode(WIFI_RESET_MODE);
		printf("WIFI_RESET_MODE OK\r\n");
	}
}

//机智云初始化
void Gizwits_Init(void)
{
	TIM1_Init();//TIM1初始化
	Usart2_Init();//USART2初始化
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
	gizwitsInit();
}




