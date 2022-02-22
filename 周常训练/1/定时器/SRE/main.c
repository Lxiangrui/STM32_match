#include <stdio.h> 
#include "stm32f10x.h"
#include "lcd.h"
uint8_t ADC_Flag;
void ADC_Config(void);
float Read_ADC(void);

uint32_t TimingDelay = 0;
uint8_t EXTI_Status = 0;

void Delay_Ms(uint32_t nTime);
void EXTI_Config(void);


u8 buf[50]; 
int H,M,S,R_TIME=86400;
int L_TIME;
int flog;
int L_folg=1;
void MAIN_UI(void){
	
	S=R_TIME%60;
	M=(R_TIME/60)%60;
	H=R_TIME/3600;
	sprintf((char*)buf,"     T:%.2d-%.2d-%.2d          ",H,M,S);		
	LCD_DisplayStringLine(Line6,buf);	
	
}
void SET_UI(void){
	LCD_DisplayStringLine(Line3,"       Setting");
	S=L_TIME%60;
	M=(L_TIME/60)%60;
	H=L_TIME/3600;
	
	if((flog==2)){
					sprintf((char*)buf,"     T:%.2d-%.2d- _",H,M);	
					LCD_DisplayStringLine(Line6,buf);
					}
					else if((flog==3)){
					sprintf((char*)buf,"     T:%.2d- _-%.2d",H,S);	
					LCD_DisplayStringLine(Line6,buf);
					}
					else if((flog==4)){
					sprintf((char*)buf,"     T: _-%.2d-%.2d",M,S);	
					LCD_DisplayStringLine(Line6,buf);
					}
	Delay_Ms(100);
	sprintf((char*)buf,"     T:%.2d-%.2d-%.2d",H,M,S);	
	LCD_DisplayStringLine(Line6,buf);	
	Delay_Ms(100);
}
int main(void)
{   
	float adc_temp;
	uint8_t  string[20];  //ADC结果
	
    SysTick_Config(SystemCoreClock/1000);  //1ms中断一次
		
		ADC_Config();
	
	
	EXTI_Config();
	
	//LCD工作模式配置
	STM3210B_LCD_Init();
	LCD_Clear(White);
	LCD_SetTextColor(Black);
	LCD_SetBackColor(White);
	
	MAIN_UI();//初始界面
	
    while(1){
			
				if(flog==0){
			ADC_Flag = 0;
			adc_temp = Read_ADC();
			sprintf(string,"%s%.3f","     V1:",adc_temp);
			LCD_DisplayStringLine(Line2,string);
					
					
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0){
				LCD_DisplayStringLine(Line4,"     LED:NO  ");
			}
			else
			{
				LCD_DisplayStringLine(Line4,"     LED:YES  ");
			}
		}	
			
		
		
		switch(EXTI_Status)
		{
			case 1:
				LCD_Clear(White);
				flog++;
				
			
				break;
			case 2:
					LCD_Clear(White);
					if((flog>=1)&&(flog<=4)){
					if((flog==2)){
					L_TIME++;
					}
					else if((flog==3)){
					L_TIME=L_TIME+60;
					}
					else if((flog==4)){
					L_TIME=L_TIME+3600;
					}
				}
				break;
		}
		EXTI_Status=0;				//清空按键返回值
		
		if(L_TIME<0)
		{
		L_TIME=86399;
		}
		if(R_TIME<0)
		{
		R_TIME=86400;
		}
		L_TIME=L_TIME%86400;
		R_TIME=R_TIME%86400;
		if((flog>=1)&&(flog<=4)){
		SET_UI();
		if((flog==1)&&(L_folg))//按下k1时，保存时间
		{
		L_TIME=R_TIME;
			L_folg=0;						//标志位置0，防止下次按下k1时保存时间。
		}	
		}
		else if(flog==0)
		{
		MAIN_UI();
			L_folg=1;						//标志位置1，下次按下k1，保存时间
		}
		else if(flog>4){
			flog=0;							//标志位置0，循环回去
			R_TIME=L_TIME;			//将设置好的时间变量赋值给活着的时间变量
		}
		
		Delay_Ms(100);
	}         
}



































/**
  * @说明     配置相关引脚为EXTI工作模式
  * @参数     none
  * @返回值   None
  */
void EXTI_Config(void)
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
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI0 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//PA8-BUTTON2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);
	
	/* Configure EXTI9_5 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI0 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



/**
  * @说明     ADC时钟源及工作模式配置
  * @参数     none
  * @返回值   None
  */
void ADC_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//PB0-ADC channel 8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// ADC1 工作模式配置
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  //单次转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_13Cycles5);    

	ADC_Cmd(ADC1, ENABLE);   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));
}

/**
  * @说明     读取ADC转换结果
  * @参数     none
  * @返回值   ADC_VALUE:ADC转换结果
  */

float Read_ADC(void)
{
	float ADC_VALUE;
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	Delay_Ms(5);
	ADC_VALUE = ADC_GetConversionValue(ADC1)*3.30/0xfff;
	
	return ADC_VALUE;
}







/**
  * @说明     延时函数
  * @参数     nTime: 延时时间
  * @返回值   None
  */
void Delay_Ms(uint32_t nTime)
{
    TimingDelay = nTime;
    while(TimingDelay != 0);
}
/******************************************END OF FILE*************************/
