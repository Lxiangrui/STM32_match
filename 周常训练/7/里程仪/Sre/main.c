#include "stm32f10x.h"
#include "lcd.h"
#include "Fin.h"
#include "Pwm.h"
#include "RTC.h"
#include "key.h"
#include "EEPROM.h"

char buf[40];
u32 TimingDelay = 0;
u32 EXTI_Status = 0;
void Delay_Ms(u32 nTime);
extern int time_sys,FF;
extern int time_s;
int Vim = 2;

int distance = 0;
int TTTdistance;
int led_flag;

int FLAG;

//函数定义

void lcd_ui();
void key_data();
void baoj();



//Main Body
int main(void)
{
	SysTick_Config(SystemCoreClock/1000);
	IN_init();
	Pwm_init();
	RTC_Configuration();
	RTC_SetCounter(0);
	key_init();
	i2c_init();	
	
	STM3210B_LCD_Init();
	LCD_Clear(White);
	LCD_SetTextColor(Black);
	FF = 2;
				//	锁一下led，懒得写函数，因为调用的地方比较少
				GPIO_SetBits(GPIOC, GPIO_Pin_All);
				GPIO_SetBits(GPIOD,GPIO_Pin_2);
				GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	
	
	x24c02_write(0xff,0);
	Delay_Ms(2);
	
	


	while(1){
		
		if(Vim==0)		{					//判断是否有输入
			LCD_Clear(White);
			LCD_DisplayStringLine(Line4 ,"NO SINGAL");
			//保存本次公里数到eeprom，清空各个数据
			
			
			if(FLAG == 0){
					x24c02_write(0xff,distance);
					Delay_Ms(20);
						FLAG = 1;
			}
			
			
			RTC_SetCounter(0);
			distance = 0;
			FF = 2;
			
			LCD_Clear(White);
								}
		else{		
			if(FLAG == 1){
					time_s = 0;
					time_sys = 0;
					distance= 0;
						FLAG = 0;
			}
			
			
			FLAG = 0;
			lcd_ui();
			key_data();	
				}
		
		baoj();	
		
		}		
	
}

//
void Delay_Ms(u32 nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}
void lcd_ui(){
		sprintf(buf,"      Total(km):%.1f  ",((float)(x24c02_read(0xff)+distance)/1000));
		Delay_Ms(2);
		LCD_DisplayStringLine(Line9 ,buf);
	

//		sprintf(buf,"Vim(km/h):  %d %d    ",Vim*18/5,Vim/2);
		sprintf(buf,"Vim(km/h):  %.1f    ",(float)Vim*18/5);
		LCD_DisplayStringLine(Line1 ,buf);
		
		sprintf(buf,"Vavg(km/h): %.1f  ",(float)distance/time_s*18/5);
		LCD_DisplayStringLine(Line3 ,buf);
		
		sprintf(buf,"Time: %.2d:%.2d:%.2d  ",RTC_GetCounter()/3600,RTC_GetCounter()/60%60,RTC_GetCounter()%60);
		LCD_DisplayStringLine(Line5 ,buf);
		
		if(RTC_GetCounter()>86400)		RTC_SetCounter(0);
		
		sprintf(buf,"S(km): %.1f  ",(float)distance/1000);
		LCD_DisplayStringLine(Line7 ,buf);
}
void key_data(){
	//k1
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 0){
		Delay_Ms(100);
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 0);
		FF++;
	}
	//k2
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 0){
		Delay_Ms(100);
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 0);
		FF--;
	}


}

void baoj(){


if(((float)Vim*18/5)>90)		led_flag = 1;
				
		if(led_flag == 1){
			if(time_sys%1000 < 500){
					GPIO_SetBits(GPIOC, GPIO_Pin_All);
					GPIO_ResetBits(GPIOC, GPIO_Pin_8);
					GPIO_SetBits(GPIOD,GPIO_Pin_2);
					GPIO_ResetBits(GPIOD,GPIO_Pin_2);	
			}
			else{
					GPIO_SetBits(GPIOC, GPIO_Pin_All);
					GPIO_SetBits(GPIOD,GPIO_Pin_2);
					GPIO_ResetBits(GPIOD,GPIO_Pin_2);	
			
			}
		}}