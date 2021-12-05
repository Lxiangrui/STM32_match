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
	SysTick_Config(SystemCoreClock/100000);	//һ���ж�10us��1khz��1ms100���жϣ�2khz��200���ж�
	Key_init();
	Led_init();
	Led_all_close();
	ADC_Config();
	
	STM3210B_LCD_Init();
	LCD_Clear(White);
	LCD_SetBackColor(White);
	LCD_SetTextColor(Black);

				
	
	while(1){

	Key_Set();										//��������PWM
	UUII();											  //LCD��ʾ
	}
}


void Delay_Ms(u32 nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}
