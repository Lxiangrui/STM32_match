#include "stm32f10x.h"
#include "oled.h"
#include "delay.h"
#include "sys.h"
#include "adc.h"
#include "pwm.h"
#include "time.h"
#include "uart.h"
#include "i2c.h"
#include "key.h"
int set_pwm_fre = 0;
u32 TimingDelay = 0;
//extern int IC2Value ;
//extern int DutyCycle ;
//extern int Frequency ;
void Delay_Ms(u32 nTime);
//Main Body
int main(void)
{
	SysTick_Config(SystemCoreClock/1000);
	delay_init();
	NVIC_Configuration();
	OLED_Init();
	OLED_ColorTurn(0);//0正常显示，1 反色显示
  OLED_DisplayTurn(0);
	OLED_Refresh();
	LED_ON;
	key_init();
	ADC_Config();
	PWM_IO_Config();
	TIM_Config(set_pwm_fre);   //HZ
	TIME_INPUT();
	RTC_Configuration();
	USART_Config();
	i2c_init();
	eeprom_read();
	LED_Init_11();
	LED_Control(LEDALL,0);
	while(1){
	ctr_ui();
	usart_handle();	
	limit();
	//get_data();
	}
}
//记录60个数据的时候，我使用前一个时间减去当前时间等于采样间隔时，记录一个数据，但是在我改间隔的时候，数据还在记录，导致没有插值等于采样间隔，程序卡住。
//解决方法，使采样程序只在数据界面运行，设置界面OLD_TSS等于时间时间（s）

//不知道为什么，包含79个数组的结构体，前4个无法赋值
//解决办法，从第5个开始赋值

//eeprom写入频率数据失败
//它这个eeprom一个地址的是一个byte，8个bit不能超过256，大于的就高低位分开写

void Delay_Ms(u32 nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}
