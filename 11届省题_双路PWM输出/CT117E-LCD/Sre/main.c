#include "stm32f10x.h"
#include "lcd.h"
#include "UI.h"
#include "key.h"
#include "stdio.h"
u32 TimingDelay = 0;

void Delay_Ms(u32 nTime);
extern int duty1,duty2;


int main(void)
{
	SysTick_Config(SystemCoreClock/100000);	//一次中断10us，1khz是1ms100个中断，2khz是200个中断
	Key_init();
	Led_init();
	Led_all_close();
	ADC_Config();
	
	STM3210B_LCD_Init();
	LCD_Clear(White);
	LCD_SetBackColor(White);
	LCD_SetTextColor(Black);

				
	
	while(1){

	Key_Set();										//按键控制PWM
	UUII();											  //LCD显示
	}
}


void Delay_Ms(u32 nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}
