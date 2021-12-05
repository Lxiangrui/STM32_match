#include "lcd.h"
#include "UI.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "key.h"
char read[9];
int ADC_Flag;
extern int flag_key_1;
extern int flag_key_4;
int Mode;
int UI_q;					//切换数据显示或者参数显示
int duty1,duty2;

void UUII(){
	if(flag_key_1%2 == 0){
		if((flag_key_4%2 ==0)){
		LCD_DisplayStringLine(Line0,(unsigned char *)"        Data");
				sprintf((char*)read,"      V: %.2fv",Read_ADC()*3.3/100);
		LCD_DisplayStringLine(Line3,(unsigned char *)read);
		LCD_DisplayStringLine(Line6,(unsigned char *)"      Mode: AUTO");
		
		duty1=(int)Read_ADC();
		duty2=(int)Read_ADC();		//现在的adc输出的是真实电压的10/3.3倍，所以可以可以直接赋值
			
			
				Led_all_close();
				GPIO_ResetBits(GPIOC,GPIO_Pin_8);
				GPIO_SetBits(GPIOD,GPIO_Pin_2);
        GPIO_ResetBits(GPIOD,GPIO_Pin_2); 
		}
		else{
		LCD_DisplayStringLine(Line0,(unsigned char *)"        Data");
		sprintf((char*)read,"      V: %.2fv",Read_ADC()*3.3/100);
		LCD_DisplayStringLine(Line3,(unsigned char *)read);
		LCD_DisplayStringLine(Line6,(unsigned char *)"      Mode: MANU");
			
				Led_all_close();
				GPIO_ResetBits(GPIOC,GPIO_Pin_9);
				GPIO_SetBits(GPIOD,GPIO_Pin_2);
        GPIO_ResetBits(GPIOD,GPIO_Pin_2); 
	
			
		}
		UI_q=0;
	}
	else{
		if((flag_key_4%2 ==0)){							//自动模式
			
		duty1=(int)Read_ADC();
		duty2=(int)Read_ADC();
				
		}
		else{																//手动模式
		if(RB2 == KEY_ON){
		LCD_Clear(White);
		duty1=duty1+10;
		if(duty1>90){duty1=0;}
			
	
	}	
		if(RB3 == KEY_ON){
		LCD_Clear(White);
		duty2=duty2+10;
			if(duty2>90){duty2=0;}
	}	
		
			
		}
		LCD_DisplayStringLine(Line0,(unsigned char *)"        Para");
		sprintf((char*)read,"        PA6:%d   ",duty1);
		LCD_DisplayStringLine(Line3,(unsigned char *)read);
		sprintf((char*)read,"        PA7:%d   ",duty2);
		LCD_DisplayStringLine(Line6,(unsigned char *)read);
		UI_q=1;}
	if(UI_q == 0){	
	
	}
	
	
}

float Read_ADC(void)
{
	float ADC_VALUE;
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	ADC_VALUE = ADC_GetConversionValue(ADC1)*100/0xfff;
	return ADC_VALUE;
}


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