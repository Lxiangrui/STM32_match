/*******************************************************************************  
* �ļ����ƣ���ʱ��PWM���ʵ��
* ʵ��Ŀ�ģ�����STM32��ʱ�����ܺ����÷���
* ����˵��������TIM3 CH2��CH3ΪPWM���ģʽ��CH2���1KHZ��ռ�ձ�50%�źţ�
*           CH3���1KHz��ռ�ձ�70%�ź�
* ���ڰ汾��2011-9-19/V1.0a 
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
int ms_cnt;
uint16_t Channel2Pulse = 0, Channel3Pulse = 0;
int EXTI_Status;
/* Private function prototypes -----------------------------------------------*/
void Delay_Ms(uint32_t nTime);
void NVIC_Configuration(void);
void TIM_Config(uint16_t Channel2Pulse, uint16_t Channel3Pulse);
void PWM_IO_Config(void);
void In_init(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @˵��     ������
  * @����     None 
  * @����ֵ   None
  */
int main(void)
{   	


	PWM_IO_Config();
	
	TIM_Config(998/2,998*7/10);
	In_init();
	//LCD����ģʽ����
	STM3210B_LCD_Init();
	LCD_Clear(White);
	LCD_SetTextColor(Black);
	LCD_SetBackColor(White);

		
    while(1){		
			
			
			if((EXTI_Status%2 == 0)&&(EXTI_Status != 0)){
			if(TimingDelay > 1){
			LCD_DisplayStringLine(Line4,"      Long press");
			}
			else{
			LCD_DisplayStringLine(Line4,"      Short press");
			}
			EXTI_Status=0;
			}
		
			if(EXTI_Status%2 == 0){
				LCD_DisplayStringLine(Line1,"      Losing");
				TIM_Cmd(TIM2, DISABLE);
				ms_cnt = 0;
				TimingDelay = 0;
			}
			else{
				
				LCD_DisplayStringLine(Line1,"      Pressing");
				TIM_Cmd(TIM2, ENABLE);
				
			}
			
	
			sprintf(buff,"   Long Time:%ds",TimingDelay);	
			LCD_DisplayStringLine(Line0,buff);
			
			
		
		
	}
}



void In_init(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	/* Enable GPIOA clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
	//PA0-BUTTON1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

	/* Configure EXTI0 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI0 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}


/**
  * @˵��     ͨ�ö�ʱ��TIM3���ú���,PWM�������ģʽ
  * @����     none
  * @����ֵ   None
  */
void TIM_Config(uint16_t Channel2Pulse, uint16_t Channel3Pulse)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure; 
	
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	


		// ���ȼ�����
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn ;	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
    NVIC_Init(&NVIC_InitStructure);		
	
	
	TIM_TimeBaseStructure.TIM_Period = 1000-1;  //1ms������һ���ж�
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;//1Mhz	t = 1us
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	//ʹ��TIM2��ʱ������
	TIM_Cmd(TIM2, ENABLE);
	// �����������ж�
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);			
	
}	

/**
  * @˵��     PWM���ģʽ PA1(TIM3-CH2)��PA2(TIM3-CH3)��������
  * @����     None 
  * @����ֵ   None
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
