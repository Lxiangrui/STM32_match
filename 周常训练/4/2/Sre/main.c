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
void TIM_Config(void);
void PWM_IO_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @说明     主函数
  * @参数     None 
  * @返回值   None
  */
int main(void)
{   	


	PWM_IO_Config();
	TIM_Config();
	//LCD工作模式配置
	STM3210B_LCD_Init();
	LCD_Clear(White);
	LCD_SetTextColor(Black);
	LCD_SetBackColor(White);

		
    while(1){		

				sprintf(buff,"       F:1KHZ");	
				LCD_DisplayStringLine(Line4,buff);
			
				sprintf(buff,"      DUTY:40");	
				LCD_DisplayStringLine(Line6,buff);

	}
}





/**
  * @说明     通用定时器TIM3配置函数,PWM输出工作模式
  * @参数     none
  * @返回值   None
  */
void TIM_Config()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);


		// 优先级配置
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn ;	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
    NVIC_Init(&NVIC_InitStructure);		
	
	
	TIM_TimeBaseStructure.TIM_Period = 250-1;  //250us，发生一次中断
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
  * @说明     PWM输出模式 PA6引脚配置
  * @参数     None 
  * @返回值   None
  */
void PWM_IO_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/******************************************END OF FILE*************************/
