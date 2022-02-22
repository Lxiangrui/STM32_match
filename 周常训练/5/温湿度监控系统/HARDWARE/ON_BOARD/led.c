
/**
  * @说明     LED 相关GPIO引脚工作模式及时钟源配置
  * @参数     None 
  * @返回值   None
  */
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    //LED引脚配置，PC08~PC15
    GPIO_InitStructure.GPIO_Pin = LED0 |LED1 | LED2 | LED3 | LED4 | LED5 | LED6\
                                  | LED7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //74HC573锁存引脚配置，PD2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/**
  * @说明     控制LED打开或关闭
  * @参数     Led: LED编号,GPIO_Pin_8到GPIO_Pin_15
  * @参数     Ledstatus: 0，关闭LED；1，打开LED
  * @返回值   None
  */
void LED_Control(uint16_t LED,uint8_t LED_Status)
{
    if(LED_Status == 0){
        GPIO_SetBits(GPIOC,LED);
        GPIO_SetBits(GPIOD,GPIO_Pin_2);
        GPIO_ResetBits(GPIOD,GPIO_Pin_2);  //状态锁存
    }        
    else
    {
        GPIO_ResetBits(GPIOC,LED);
        GPIO_SetBits(GPIOD,GPIO_Pin_2);
        GPIO_ResetBits(GPIOD,GPIO_Pin_2);  //状态锁存    
    }
}