#include "stm32f10x.h"
#include "pwm.h"



void Init_pwm(uint16_t FF,uint16_t DUTY){
	 GPIO_InitTypeDef GPIO_InitStructure;
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 TIM_OCInitTypeDef  TIM_OCInitStructure;
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = (int)(1000/FF)   -1;
  TIM_TimeBaseStructure.TIM_Prescaler = 72   -1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);


  /* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = DUTY;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);


  TIM_ARRPreloadConfig(TIM2, ENABLE);

  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);
}