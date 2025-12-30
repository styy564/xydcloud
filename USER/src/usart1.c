#include "usart1.h"


/**
  * @brief  USART1初始化
  * @param  void
  * @retval void
	* @note 
	* PA9  -- USART1_TX -- 复用模式
	* PA10 -- USART1_RX -- 浮空输入
  */
void Usart1_Init(u32 baud)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//PA9 -- 复用模式
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//PA10 -- 浮空输入
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	USART_InitTypeDef USART_InitStruct = {0};
	USART_InitStruct.USART_BaudRate = baud;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStruct);
	
	//NVIC
	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(4,5,0));
	NVIC_EnableIRQ(USART1_IRQn);
	
	//中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	
	USART_Cmd(USART1,ENABLE);
}


/**
  * @brief  USART1中断服务函数
  * @param  void
  * @retval void
	* @note 
  */
U1 u1;
void USART1_IRQHandler(void)
{
	//接收中断
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		u1.str[u1.count++] = USART_ReceiveData(USART1);	
	}
	//空闲中断
	if(USART_GetITStatus(USART1,USART_IT_IDLE))
	{
		USART1->SR;
		USART1->DR;

		u1.str[u1.count]='\0';//结尾加\0
		u1.count=0;//数组下标清零
		printf("%s",u1.str);
	}
}


/**
  * @brief  printf 重定向
  * @param  void
  * @retval void
	* @note 
  */
int fputc(int c, FILE * stream)
{
	while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));
	USART_SendData(USART1,c);
	return c;
}


