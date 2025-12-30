#ifndef DHT11_H
#define DHT11_H

#include "main.h"


#define DHT11_DATA_H   GPIO_SetBits(GPIOB,GPIO_Pin_3)
#define DHT11_DATA_L   GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define DHT11_DATA_IN  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)


extern float dht_temp;
extern float dht_humi;


void DHT11_Init(void);
u8 DHT11_Read(void);


#endif
