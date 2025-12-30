#ifndef USART1_H
#define USART1_H

#include "main.h"


typedef struct{
	u8 str[100];
	u16 count;	
}U1;
extern U1 u1;


void Usart1_Init(u32 baud);


#endif
