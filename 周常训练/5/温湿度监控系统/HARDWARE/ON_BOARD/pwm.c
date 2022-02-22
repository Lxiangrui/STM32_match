#include "stm32f10x.h"


void TIM_Config(uint16_t Channel2Pulse)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		
	
	/* Time base configuration */			
	//频率单位为HZ，周期单位为us
	Channel2Pulse = 1000000/Channel2Pulse;
	
	TIM_TimeBaseStructure.TIM_Period = Channel2Pulse;  //1KHz
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	//TIM2预分频设置:1MHZ,APB1分频系数2，输入到TIM3时钟为36MHzx2 = 72MHz  
	TIM_PrescalerConfig(TIM2,71, TIM_PSCReloadMode_Immediate);		

	/* Channel 2 and 3 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = Channel2Pulse/2;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	
	
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);


	//使能TIM2定时计数器
	TIM_Cmd(TIM2, ENABLE);
	//使能TIM2 PWM输出模式
	TIM_CtrlPWMOutputs(TIM2, ENABLE);
}

/**
  * @说明     PWM输出模式 PA1(TIM2-CH2)、PA2(TIM2-CH3)引脚配置
  * @参数     None 
  * @返回值   None
  */
void PWM_IO_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void TIME_INPUT(void)
{
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* GPIOA clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	

  /* TIM3 channel 2 pin (PA.07) configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  /* Enable the TIM3 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

  TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);

  /* Select the TIM3 Input Trigger: TI2FP2 */
  TIM_SelectInputTrigger(TIM3, TIM_TS_TI2FP2);

  /* Select the slave Mode: Reset Mode */
  TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);

  /* Enable the Master/Slave Mode */
  TIM_SelectMasterSlaveMode(TIM3, TIM_MasterSlaveMode_Enable);

  /* TIM enable counter */
  TIM_Cmd(TIM3, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);

}