/*******************************************************************************  
* 文件名称：定时器PWM输出实验
* 实验目的：掌握STM32定时器功能和配置方法
* 程序说明：配置TIM3 CH2、CH3为PWM输出模式，CH2输出1KHZ，占空比50%信号，
*           CH3输出1KHz，占空比70%信号
* 日期版本：2011-9-19/V1.0a 
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "lcd.h"
#include "stdio.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
char buff[20];
uint32_t TimingDelay = 0;
uint16_t Channel2Pulse = 0, Channel3Pulse = 0;
int FF;
/* Private function prototypes -----------------------------------------------*/
void Delay_Ms(uint32_t nTime);
void NVIC_Configuration(void);
void TIM_Config(uint16_t Channel2Pulse, uint16_t Channel3Pulse);
void PWM_IO_Config(void);
void In_init(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @说明     主函数
  * @参数     None 
  * @返回值   None
  */
int main(void)
{   	


	PWM_IO_Config();
	
	TIM_Config(998/2,998*7/10);
	In_init();
	//LCD工作模式配置
	STM3210B_LCD_Init();
	LCD_Clear(White);
	LCD_SetTextColor(Black);
	LCD_SetBackColor(White);

		
    while(1){		
		
			
			if(TimingDelay == 1){
			
				sprintf(buff,"      F:%dHZ",FF);	
				LCD_DisplayStringLine(Line4,buff);
			FF=0;	
			TimingDelay=0;
			}
			
			
			
		
	}
}



void In_init(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	/* Enable GPIOA clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
	//PA0-BUTTON1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);

	/* Configure EXTI0 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI0 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/**
  * @说明     通用定时器TIM3配置函数,PWM输出工作模式
  * @参数     none
  * @返回值   None
  */
void TIM_Config(uint16_t Channel2Pulse, uint16_t Channel3Pulse)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure; 
	
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 999;  //1KHz
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* Channel 2 and 3 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);

	//使能TIM3定时计数器
	TIM_Cmd(TIM3, ENABLE);
	//使能TIM3 PWM输出模式
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
	
	
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		// 开启定时器时钟,即内部时钟CK_INT=72M

		// 优先级配置
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn ;	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
    NVIC_Init(&NVIC_InitStructure);		
	
	
	TIM_TimeBaseStructure.TIM_Period = 1000-1;  //1ms，发生一次中断
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;//1Mhz	t = 1us
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	//使能TIM2定时计数器
	TIM_Cmd(TIM2, ENABLE);	
	// 开启计数器中断
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);			
	
}	

/**
  * @说明     PWM输出模式 PA1(TIM3-CH2)、PA2(TIM3-CH3)引脚配置
  * @参数     None 
  * @返回值   None
  */
void PWM_IO_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/******************************************END OF FILE*************************/
