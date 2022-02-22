#ifndef __PWM_H
#define __PWM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

void PWM_IO_Config(void);
void TIM_Config(uint16_t Channel2Pulse, uint16_t Channel3Pulse);


#endif 