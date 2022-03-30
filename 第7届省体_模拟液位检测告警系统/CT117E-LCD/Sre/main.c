#include "stm32f10x.h"
#include "lcd.h"
#include "i2c.h"

#define B1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define B2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)
#define B3 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define B4 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)

#define LED1 GPIO_Pin_8
#define LED2 GPIO_Pin_9
#define LED3 GPIO_Pin_10
#define LED4 GPIO_Pin_11
#define LED5 GPIO_Pin_12
#define LED6 GPIO_Pin_13
#define LED7 GPIO_Pin_14
#define LED8 GPIO_Pin_15
#define LED_all GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8

u32 TimingDelay = 0;
 int time;
 int time222;
char buff[30];
void Ui_set();
float valu;
float valu_1;
char UI_flag;
char LINE = Line3;
int T1 = 30 ,T2 = 50,T3 = 70;
char floagg;
 char RxBuffer2[50];
 char RX_flag;
 char RX_char;
int i;
int status;
float aaa;//是上电时，由于电压的突然升高不会上报
int histroy_flag;	//用于记录前后adc

void Delay_Ms(u32 nTime);
void 	Adc_init();
void Ui_main();
void Usart_init();
void Usart_send(char *bufff);

uint8_t x24c02_read(uint8_t address);
void x24c02_write(unsigned char address,unsigned char info);

void Key_init();
void Key_control();
void led_ctrol(uint16_t GPIO_Pin,char sss);
//Main Body
int main(void)
{
	SysTick_Config(SystemCoreClock/1000);
	
//I2C总线初始化
	i2c_init();
	
	Delay_Ms(200);
	Adc_init();
	Key_init();
	STM3210B_LCD_Init();
	Usart_init();
	LCD_Clear(White);
	LCD_SetBackColor(White);
	LCD_SetTextColor(Black);
	T1 = x24c02_read(0x00);
	Delay_Ms(2);
	T2 = x24c02_read(0x10);
	Delay_Ms(2);
	T3 = x24c02_read(0x20);
	Delay_Ms(2);
	if((T1>T2)||(T1>T3)||(T2>T3)||(T1<0)||(T2<0)||(T3<0)||(T1>100)||(T2>100)||(T3>100)){
		T1 = 30 ,T2 = 50,T3 = 70;

	}
	

	
//	Delay_Ms(2);
//	sprintf(buff,"  : %d",x24c02_read(0x00));
//	Delay_Ms(2);
//	LCD_DisplayStringLine(Line0, buff);
//	GPIO_SetBits(GPIOC,GPIO_Pin_8);
//		GPIO_SetBits(GPIOD,GPIO_Pin_2);
//		GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	
	while(1){
		
		//Ui_main();
		Key_control();
	if(RX_flag == 1){
		
LCD_DisplayStringLine(Line5 ,RxBuffer2);	
			RX_char = RxBuffer2[0];
			RX_flag = 0;
		for(i = 0 ; i<60 ; i++)	RxBuffer2[i]= 0;
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		if(RX_char == 'C'){
			 
			sprintf(buff,"C:H%.0f+L%d\r\n",valu,status);
			Usart_send(buff);
		}
		if(RX_char == 'S'){

			sprintf(buff,"S:TL%d+TM%d+TH%d\r\n",T1,T2,T3);
			Usart_send(buff);
		}
	}
	
	}
}

//
void Usart_send(char *bufff){
//	for(i = 0 ; i<lenth ; i++){	USART_SendData( USART2, bufff[i]);
//	}
 i=0;
	do{
		USART_SendData( USART2, bufff[i]);
		Delay_Ms(1);
//		while(!USART_GetFlagStatus( USART2,  USART_FLAG_TXE));
		i++;
	}while(bufff[i]!=0);
	
}

void Usart_init(){
	 GPIO_InitTypeDef GPIO_InitStructure;
	 NVIC_InitTypeDef NVIC_InitStructure;
	 USART_InitTypeDef USART_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
  /* Enable the USART2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Configure USART2 Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* Configure USART2 */
  USART_Init(USART2, &USART_InitStructure);

  
  /* Enable USART2 Receive and Transmit interrupts */
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);



  /* Enable the USART2 */
  USART_Cmd(USART2, ENABLE);

	
}


