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
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
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
//��¼60�����ݵ�ʱ����ʹ��ǰһ��ʱ���ȥ��ǰʱ����ڲ������ʱ����¼һ�����ݣ��������Ҹļ����ʱ�����ݻ��ڼ�¼������û�в�ֵ���ڲ������������ס��
//���������ʹ��������ֻ�����ݽ������У����ý���OLD_TSS����ʱ��ʱ�䣨s��

//��֪��Ϊʲô������79������Ľṹ�壬ǰ4���޷���ֵ
//����취���ӵ�5����ʼ��ֵ

//eepromд��Ƶ������ʧ��
//�����eepromһ����ַ����һ��byte��8��bit���ܳ���256�����ڵľ͸ߵ�λ�ֿ�д

void Delay_Ms(u32 nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}
