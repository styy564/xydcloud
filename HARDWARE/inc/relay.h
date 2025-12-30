#ifndef RELAY_H
#define RELAY_H

#include "main.h"


#define RELAY_ON    GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define RELAY_OFF   GPIO_ResetBits(GPIOB,GPIO_Pin_1)

void Relay_Init(void);


#endif
