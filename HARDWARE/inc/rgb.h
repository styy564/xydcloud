#ifndef RGB_H
#define RGB_H

#include "main.h"

#define RGB_CLK_L   (GPIO_ResetBits(GPIOB,GPIO_Pin_8))//时钟线
#define RGB_CLK_H   (GPIO_SetBits(GPIOB,GPIO_Pin_8))

#define RGB_DATA_L  (GPIO_ResetBits(GPIOB,GPIO_Pin_9))//数据线
#define RGB_DATA_H  (GPIO_SetBits(GPIOB,GPIO_Pin_9))



void RGB_Init(void);
void RGB_Transferdata(u8 data);
void RGB_SendData(u8 r,u8 g,u8 b);


#endif
