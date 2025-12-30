#include "rgb.h"

/**
  * @brief  RGB???????
  * @param  void
  * @retval void
	* @note 
	* PB8 -- CLK
	* PB9 -- DATA
  */
void RGB_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//PB8 PB9
	GPIO_InitTypeDef	GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	RGB_SendData(0x00,0x00,0x00);
	RGB_CLK_L;
	RGB_DATA_H;
}

/**
  * @brief  RGB???????
  * @param  u8 data
  * @retval void
	* @note 
	*/
void RGB_Transferdata(u8 data)
{
  for(u8 i=0;i<8;i++)
	{	
		RGB_CLK_L;//下拉时钟线	
		delay_us(20);
		if(data & (0x80 >> i))
		{
			RGB_DATA_H;
		}
		else 
		{
			RGB_DATA_L;
		}
		delay_us(20);
		RGB_CLK_H;
		delay_us(20);		
	}
}


/**
  * @brief  RGB????????
  * @param  u8 data
  * @retval void
	* @note 
	*/
void RGB_SendData(u8 r,u8 g,u8 b)
{
	RGB_Transferdata(0x00);
	RGB_Transferdata(0x00);
	RGB_Transferdata(0x00);
	RGB_Transferdata(0x00);
	
	//???λ??У??λ
	u8 flag_data = (b&0xC0) | (~(((b&0xC0) >> 2) | ((g&0xC0) >> 4) | ((r&0xC0) >>6)));
	
	RGB_Transferdata(flag_data);
	
	RGB_Transferdata(b);
	RGB_Transferdata(g);
	RGB_Transferdata(r);
	
	RGB_Transferdata(0x00);
}



