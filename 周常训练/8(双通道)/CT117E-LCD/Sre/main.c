#include "stm32f10x.h"
#include "lcd.h"
#include "pwm.h"
#include "i2c.h"

#define RB1	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define RB2	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)
#define RB3 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define RB4 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)
#define LED0    GPIO_Pin_8
#define LED1    GPIO_Pin_9
#define LED2    GPIO_Pin_10
#define LED3    GPIO_Pin_11
#define LED4    GPIO_Pin_12
#define LED5    GPIO_Pin_13
#define LED6    GPIO_Pin_14
#define LED7    GPIO_Pin_15
#define LEDALL	GPIO_Pin_All


u32 TimingDelay = 0;

extern int OUT_TT;							//khz

char buf[50];

int	flag_12 ;	//切换12模式

int flag_chanl;

int n1,n2;

void Delay_Ms(u32 nTime);
void TIME_INPUT(void);
void Key_Init(void);
void LED_Control(uint16_t LED,uint8_t LED_Status);
void LED_Init(void);


uint8_t USART_RXBUF[20];
extern uint8_t RXOVER;
void NVIC_Configuration(void);
void USART_Config(void);
void USART_SendString(int8_t *str);
uint8_t x24c02_read(uint8_t address);
void x24c02_write(unsigned char address,unsigned char info);



extern int Frequency ;

int Frequency2;

//Main Body
int main(void)
{
	int i;
	SysTick_Config(SystemCoreClock/1000);
	USART_Config();
	STM3210B_LCD_Init();
	LCD_Clear(White);
	LCD_SetTextColor(Black);
	
	i2c_init();
	
	OUT_TT = 2;		//单位 ms	,最低频率好像不多达到 50hz
	
	TIM3_Configuration();
	
	TIME_INPUT();
	Key_Init();
	LED_Init();
  LED_Control(LEDALL,0);
	
	
	n1=x24c02_read(0x11);
	Delay_Ms(2);
	n2=x24c02_read(0x12);
	Delay_Ms(2);
	
	
		if(n1>=10)	n1 = 10;
		else if(n1<=0)	n1 = 0;
		
		if(n2>=10)	n2 = 10;
		else if(n2<=0)	n2 = 0;
		
		
	while(1){
		
		x24c02_write(0x11,n1);
		Delay_Ms(2);
		x24c02_write(0x12,n2);
		Delay_Ms(2);
		
		//串口接受处理部分
		if(RXOVER == 1){

			if((flag_12%2 == 1)){
			n1 = USART_RXBUF[4] - 48;
			n2 = USART_RXBUF[6]	- 48;
			
			
			if(n2 == 1)	n2 = 10;
			}
			
			
			for(i=0;i<20;i++){
				USART_RXBUF[i] = 0;  //清空接收区
			}
			RXOVER = 0;
			USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
		}		
		
		
		
		
		//控制led
		if((Frequency != 0)&&(Frequency2 != 0)){
			LED_Control(LEDALL,0);
			LED_Control(LED0,0);
			LED_Control(LED1,0);
		}
		else if((Frequency != 0)&&(Frequency2 == 0)){
			LED_Control(LEDALL,0);
			LED_Control(LED0,1);
			LED_Control(LED1,0);
		}
		else	LED_Control(LEDALL,0);

		
		
		
		//显示输入频率		
				sprintf(buf," Channel:%dHZ",Frequency);
				LCD_DisplayStringLine(Line1, buf);
				sprintf(buf," Channe2:%dHZ",Frequency2);
				LCD_DisplayStringLine(Line5, buf);
				
		//输出倍数
				sprintf(buf," N(1):%d  ",n1);
				LCD_DisplayStringLine(Line3, buf);
				sprintf(buf," N(2):%d  ",n2);
				LCD_DisplayStringLine(Line7, buf);
		
		
		
		//改变模式
				if(RB1 == 0){	
					Delay_Ms(100);
					while(RB1 == 0);
					flag_12 ++;
				}
				
				if(flag_12%2 == 0)	LCD_DisplayStringLine(Line8, "               1");
				else 							LCD_DisplayStringLine(Line8, "               2");
			
				
			//按键2，3，4
			if(flag_12%2 == 0){	///1模式下
				if(RB2 == 0){
					Delay_Ms(100);
					while(RB2 == 0);
					flag_chanl++;}
				if(flag_chanl%2 == 0){		//通道1
					if(RB3 == 0){
						Delay_Ms(100);
						while(RB3 == 0);
							n1++;
				}
				if(RB4 == 0){
					Delay_Ms(100);
					while(RB4 == 0);
							n1--;
				}
				
			}

			else{								//通道2
			if(RB3 == 0){
						Delay_Ms(100);
						while(RB3 == 0);
							n2++;
				
				}
				if(RB4 == 0){
					Delay_Ms(100);
					while(RB4 == 0);
							n2--;
				}
			
			}
			//限制n1\n2
			if(n1<=0)	n1 = 1;
			if(n1>=10)	n1 = 10;
			if(n2<=0)	n2 = 1;
			if(n2>=10)	n2 = 10;
			
	}
			else{							//2模式下,需要做的事情有
			
			
			}
}
}
//
void Delay_Ms(u32 nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}




















void TIME_INPUT(void)
{
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* GPIOA clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	

  /* TIM2 channel 2 pin (PA.07) configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  /* Enable the TIM2 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2|TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

  TIM_PWMIConfig(TIM2, &TIM_ICInitStructure);

  /* Select the TIM2 Input Trigger: TI2FP2 */
  TIM_SelectInputTrigger(TIM2, TIM_TS_TI2FP2);

  /* Select the slave Mode: Reset Mode */
  TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);

  /* Enable the Master/Slave Mode */
  TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);

  /* TIM enable counter */
  TIM_Cmd(TIM2, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);

}



/**
  * @说明     按键接口初始化函数
  * @参数     none
  * @返回值   None
  */
void Key_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    //B1、B2按键引脚配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8; // 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //B3、B4按键引脚配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    //LED引脚配置，PC08~PC15
    GPIO_InitStructure.GPIO_Pin = LED1 | LED2 | LED3 ;
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
void USART_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
		NVIC_Configuration();
    //配置USART2 TX引脚工作模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //配置USART2 RX引脚工作模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //串口2工作模式配置
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
	
		USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART2, ENABLE);
}

/**
  * @说明     USART2字符串发送函数
  * @参数     str: 指向字符串的指针
  * @返回值   None
  */
void USART_SendString(int8_t *str)
{
    uint8_t index = 0;
    
    do
    {
        USART_SendData(USART2,str[index]);
        while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
        index++;        
    }
    while(str[index] != 0);  //检查字符串结束标志
    
}

/**
  * @说明     配置中断向量控制器
  * @参数     None
  * @返回值   None
  */
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
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