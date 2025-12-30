#include "dht11.h"

/**
  * @brief  SHT11管脚初始化
  * @param  void
  * @retval void
	* @note 
	* PB3 -- 开漏输出
  */
void DHT11_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef	GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//解除限制
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	DHT11_DATA_H;
	delay_ms(1000);
}


/**
  * @brief  DHT11读取温湿度
  * @param  void
  * @retval void
	* @note 
	* PB3 -- 开漏输出
  */
float dht_temp = 0;
float dht_humi = 0;
u8 DHT11_Read(void)
{
	//起始信号
	DHT11_DATA_L;
	delay_ms(20);
	DHT11_DATA_H;

	
	//从机应答信号
	u8 cnt = 0;
	while(DHT11_DATA_IN)//等待从机拉低信号
	{
		cnt++;
		delay_us(5);
		if(cnt >= 20)
		{
			return 1;//从机没有应答
		}
	}
	while (!DHT11_DATA_IN);
	while (DHT11_DATA_IN);
	
	//开始读取数据，不接收校验位
	u32 num = 0;
	for(u8 i=0;i<32;i++)
	{
		num <<= 1;
		while(!DHT11_DATA_IN);//等待变为高电平
		delay_us(35);
		if(DHT11_DATA_IN) num |= 1;
		while(DHT11_DATA_IN);//等待高电平时间结束
	}
	dht_humi = ((num>>24)&0xFF) + ((num>>16)&0xFF) / 10.0f;
	dht_temp = ((num>>8)&0xFF) + (num&0xFF) / 10.0f;
	
//	printf("温度：%.1f℃\r\n",dht_temp);
//	printf("湿度：%d%%\r\n",dht_humi);
	
	return 0;
}


