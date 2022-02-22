#include "key.h"
#include "stm32f10x.h"
#include "oled.h"
#include "delay.h"
#include "sys.h"
#include "adc.h"
#include "stdio.h"
#include "time.h"
#include "pwm.h"
#include "uart.h"
#include "i2c.h"
#include "led.h"
extern uint8_t RXOVER;
uint8_t USART_RXBUF;

extern int Frequency ;
//eepr
extern int set_pwm_fre;
int  temp_limit ;
int  humi_limit ;
int  T_interval ;			//��ֵ���ź�Ƶ��

int wendu = 0;								//��ǰ�¶Ⱥ�ʪ��
int	shidu = 0;
char buff[20];						//��ʾ�õ��ַ���

struct piot
{
	int temp;
	int humi;
	int THH,TMM,TSS;
};
int mm = 4;//Ϊ��ǰ��¼����Ľű꣬ÿ�����ڵ�ʱ�����ͼ�һ
struct piot tempp[100];			//���60������

u32 THH = 0, TMM = 0, TSS = 0;
u32 OLD_TSS,oooo;
void eeprom_read();
void eeprom_write();
void baojin();//������ʾ


//�ɼ��������	
void get_data(){
	if(RTC_GetCounter() - OLD_TSS == T_interval){
		 OLD_TSS = RTC_GetCounter();
		mm++;
		
		tempp[mm].humi = shidu;
		tempp[mm].temp = wendu;
		tempp[mm].THH = THH;
		tempp[mm].TMM = TMM;
		tempp[mm].TSS = TSS;
		
	}
}

//ʵʱ���ݽ���
void Data_ui(){
//		
//	sprintf(buff,"%d  %d",RTC_GetCounter(),OLD_TSS);//��ʾʱ��
//	OLED_ShowString(0,0,buff,12);
		baojin();
	
		get_data();	
		if(mm>64)	 {mm = 4;}	//	���������60���ʹ�ǰ�濪ʼ����
	
		OLED_ShowChinese(36-4,0,0,16);//ʵʱ����
		OLED_ShowChinese(54-4,0,1,16);
		OLED_ShowChinese(72-4,0,2,16);
		OLED_ShowChinese(90-4,0,3,16);
	
	sprintf(buff,"%d'C  ",(int)Read_Temperature());//��ʾ�¶�
	wendu = (int)Read_Temperature();
	OLED_ShowString(0,15,"temp:",12);
	OLED_ShowString(40,15,buff,12);
	
	
	shidu = Frequency/100;
	sprintf(buff,"%d%%  ",shidu+1);//��ʾʪ��
	OLED_ShowString(0,30,"humi:",12);
	OLED_ShowString(40,30,buff,12);
	
	sprintf(buff,"num:%.2d",mm-4);
	OLED_ShowString(75,30,buff,12);
	
	
	OLED_ShowString(0,45,"time:",12);
	

	/* Compute  hours */
	THH = RTC_GetCounter() / 3600;
	/* Compute minutes */
	TMM = (RTC_GetCounter() % 3600) / 60;
	/* Compute seconds */
	TSS = (RTC_GetCounter() % 3600) % 60;
	if(oooo == 0){
	OLD_TSS = RTC_GetCounter();
			 oooo = 1;			//ִֻ��һ��
			TIM_Config(set_pwm_fre);   //HZ
	}
	sprintf(buff,"%0.2d:%0.2d:%0.2d",THH, TMM, TSS);
	OLED_ShowString(40,45,buff,12);
		OLED_Refresh();

}


int ctr_data_flag = 47+5;


//���ò����ĳ���
void ctr_data(){
	OLD_TSS = RTC_GetCounter();
	if(RB2 == 0){
		while(RB2 == 0);
		ctr_data_flag += 11; 
		
		if(ctr_data_flag > 47+5)	ctr_data_flag = 14+5;
		
		OLED_ShowString(90,14+5,"       ",12);
		OLED_ShowString(90,25+5,"       ",12);
		OLED_ShowString(90,36+5,"       ",12);
		OLED_ShowString(90,47+5,"       ",12);
		sprintf(buff,"<=",temp_limit);
		OLED_ShowString(90,ctr_data_flag,buff,12);
	}
	if(ctr_data_flag == 14+5){
		if(RB3 == 0){
			while(RB3 == 0);
			temp_limit++;
			eeprom_write();
		}
		if(RB4 == 0){
			while(RB4 == 0);
			temp_limit--;
			eeprom_write();
		}
	}
	else if(ctr_data_flag == 25+5){
		if(RB3 == 0){
			while(RB3 == 0);
			humi_limit += 5;
			eeprom_write();
		}
		if(RB4 == 0){
			while(RB4 == 0);
			humi_limit -= 5;
			eeprom_write();
		}
	}
	else if(ctr_data_flag == 36+5){
		if(RB3 == 0){
			while(RB3 == 0);
			T_interval += 1;
      eeprom_write();
		}
		if(RB4 == 0){
			while(RB4 == 0);
			T_interval -= 1;
			eeprom_write();
		}
	}
	else if(ctr_data_flag == 47+5){
		if(RB3 == 0){
			while(RB3 == 0);
//			set_pwm_fre = x24c02_read(0x0f)*50;
			set_pwm_fre += 500;
//			sprintf(buff,"���µ�ʱ��:%d\n",set_pwm_fre);
//			USART_SendString(buff);
//			
			eeprom_write();
			TIM_Config(set_pwm_fre);   //HZ
		}
		if(RB4 == 0){
			while(RB4 == 0);
			set_pwm_fre -= 500;
			eeprom_write();
			TIM_Config(set_pwm_fre);   //HZ
		}
		
		
	}
	


}
//�������ý���
void Set_ui(){
		float aaa;//�м����
		OLED_ShowChinese(36-4,0,6,16);//ʵʱ����
		OLED_ShowChinese(54-4,0,7,16);
		OLED_ShowChinese(72-4,0,8,16);
		OLED_ShowChinese(90-4,0,9,16);

	OLED_ShowString(0,14+5,"temp:",12);
	sprintf(buff,"%d'C",temp_limit);
	OLED_ShowString(40,14+5,buff,12);
	
	OLED_ShowString(0,25+5,"humi:",12);
	sprintf(buff,"%d%%",humi_limit);
	OLED_ShowString(40,25+5,buff,12);
	
	OLED_ShowString(0,36+5,"time:",12);
	sprintf(buff,"%dS",T_interval);
	OLED_ShowString(40,36+5,buff,12);
	
	OLED_ShowString(0,47+5,"sigl:",12);
	
	aaa = (float)set_pwm_fre/1000;
	sprintf(buff,"%.1fKHz",aaa);
	OLED_ShowString(40,47+5,buff,12);

	ctr_data();
	
		OLED_Refresh();
}




