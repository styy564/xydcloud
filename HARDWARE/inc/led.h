#ifndef LED_H
#define LED_H

#include "main.h"


#define LED1_ON    GPIO_SetBits(GPIOB,GPIO_Pin_11)
#define LED1_OFF   GPIO_ResetBits(GPIOB,GPIO_Pin_11)
#define LED1_TURN  GPIOB->ODR ^= (1<<11)

#define LED2_ON    GPIO_SetBits(GPIOB,GPIO_Pin_14)
#define LED2_OFF   GPIO_ResetBits(GPIOB,GPIO_Pin_14)
#define LED2_TURN  GPIOB->ODR ^= (1<<14)

#define LED3_ON    GPIO_SetBits(GPIOA,GPIO_Pin_11)
#define LED3_OFF   GPIO_ResetBits(GPIOA,GPIO_Pin_11)
#define LED3_TURN  GPIOA->ODR ^= (1<<11)



void Led_Init(void);


#endif
