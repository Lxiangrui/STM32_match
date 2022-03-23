#include "stm32f10x.h"
#include "stdio.h"
#include "lcd.h"
#include "rtc.h"
#include "adc.h"
#include "e2prom.h"
#include "i2c.h"
#include "key.h"
#include "usart.h"

u32 TimingDelay = 0;
u32 Time_adc;									//刷新adc值的时间
void Delay_Ms(u32 nTime);
char buf[50];									//显示存放的字符串
float k;											//比例
void key_FNC();								//按键功能
void lcd_UI_1();							//液晶界面
void lcd_UI_2();
int anjiancixu;


int UI_flag;
int BJ_flag;
int Time_RTC;									//rtc时间
int Time_RTC_flag;

int flog_once;

void	shangbao();
int shangbao_flag = 3;

	char str[40] ;							//串口相关
	int index;
	uint8_t USART_RXBUF[20];
	extern uint8_t RXOVER;
//	void send_string(char *sendstr);
//Main Body
int main(void)
{
	usart_init();																	//串口	
	adc_init();																		//adc初始化
	SysTick_Config(SystemCoreClock/1000);
	//RTV初始化
	RTC_Configuration();
	NVIC_Configuration();
	RTC_SetCounter(86395);
	//lcd初始化
	STM3210B_LCD_Init();
	LCD_Clear(White);
	LCD_SetBackColor(White);
	LCD_SetTextColor(Black);
	//eeprom初始化
	i2c_init();	
	if((x24c02_read(0x00)>=10)||(x24c02_read(0x00)<=0)){
		Delay_Ms(2);
		x24c02_write(0x00,1);
	}
	
	k = ((float)x24c02_read(0x00))/10;
	Delay_Ms(2);
//	if((k>=1)||(k<=0))	k = 99;
	//按键初始化
	key_led_init();
	
	
//	led_ctrol(GPIO_Pin_8,0);
	while(1){
		
		key_FNC();

		if(RXOVER == 1){
			k = ((float)(USART_RXBUF[3]-48))*0.1;
			x24c02_write(0x00,k*10);
			Delay_Ms(2);
			RXOVER = 0;
			USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
		
        USART_SendData(USART2,'o');
				Delay_Ms(10);
				USART_SendData(USART2,'k');
				Delay_Ms(10);
				USART_SendData(USART2,'\n');
				Delay_Ms(10);
			
		}
		
		if((Time_RTC == 0)&&(flog_once == 0)){
			shangbao();
			flog_once = 1;
		}
	
	
	}
}

//
void Delay_Ms(u32 nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}

void lcd_UI_1(){

	if(Time_adc%20==0){
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
		Delay_Ms(5);
		sprintf(buf,"  V:%.2fv     ",(ADC_GetConversionValue(ADC1)*3.30)/0xfff);
		LCD_DisplayStringLine(Line5 ,(char *)buf);
		}
		Time_RTC = RTC_GetCounter();
		if(Time_RTC>86399) RTC_SetCounter(0);	
		sprintf(buf,"  T:%.2d-%.2d-%.2d",Time_RTC/3600,Time_RTC/60%60,Time_RTC%60);
		LCD_DisplayStringLine(Line7 ,(char *)buf);
		
		sprintf(buf,"  K:%.1f     ",k);
		LCD_DisplayStringLine(Line3 ,(char *)buf);

}
void lcd_UI_2(){

	if(Time_adc%500 < 250){			//利用systick的变量闪烁光标
		LCD_DisplayStringLine(Line3 ,"      Setting");
		sprintf(buf,"     %.2d-%.2d-%.2d",Time_RTC/3600,Time_RTC/60%60,Time_RTC%60);
		LCD_DisplayStringLine(Line6 ,(char *)buf);
	}
	else{
	//光标闪烁
		if(Time_RTC_flag == 0){
		sprintf(buf,"     %.2d-%.2d-__",Time_RTC/3600,Time_RTC/60%60);
		LCD_DisplayStringLine(Line6 ,(char *)buf);
		}
		else if(Time_RTC_flag == 1){
		sprintf(buf,"     %.2d-__-%.2d",Time_RTC/3600,Time_RTC%60);
		LCD_DisplayStringLine(Line6 ,(char *)buf);
		}
		else{
		sprintf(buf,"     __-%.2d-%.2d",Time_RTC/60%60,Time_RTC%60);
		LCD_DisplayStringLine(Line6 ,(char *)buf);
		}}
}


void key_FNC(){
	float kk;
		if(UI_flag%2 == 0){
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0){		//打开或关闭报警
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0);
				BJ_flag++;
	}
	if(BJ_flag%2 == 0){	LCD_DisplayStringLine(Line1 ,"  LED:ON   ");
										kk = k*3.3;
										if((ADC_GetConversionValue(ADC1)*3.30)/0xfff >= kk){
											if(Time_adc%200<100){
												led_ctrol(GPIO_Pin_All,1);
												led_ctrol(GPIO_Pin_8,0);
											}
											else							 led_ctrol(GPIO_Pin_All,1);}
										else	led_ctrol(GPIO_Pin_All,1);
	}
	else{								LCD_DisplayStringLine(Line1 ,"  LED:OFF   ");
											led_ctrol(GPIO_Pin_All,1);
	}}
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 0){		//切换界面
		Delay_Ms(100);
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 0);
		UI_flag++;
		
		shangbao_flag = 0;
		
		LCD_Clear(White);
		RTC_SetCounter(Time_RTC);
	}
	if(UI_flag%2 == 1){	
		lcd_UI_2();
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0){
		Delay_Ms(100);
			while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);
			Time_RTC_flag++;
			if(Time_RTC_flag>2)	Time_RTC_flag = 0;
		}
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2) == 0){
		Delay_Ms(100);
			anjiancixu = 0;
			
			while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2) == 0);
			if(Time_RTC_flag == 0){
				if(anjiancixu>100) Time_RTC++;
				else	Time_RTC--;
			}
			else if(Time_RTC_flag == 1){
				if(anjiancixu>100) Time_RTC+=60;
				else	Time_RTC-=60;
			}
			else{
				if(anjiancixu>100) Time_RTC+=3600;
				else	Time_RTC-=3600;
			}
			if(Time_RTC>86399)	Time_RTC=0;			//时间限制
			else if(Time_RTC<0)	Time_RTC=86400;
		}
	}
	else{		lcd_UI_1();
					
		if(shangbao_flag == 0){
			shangbao();
			shangbao_flag = 1;
		}
		
		
	}
	
}

void	shangbao(){
		
				index = 0;
				sprintf(buf,"%.2f+%.2f+%.2d:%.2d:%.2d\nx",((float)(ADC_GetConversionValue(ADC1)*3.30))/0xfff,k,Time_RTC/3600,Time_RTC/60%60,Time_RTC%60);
				for(;buf[index] != 'x'; index++){
						USART_SendData( USART2, buf[index]);
						Delay_Ms(10);
					}

}

