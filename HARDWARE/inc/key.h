#ifndef KEY_H
#define KEY_H

#include "main.h"


#define KEY1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define KEY2 (!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8))
#define KEY3 (!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10))




void Key_Init(void);
u8 key_scanf(void);


#endif
