#include "key.h"
#include "stm32f10x.h"
#include "lcd.h"
int flag_key_1;
int flag_key_4;

uint16_t Channel2Pulse = 0, Channel3Pulse = 0;

void Key_init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
		//B1¡¢B2°´¼üÒý½ÅÅäÖÃ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8; // 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //B3¡¢B4°´¼üÒý½ÅÅäÖÃ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	
}

uint16_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON)
	{
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);
		return KEY_ON;
	}
	else
		return KEY_OFF;
}

void Key_Set(void){
	if(RB1 == KEY_ON){
	LCD_Clear(White);
	flag_key_1++;
	
	}
	
	if(RB4 == KEY_ON){
	LCD_Clear(White);
	flag_key_4++;
	
	}
	

}
void Led_init(void){
		GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
		//LEDÒý½ÅÅäÖÃ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15; // 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //Ëø´æÆ÷Òý½Å
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

}				

void Led_all_close(){
				GPIO_SetBits(GPIOC,GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15);
				GPIO_SetBits(GPIOD,GPIO_Pin_2);
        GPIO_ResetBits(GPIOD,GPIO_Pin_2); 
}



