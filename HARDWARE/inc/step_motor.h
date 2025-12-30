#ifndef STEP_MOTOR_H
#define STEP_MOTOR_H

#include "main.h"


#define  STEP_P_B_L   (GPIO_ResetBits(GPIOA,GPIO_Pin_4))
#define  STEP_P_B_H   (GPIO_SetBits(GPIOA,GPIO_Pin_4))

#define  STEP_PB_L    (GPIO_ResetBits(GPIOA,GPIO_Pin_5))
#define  STEP_PB_H    (GPIO_SetBits(GPIOA,GPIO_Pin_5))

#define  STEP_P_A_L   (GPIO_ResetBits(GPIOA,GPIO_Pin_6))
#define  STEP_P_A_H   (GPIO_SetBits(GPIOA,GPIO_Pin_6))

#define  STEP_PA_L    (GPIO_ResetBits(GPIOA,GPIO_Pin_7))
#define  STEP_PA_H    (GPIO_SetBits(GPIOA,GPIO_Pin_7))

#define  STEP_STOP    (GPIO_ResetBits(GPIOA,GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7))

#define	 STEP1   {STEP_PA_H;STEP_P_A_L;STEP_PB_L;STEP_P_B_H;}
#define	 STEP2   {STEP_PA_L;STEP_P_A_H;STEP_PB_L;STEP_P_B_H;}
#define	 STEP3   {STEP_PA_L;STEP_P_A_H;STEP_PB_H;STEP_P_B_L;}
#define	 STEP4   {STEP_PA_H;STEP_P_A_L;STEP_PB_H;STEP_P_B_L;}


void Step_Motor_Init(void);
void Step_Soft_Control(u8 speed,u8 dire,u8 dis);
void Step_Motor_Control(u8 speed,u8 dire,u8 dis);



#endif