//�����л���ʾ����
int qiehuan_flag;
void ctr_ui(){
	if(	RB1 == 0 ){
	//USART_SendString("11a");
	while(	RB1 == 0 );
	qiehuan_flag++;
	OLED_Clear();
	}
	if(qiehuan_flag%2 == 0){
			Data_ui();
	}
	else
			Set_ui();
			
}





//���ڴ���
void usart_handle(){
	int i;
if(RXOVER == 1){

			RXOVER = 0;
			sprintf(buff,"%c",USART_RXBUF);
			
			USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
		}
			if(USART_RXBUF == 'T'){
				sprintf(buff,"\n�¶����ޣ�%d ��--ʪ������%d%%--ʵ��ʱ��:%0.2d:%0.2d:%0.2d",temp_limit,humi_limit,THH, TMM, TSS);
			  USART_SendString(buff);
			}
			else if(USART_RXBUF == 'D'){
				
				for(i = 4 ; i<mm ; i++){
					sprintf(buff,"\n��%.2d�����ݣ��¶ȣ�%d--ʪ�ȣ�%d--��¼ʱ��ʱ�䣺%0.2d:%0.2d:%0.2d",i-3,tempp[i].temp,tempp[i].humi,tempp[i].THH,tempp[i].TMM,tempp[i].TSS);
			  USART_SendString(buff);
				}
				
			}
			USART_RXBUF = 0;  //��ս�����
}


//�������ݵ�eeprom,�����ĸ�����

void eeprom_write(){
	x24c02_write(0x00,temp_limit);
	delay_ms(2);
	x24c02_write(0x01,set_pwm_fre/50);
	delay_ms(2);
//	sprintf(buff,"set_pwm_freд֮ǰ��%dд��ʱ��д���������%d\n",set_pwm_fre,50*x24c02_read(0x01));
//	USART_SendString(buff);
	
	
	x24c02_write(0x1f,humi_limit);
	delay_ms(2);
	x24c02_write(0x2f,T_interval);
	delay_ms(2);
}
//��eeprom��ȡ����
void eeprom_read(){
	set_pwm_fre = 50*x24c02_read(0x01);
	delay_ms(2);
	temp_limit = x24c02_read(0x00);
	delay_ms(2);
	humi_limit = x24c02_read(0x1f);
	delay_ms(2);
	T_interval = x24c02_read(0x2f);
	delay_ms(2);
}
//����
void limit(){
	if(humi_limit > 100){
			humi_limit = 100;
			eeprom_write();
			}
	else if(humi_limit < 0){
			humi_limit = 0;
			eeprom_write();
			}
	if(temp_limit > 60){
			temp_limit = 60;
			eeprom_write();
			}
	else if(temp_limit < -20){
			temp_limit = -20;
			eeprom_write();
			}
	if(T_interval > 5){
			T_interval = 5;
			eeprom_write();
			}
	else if(T_interval < 1){
			T_interval = 1;
			eeprom_write();
			}
	
	if(set_pwm_fre > 9000){
//			USART_SendString("��������\n");
			set_pwm_fre = 9000;
			eeprom_write();
			}
	else 	if(set_pwm_fre < 1000){
//			sprintf(buff,"����ʱ��%d\n",set_pwm_fre);
//			USART_SendString(buff);
		
//			USART_SendString("��������\n");
			set_pwm_fre = 1000;
			eeprom_write();
			}
	
	
}

//�¶ȼ��
void baojin(){
	if(wendu>temp_limit){//�¶ȱ���
		if(TSS%2 == 1)	LED_Control(LED1,1);
		else						LED_Control(LED1,0);
	}
	else							LED_Control(LED1,0);
	
	
	if(shidu>humi_limit){//ʪ�ȱ���
		if(TSS%2 == 1)	LED_Control(LED2,1);
		else						LED_Control(LED2,0);
	}
	else							LED_Control(LED2,0);
}



