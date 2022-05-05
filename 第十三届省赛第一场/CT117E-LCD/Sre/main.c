#include "stm32f10x.h"
#include "lcd.h"
#include "usart.h"
#include "key.h"
#include "pwm.h"
#include <string.h>
u32 TimingDelay = 0;

extern uint8_t RxBuffer1[50];
extern char RXover;
extern int time_s,time_ms;
int flag[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//1,切换显示标志位,只有为0时，才能输入密码
//2,记录密码输入错误的次数

void PSD_ui();
void STA_ui();
void LED_Ctro();
char True_keyword[3] = {'1','2','3'};
char Now_keyword[3] = {'@','@','@'};
char YY_usart_kw[3] = {0};

char buff[20];

void Delay_Ms(u32 nTime);
static __inline uint32_t SysTick_Config1(uint32_t ticks);
//Main Body
int main(void)
{
	int i = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SysTick_Config1(SystemCoreClock/1000);
	
	Init_led();
	Init_key();
	Init_usart();
	STM3210B_LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
	PSD_ui();
	Ctro_led(ledall,0);
	Init_pwm(1000,500);
	
	while(1){
	
	if(RXover == 1){	
			LCD_DisplayStringLine(Line5 ,RxBuffer1);
			YY_usart_kw[0] = RxBuffer1[0];
			YY_usart_kw[1] = RxBuffer1[1];
			YY_usart_kw[2] = RxBuffer1[2];
			
			if((YY_usart_kw[0] == True_keyword[0])&&(YY_usart_kw[1] == True_keyword[1])&&(YY_usart_kw[2] == True_keyword[2])){
					True_keyword[0] = RxBuffer1[4];
					True_keyword[1] = RxBuffer1[5];
					True_keyword[2] = RxBuffer1[6];
			}
			USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
			for(i = 0; i<50 ; i++)	RxBuffer1[i] = 0;	
			RXover = 0;
	}
	
		LED_Ctro();
	
	

	}
}


void LED_Ctro(){
	if(flag[0] == 1){
		//flag[0] = 1;
		Ctro_led(led0,1);
		Init_pwm(2000,200);
	if(time_s==5){			//关门
	Ctro_led(led0,0);
	flag[0] = 0;				//切换为输入密码模式
			Now_keyword[0] = '@';
			Now_keyword[1] = '@';
			Now_keyword[2] = '@';
	PSD_ui();
	Init_pwm(1000,500);
	}
	}
	
	
	if(flag[1] == 3){
	//
	if(time_s<5){
	if(time_ms%200 <100)	Ctro_led(led1,1);
	else	Ctro_led(led1,0);
	}
	else{ flag[1] = 2;
				Ctro_led(led1,0);
	}
	}
}
void STA_ui(){
	LCD_Clear(White);
	LCD_DisplayStringLine(Line2 ,"         STA");
	LCD_DisplayStringLine(Line4 ,"     F :2000hz");
	LCD_DisplayStringLine(Line5+12 ,"     D :10%");
	
}
void PSD_ui(){
	LCD_DisplayStringLine(Line2 ,"         PSD");
	sprintf(buff,"     B 1:%c          ",Now_keyword[0]);
	LCD_DisplayStringLine(Line4 ,buff);
	sprintf(buff,"     B 2:%c          ",Now_keyword[1]);
	LCD_DisplayStringLine(Line5+12 ,buff);
	sprintf(buff,"     B 3:%c          ",Now_keyword[2]);
	LCD_DisplayStringLine(Line6+24 ,buff);

}

void Delay_Ms(u32 nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}

void EXTI0_IRQHandler(void)
{
	Delay_Ms(50);
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    /* Toggle LED1 */
    //
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 0);
		if(flag[0] == 0){
			
		if(Now_keyword[0] == '@'){Now_keyword[0] = '0';}
		else Now_keyword[0]++;
		
		if(Now_keyword[0] > '9')	Now_keyword[0] = '0';
		
		PSD_ui();
		}
    /* Clear the  EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
		

}
void EXTI9_5_IRQHandler(void)
{
	Delay_Ms(50);
if(EXTI_GetITStatus(EXTI_Line8) != RESET)
  {
		
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 0);
		if(flag[0] == 0){
    if(Now_keyword[1] == '@'){Now_keyword[1] = '0';}
		else Now_keyword[1]++;
		
		PSD_ui();
		}
		if(Now_keyword[1]>'9')	Now_keyword[1] = '0';
    /* Clear the  EXTI line 8 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line8);
  }
		

	
}
void EXTI1_IRQHandler(void)
{
	Delay_Ms(50);
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
    /* Toggle LED1 */
		
    while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == 0);
		if(flag[0] == 0){
		if(Now_keyword[2] == '@'){Now_keyword[2] = '0';}
		else Now_keyword[2]++;
		if(Now_keyword[2]>'9')	Now_keyword[2] = '0';
		
		PSD_ui();
		}
    /* Clear the  EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line1);
  }
		

}
void EXTI2_IRQHandler(void)
{
	Delay_Ms(50);
  if(EXTI_GetITStatus(EXTI_Line2) != RESET)
  {
    /* Toggle LED1 */
    //
    while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2) == 0);
    /* Clear the  EXTI line 0 pending bit */
		
		if((True_keyword[0]==Now_keyword[0])&&(True_keyword[1]==Now_keyword[1])&&(True_keyword[2]==Now_keyword[2])){	
			flag[0] = 1;
			LCD_Clear(White);	
			flag[1] = 0;
		}
		else{
			Now_keyword[0] = '@';
			Now_keyword[1] = '@';
			Now_keyword[2] = '@';
			flag[1]++;	
		}
		time_s = 0;
    EXTI_ClearITPendingBit(EXTI_Line2);
  }
	if(flag[0]==0)		PSD_ui();
	else							STA_ui();
}
static __inline uint32_t SysTick_Config1(uint32_t ticks)
{
  if ((ticks - 1) > SysTick_LOAD_RELOAD_Msk)  return (1);      /* Reload value impossible */

  SysTick->LOAD  = ticks - 1;                                  /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, 1);  /* set Priority for Systick Interrupt */
  SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
  return (0);                                                  /* Function successful */
}

