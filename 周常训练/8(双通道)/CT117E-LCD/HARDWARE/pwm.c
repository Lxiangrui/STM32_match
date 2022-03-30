#include "stm32f10x.h"

uint16_t CCR1_Val ; //ռ�ձ� 20%
uint16_t CCR1_Val ; //40%

int OUT_TT;							//khz

//PA0 PA1 PA2  PA3
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //PWM �������Ǹ����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//TIM3 ��Ϊ������ʱ�� 4��ͨ��
//TIM3 ��4·ͨ�� PA0 PA1 PA2 PA3 
void TIM3_Configuration()
{
	
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
  GPIO_Configuration();
	
  CCR1_Val  =  OUT_TT/2;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//һ���򿪸���ʱ��
	
	/* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = OUT_TT-1;
  TIM_TimeBaseStructure.TIM_Prescaler = 71; //72��Ƶ  һ�� 1/1M  ��������Ϊ 1/ 1000 S	   
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
  //���ø���ͨ��
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWMģʽ
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;//��һ���Ƚ�ֵ
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  //PWM ���ʱ ֻ����������Ϳ�����
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
  //�����������װ��
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  //ͨ��2
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
 
 
  TIM_ARRPreloadConfig(TIM3, ENABLE);
  TIM_Cmd(TIM3, ENABLE);

  //����Ҫ�ж�
  //TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);
 
}