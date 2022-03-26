#include "stm32f10x.h"
#include "Pwm.h"

void Pwm_init(void){
			GPIO_InitTypeDef   GPIO_InitStructure;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
}