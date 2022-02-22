#include "key.h"
#include "stm32f10x.h"
void key_init(void){
		GPIO_InitTypeDef  GPIO_InitStructure;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    //B1¡¢B2°´¼üÒý½ÅÅäÖÃ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8| GPIO_Pin_4; // 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //B3¡¢B4°´¼üÒý½ÅÅäÖÃ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
		
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    //LEDÒý½ÅÅäÖÃ£¬PC08~PC15
    GPIO_InitStructure.GPIO_Pin = LED0 |LED1 | LED2 | LED3 | LED4 | LED5 | LED6\
                                  | LED7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //74HC573Ëø´æÒý½ÅÅäÖÃ£¬PD2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

	}
void LED_Control(uint16_t LED,uint8_t LED_Status)
{
    if(LED_Status == 0){
        GPIO_SetBits(GPIOC,LED);
        GPIO_SetBits(GPIOD,GPIO_Pin_2);
        GPIO_ResetBits(GPIOD,GPIO_Pin_2);  //×´Ì¬Ëø´æ
    }        
    else
    {
        GPIO_ResetBits(GPIOC,LED);
        GPIO_SetBits(GPIOD,GPIO_Pin_2);
        GPIO_ResetBits(GPIOD,GPIO_Pin_2);  //×´Ì¬Ëø´æ    
    }
}

    

