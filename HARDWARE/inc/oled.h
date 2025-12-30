#ifndef OLED_H
#define OLED_H

#include "main.h"


#define  OLED_SCK_L    (GPIO_ResetBits(GPIOB,GPIO_Pin_13))//时钟线
#define  OLED_SCK_H    (GPIO_SetBits(GPIOB,GPIO_Pin_13))

#define  OLED_MOSI_L   (GPIO_ResetBits(GPIOB,GPIO_Pin_15))//发送数据线
#define  OLED_MOSI_H   (GPIO_SetBits(GPIOB,GPIO_Pin_15))

#define  OLED_RESET  	 (GPIO_ResetBits(GPIOB,GPIO_Pin_6))//复位
#define  OLED_UNRESET	 (GPIO_SetBits(GPIOB,GPIO_Pin_6))//不复位

#define  OLED_CMD      (GPIO_ResetBits(GPIOB,GPIO_Pin_12))//发送指令
#define  OLED_DATA     (GPIO_SetBits(GPIOB,GPIO_Pin_12))//发送数据

#define  OLED_CS_L     (GPIO_ResetBits(GPIOB,GPIO_Pin_7))//片选
#define  OLED_CS_H     (GPIO_SetBits(GPIOB,GPIO_Pin_7))






void OLED_Transferdata(u8 data);
void OLED_Clear(u8 color);
void OLED_Init(void);


void OLED_SetClear(u8 col,u8 page,u8 width,u8 high,u8 color);
void OLED_DisplayPic(u8 col,u8 page,u8 width,u8 high,u8*pic);
void OLED_DisplayNewPic(u8 col,u8 page,u8 width,u8 high,u8 front,u8*pic);
void OLED_DisplayChar(u8 x,u8 y,u8 pic);
void OLED_DisplayChin(u8 x,u8 page,u8*pic);
void OLED_DisplayStr(u8 col,u8 page,u8*pic);

#endif
