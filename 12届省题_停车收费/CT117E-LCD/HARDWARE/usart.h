#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"
void USART_Config(void);
void NVIC_Configuration_1(void);
void USART_SendString(int8_t *str);

#endif 