#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

#define RB1	Key_Scan(GPIOA,GPIO_Pin_0)
#define RB2	Key_Scan(GPIOA,GPIO_Pin_8)
#define RB3 Key_Scan(GPIOB,GPIO_Pin_1)
#define RB4 Key_Scan(GPIOB,GPIO_Pin_2)



void Key_init();
uint16_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
void Key_Set(void);
void Led_all_close();
void Led_init(void);

#define KEY_ON					0
#define KEY_OFF					1

#endif