void Key_control(){
	if(B1 == 0){	
		Delay_Ms(100);
		while(B1 == 0);
		UI_flag++;
		
		LCD_Clear(White);
		if(UI_flag%2 == 1)	Ui_set();	
		else Ui_main();
//		if(floagg == 0){
//			floagg = 1;
//			Ui_set();	
//		}
	}
	if(UI_flag%2 == 0)	Ui_main();
	//else 								
	
	
	if(UI_flag%2 == 1){
	if(B2 == 0){
		Delay_Ms(100);
		while(B2 == 0);
		LINE += 24;
		if(LINE > Line5)	LINE = Line3;
//		LCD_Clear(White);
//		LCD_SetBackColor(Green);
//		LCD_DisplayStringLine(LINE,"                    ");
		
		
		LCD_SetBackColor(White);
		
		Ui_set();
	}
	if(B3 == 0){
		Delay_Ms(100);
		while(B3 == 0);
		if(LINE == Line3){
			T1 += 5;
		}
		else if(LINE == Line4){
			T2 += 5;
		}
		else if(LINE == Line5){
			T3 += 5;
		}
		Ui_set();
				x24c02_write(0x00,T1);
				Delay_Ms(2);
			x24c02_write(0x10,T2);
				Delay_Ms(2);
			x24c02_write(0x20,T3);
				Delay_Ms(2);
	}
	if(B4 == 0){
		Delay_Ms(100);
		while(B4 == 0);
		if(LINE == Line3){
			T1 -= 5;
		}
		else if(LINE == Line4){
			T2 -= 5;
		}
		else if(LINE == Line5){
			T3 -= 5;
		}
		Ui_set();
				x24c02_write(0x00,T1);
				Delay_Ms(2);
			x24c02_write(0x10,T2);
				Delay_Ms(2);
			x24c02_write(0x20,T3);
				Delay_Ms(2);
	}
	}

	
	
	

}
void led_ctrol(uint16_t GPIO_Pin,char sss){
	if(sss == 1){
		GPIO_ResetBits(GPIOC,GPIO_Pin);
		GPIO_SetBits(GPIOD,GPIO_Pin_2);
		GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	}
	else if(sss == 0){
		GPIO_SetBits(GPIOC,GPIO_Pin);
		GPIO_SetBits(GPIOD,GPIO_Pin_2);
		GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	}


}
void Key_init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOD, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void Ui_set(){
	if(T2 >= 95)	T2 = 95;
	else if(T2 <= 5)	T2 = 5;
	if(T1 >= 95)	T1 = 95;
	else if(T1 <= 5)	T1 = 5;
	if(T3 >= 95)	T3 = 95;
	else if(T3  <= 5)	T3 = 5;
	
	
	LCD_DisplayStringLine(Line1,"   Prameter Step");
	if(LINE == Line3)	LCD_SetBackColor(Green);
	sprintf(buff,"   Threshold 1: %.2d",T1);
	LCD_DisplayStringLine(Line3, buff);
	LCD_SetBackColor(White);
	
	if(LINE == Line4)	LCD_SetBackColor(Green);
	sprintf(buff,"   Threshold 2: %.2d",T2);
	LCD_DisplayStringLine(Line4, buff);
	LCD_SetBackColor(White);
	
	if(LINE == Line5)	LCD_SetBackColor(Green);
	sprintf(buff,"   Threshold 3: %.2d",T3);
	LCD_DisplayStringLine(Line5, buff);
	LCD_SetBackColor(White);
	
	LCD_DisplayStringLine(Line3, "   Threshold 1: ");
	LCD_DisplayStringLine(Line4, "   Threshold 2: ");
	LCD_DisplayStringLine(Line5, "   Threshold 3: ");
	floagg = 0;
	
	
}

void Ui_main(){
	
	
	
	
		if(time%1000 == 1){
	
			
			
			valu_1 = valu;
			LCD_DisplayStringLine(Line1,"   Liquid Level");
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		valu = ADC_GetConversionValue(ADC1)*100/4096;
			//Delay_Ms(10);
			sprintf(buff,"   Height :%.0fcm",valu);
		LCD_DisplayStringLine(Line3, buff);
			sprintf(buff,"   ADC :%.2fv",valu*3.3/100);
		LCD_DisplayStringLine(Line5 , buff);
			
		if(valu<T1)	status = 0;
		if((valu>T1)&&(valu<T2))	status = 1;
		if((valu>T2)&&(valu<T3))	status = 2;
		if(valu>T3)	status = 3;
			
			sprintf(buff,"   Level :%d",status);
			LCD_DisplayStringLine(Line7 , buff);
			time = 0;
			Delay_Ms(10);
			histroy_flag++;
				if((valu_1 - valu)  > 0.1 ){
					sprintf(buff,"A:H%.0f+L%d+D\r\n",valu,status);
					Usart_send(buff);
				}
				else if((valu_1 - valu)  < -0.1 ){
					sprintf(buff,"A:H%.f+L%.f+U\r\n",valu,status);
					Usart_send(buff);}
}
				if(status == 0){
					led_ctrol(LED_all,0);}
				if(status == 1){
					if(time222%2000<1000){
					led_ctrol(LED_all,0);
					led_ctrol(LED1,1);}
				else{
					led_ctrol(LED_all,0);
					led_ctrol(LED1,0);}}
				if(status == 2){
					if(time222%400<200){
					led_ctrol(LED_all,0);
					led_ctrol(LED2,1);}
				else{
					led_ctrol(LED_all,0);
					led_ctrol(LED2,0);}}
				if(status == 3){
					if(time222%400<200){
					led_ctrol(LED_all,0);
					led_ctrol(LED3,1);}
				else{
					led_ctrol(LED_all,0);
					led_ctrol(LED3,0);}}


}
void Delay_Ms(u32 nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}
void 	Adc_init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOB, ENABLE);
	
	

  /* Configure PC.04 (ADC Channel14) as analog input -------------------------*/
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
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_1Cycles5);

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
