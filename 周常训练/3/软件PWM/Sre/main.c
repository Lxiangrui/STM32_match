#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "key.h"
#include "lcd.h"
#include "USART.h"
#include "i2c.h"

extern uint8_t USART_RXBUF[22];
extern uint8_t RXOVER;
extern int k;
int time_s;			//秒钟，时刻在变化
int temp;
int temp2;


u32 TimingDelay = 0;
char buff[50];
void Delay_Ms(u32 nTime);

void button_1();
void button_2();
void button_3();
void button_4();
int button_1_flag;
int button_2_flag;
int button_3_flag;
int button_4_flag;


int channel_1=800;
int channel_2=100;


void UI_1();
void UI_2();

//eeprom函数
uint8_t x24c02_read(uint8_t address);
void x24c02_write(uint8_t address,uint8_t info);
//调用写好的函数


//Main Body

int main(void)
{
	

//	if(k==1000)	time_s=1;
//	if(time_s>=500)   //占空比%50,可改
//		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
//	
//	else
//		GPIO_SetBits(GPIOA,GPIO_Pin_2);

	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	SysTick_Config(SystemCoreClock/1000);
	
	key_init();
	
	USART_Config();
	
	STM3210B_LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);

	i2c_init();	
	
	temp = x24c02_read(0x00);
	Delay_Ms(2);
	temp2 = x24c02_read(0xff);
	Delay_Ms(2);

	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	while(1){
		
	

		sprintf(buff,"%s%d","time_s:",time_s);
	LCD_DisplayStringLine(Line8,buff);
		button_1();
		button_2();
		button_3();
		button_4();
		if(RXOVER){
	
			sprintf(buff,"%s","time_s:");
	LCD_DisplayStringLine(Line1,buff);
		USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
		RXOVER = 0;
		
		}
		
		
		
	temp = x24c02_read(0x00);
	Delay_Ms(2);
	sprintf(buff,"%s%d%%     ","PA1,VAL:",temp);
	LCD_DisplayStringLine(Line9,buff);
		
		
	temp2 = x24c02_read(0xff);
	Delay_Ms(2);
	sprintf(buff,"%s%d%%     ","PA2,VAL:",temp2);
	LCD_DisplayStringLine(Line7,buff);
	
	
	
	}

	}

	
	
	
	
	
	
	

void UI_1(){}
void UI_2(){}
	
	void button_1(){
	if(RB1==0){
			Delay_Ms(100);
			if(RB1==0){
			button_1_flag=button_1_flag+1;
			sprintf(buff,"%s%d","button_1_flag:",button_1_flag);
			LCD_DisplayStringLine(Line1,buff);
				if(button_1_flag%2==0){//这是奇数次按下时，执行的程序，输出PWM
				


			}
				else{}//注意是只让PA1输出低电平，所以不能用定时器失能
			
			
			
			}}}
	void button_2(){
	if(RB2==0){
			Delay_Ms(100);
			if(RB2==0){
			button_2_flag=button_2_flag+1;
			sprintf(buff,"%s%d","button_2_flag:",button_2_flag);
			LCD_DisplayStringLine(Line2,buff);
			channel_1=x24c02_read(0x00)*10;
			channel_1=channel_1+100;
				
				
				
				
				
				if(channel_1>1000){channel_1=0;}
			x24c02_write(0x00,channel_1/10);			//因为eeperom现在还不能存几百的数字
			Delay_Ms(2);
			
			}}}
	void button_3(){
	if(RB3==0){
			Delay_Ms(100);
			if(RB3==0){
			button_3_flag=button_3_flag+1;
			sprintf(buff,"%s%d","button_3_flag:",button_3_flag);
			LCD_DisplayStringLine(Line3,buff);
			if(button_3_flag%2==0){//这是奇数次按下时，执行的程序，输出PWM
			
	
			}
			else{}
			
			
			}}}
	void button_4(){
	if(RB4==0){
			Delay_Ms(100);
			if(RB4==0){
			button_4_flag=button_4_flag+1;
			sprintf(buff,"%s%d","button_4_flag:",button_4_flag);
			LCD_DisplayStringLine(Line4,buff);
			channel_2=x24c02_read(0xff)*10;
			channel_2=channel_2+100;
				if(channel_2>1000){channel_2=0;}
			x24c02_write(0xff,channel_2/10);			//因为eeperom现在还不能存几百的数字
			Delay_Ms(2);
				
				

				
				
			}}}
	
	
	
	
	
	
	
	
	
	


/**
  * @说明     从AT24C02指定地址读出一个字节数据
  * @参数     address:AT24C02内部存储地址
  * @返回值   val:读出数据
  */
uint8_t x24c02_read(uint8_t address)
{
	unsigned char val;
	
	I2CStart(); 
	I2CSendByte(0xa0);
	I2CWaitAck(); 
	
	I2CSendByte(address);
	I2CWaitAck(); 
	
	I2CStart();
	I2CSendByte(0xa1); 
	I2CWaitAck();
	val = I2CReceiveByte(); 
	I2CWaitAck();
	I2CStop();
	
	return(val);
}

/**
  * @说明     向AT24C02指定地址写入一个字节数据
  * @参数     address:AT24C02内部存储地址
  * @参数     info:写入数据
  * @返回值   None
  */
void x24c02_write(unsigned char address,unsigned char info)
{
	I2CStart(); 
	I2CSendByte(0xa0); 
	I2CWaitAck(); 
	
	I2CSendByte(address);	
	I2CWaitAck(); 
	I2CSendByte(info); 
	I2CWaitAck(); 
	I2CStop();
}

	
	

void Delay_Ms(u32 nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}
