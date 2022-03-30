#include "stm32f10x.h"
#include "lcd.h"

#define RB1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define RB2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)
#define RB3 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define RB4 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)

#define LED1 GPIO_Pin_8
#define LED2 GPIO_Pin_9
#define LED3 GPIO_Pin_10
#define LED4 GPIO_Pin_11
#define LED5 GPIO_Pin_12
#define LED6 GPIO_Pin_13
#define LED7 GPIO_Pin_14
#define LED8 GPIO_Pin_15
#define LEDALL GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15

u32 TimingDelay = 0;
float vule;
char buff[40];
char UI_flag;
float Max = 2.4,Min = 1.2;
char Uled = 1,Dled = 2;
int hang = Line3;
char flog11;

void Delay_Ms(u32 nTime);
void Init_adc();

void	UI_main();
void	UI_set();

void Init_key_led();
void SET_key();
void chang_UI();
void Limi_data();

void moto();
void led_control(uint16_t Pin,char statu);
void led_change();

//Main Body
int main(void)
{
	SysTick_Config(SystemCoreClock/1000);

	Init_adc();
	Init_key_led();
	STM3210B_LCD_Init();
	LCD_Clear(White);
	LCD_SetTextColor(Black);
	
	//led_control(GPIO_Pin_8,1);
	
	while(1){
		//Delay_Ms(100);
		
		chang_UI();
		SET_key();
		led_change();
	
	}
}


//
void led_change(){
	int LED111,LED222;
	led_control(LEDALL,0);
	if(vule<Min){
		LED111 = 256*(2^(Dled-1));
		
		led_control(LED111,1);
	}
	if(vule>Max){
		LED222 =256*(2^(Uled-1));
		
		led_control(LED222,1);
	}
	if((vule>Min)&&(vule<Max)){
		led_control(LEDALL,0);
	}

}
void led_control(uint16_t Pin,char statu){
		if(statu == 1){
			GPIO_ResetBits(GPIOC, Pin);
				GPIO_SetBits(GPIOD, GPIO_Pin_2);
				GPIO_ResetBits(GPIOD, GPIO_Pin_2);
			
		}	
		else {
			GPIO_SetBits(GPIOC, Pin);
				GPIO_SetBits(GPIOD, GPIO_Pin_2);
				GPIO_ResetBits(GPIOD, GPIO_Pin_2);
		}

}


void moto(){

	if(vule>Max)	LCD_DisplayStringLine(Line6 ,"  Status:Upper");
	else if(vule<Min)	LCD_DisplayStringLine(Line6 ,"  Status:Lower");
	else	LCD_DisplayStringLine(Line6 ,"  Status:Nomal");


}
void SET_key(){
	if(UI_flag%2 == 1){
		if(RB2 == 0){
		while(RB2 == 0);
			LCD_Clear(White);
			hang += 24;
			flog11 = 1;
		}
		
		if(hang>Line6)	hang = Line3;
//			LCD_SetBackColor(Green);
//			LCD_DisplayStringLine(hang ,"                      ");
//			LCD_SetBackColor(White);
		
//		if(RB3 == 0){
//		while(RB3 == 0);
//		LCD_DisplayStringLine(Line4 ,"123");
//			flog11 = 1;
////		}
//		if(RB4 == 0){
//		while(RB4 == 0);
//		LCD_DisplayStringLine(Line5 ,"123");
//			flog11 = 1;
//		}
	
		
		if(flog11 == 1){
			if(hang == Line3)	LCD_SetBackColor(Green);
	sprintf(buff,"Max volt:%.2f",Max);		
	LCD_DisplayStringLine(Line3 ,buff);
			LCD_SetBackColor(White);
			if(hang == Line4)	LCD_SetBackColor(Green);
	sprintf(buff,"Min volt:%.2f",Min);
	LCD_DisplayStringLine(Line4 ,buff);
			LCD_SetBackColor(White);
			if(hang == Line5)	LCD_SetBackColor(Green);
	sprintf(buff,"Upper:LD%d",Uled);
	LCD_DisplayStringLine(Line5 ,buff);
			LCD_SetBackColor(White);
			if(hang == Line6)	LCD_SetBackColor(Green);
	sprintf(buff,"Lower:LD%d",Dled);
	LCD_DisplayStringLine(Line6 ,buff);
			LCD_SetBackColor(White);
			
			
			LCD_DisplayStringLine(Line3 ,"Max volt:");
			LCD_DisplayStringLine(Line4 ,"Min volt:");
			LCD_DisplayStringLine(Line5 ,"Upper:");
			LCD_DisplayStringLine(Line6 ,"Lower:");
			
			
			
			
	flog11 = 0;
	}
		
	
		if(hang == Line3)
	{
		if(RB3 == 0){
			Delay_Ms(10);
		while(RB3 == 0);
			Max+=0.3;
			flog11 = 1;
		}
		if(RB4 == 0){
			Delay_Ms(10);
		while(RB4 == 0);
			Max-=0.3;
			flog11 = 1;
		}
		
	
	}
	if(hang == Line4)
	{
		if(RB3 == 0){
			Delay_Ms(10);
		while(RB3 == 0);
			Min+=0.3;
			flog11 = 1;
		}
		if(RB4 == 0){
			Delay_Ms(10);
		while(RB4 == 0);
			Min-=0.3;
			flog11 = 1;
		}
	
	}
	if(hang == Line5)
	{
		
		if(RB3 == 0){
			Delay_Ms(10);
		while(RB3 == 0);
			Uled++;
			flog11 = 1;
		}
		if(RB4 == 0){
			Delay_Ms(10);
		while(RB4 == 0);
			Uled--;
			flog11 = 1;
		}
	}
	if(hang == Line6)
	{
		if(RB3 == 0){
			Delay_Ms(10);
		while(RB3 == 0);
			Dled++;
			flog11 = 1;
		}
		if(RB4 == 0){
			Delay_Ms(10);
		while(RB4 == 0);
			Dled--;
			flog11 = 1;
		}
	}
	Limi_data();
	}
	
	

	
	

	
	
}
void chang_UI()
{
	if(RB1 == 0){
		while(RB1 == 0);
		UI_flag++;
		LCD_Clear(White);
		flog11 = 1;
	}
	if(UI_flag%2 == 0)	UI_main();
	else								UI_set();
	
}
void Delay_Ms(u32 nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}
void Init_adc()
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	 /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel14 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);

  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
     
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
}
void	UI_main(){
	LCD_DisplayStringLine(Line2 ,"       MAIN");
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
		vule = (float)(ADC_GetConversionValue(ADC1))/4095*3.3;
		
		sprintf(buff,"  Volt:%.2f",vule);
		LCD_DisplayStringLine(Line4 ,buff);
		moto();
}
void	UI_set(){
	LCD_DisplayStringLine(Line2 ,"       SETING");
	
	

}
void Init_key_led(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	

}
void Limi_data(){
	if(Uled<1)	Uled = 1;
	else if(Uled>8)	Uled = 8;
	
	if(Dled<1)	Dled = 1;
	else if(Dled>8)	Dled = 8;

	if(Max<0)	Uled = 0;
	else if(Max>3.3)	Max = 3.3;
	if(Min<0)	Min = 0;
	else if(Min>3.3)	Max = 3.3;
	
}