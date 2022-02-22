#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "key.h"
#include "lcd.h"
#include "pwm.h"
#include "USART.h"
extern uint8_t USART_RXBUF[22];


int time_s;			//���ӣ�ʱ���ڱ仯
int flog_out;
float charge=1;//�۸�
char charge1[5];
char cccc[11];//����11����λ
extern uint8_t RXOVER;
int flog1,flog2;//�л�����,flog1��flog2�ֱ���ʾ��������
float FEE;




int i,j;//forѭ����
typedef struct {
	char	carx;						//�����ж��ǳ���⣬���Ǹļ۸�
	int ID;				//�����ж�������
	int MMM;							//��¼ʱ��
	
	
} car;
car CHAR;							//���ڷ����жϳ�λN
car charpoit[11];				//10����λ



u32 TimingDelay = 0;
char buff[50];
void Delay_Ms(u32 nTime);
void B2_K();
void B1_K();
void	UI_1();
void	UI_2();


void date();		//��������
void qinglin();
void panduanjinchu();//�жϽ���

void charu(int AA); //���뼸����λ
int panduan(int AA);//�ж��Ƿ���ͬһ̨��
void led();

//Main Body

int main(void)
{
	
	
	SysTick_Config(SystemCoreClock/1000);
	
	key_init();
	PWM_IO_Config();
	USART_Config();
	
	STM3210B_LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
	
	LCD_DisplayStringLine(Line3,"Please select : ");
	LCD_DisplayStringLine(Line4," go in ");
	LCD_DisplayStringLine(Line5," go out ");
	LCD_DisplayStringLine(Line6," change the price");

	while(1){

		B1_K();
		B2_K();
		

////		date();//�ж��Ǹļ۸񣬻��ǳ���⣬����Ǹļ۸񣬾͸ı�۸�����ǳ���⣬�͸ı䣬���ڳ����ĳ��Ĳ���
		panduanjinchu();
		
		led();
				
		CHAR.ID=0;
		
		
		sprintf(buff, "             Rates:%.2f" , charge);
		LCD_DisplayStringLine(Line9,buff);
		
		
			
	}

	}

void led(){
		j=0;
		for(i=1;i<11;i++){
		j=j+cccc[i];
		}
		if(j<=10){
			LED_Control(LED1,0);
			LED_Control(LED0,1);

		}	
		else{ 
			LED_Control(LED0,0);
			LED_Control(LED1,1);

		}
		}


void panduanjinchu(){
	
if(RXOVER){
		date();
		
		
		
	
	
		USART_SendString("\n");
		USART_SendString("Recive:");
		USART_SendString(USART_RXBUF);
		USART_SendString("\n");
	
	
//�ж��Ƿ��г�Ҫ���		
						for(i=1;i<11;i++){
						if((cccc[i]==2)){		//����i�ó�λ�г�ʻ��
							cccc[i]=1;//���ɹ�����־λ=1��	
							USART_SendString("\nin okk!!\n");
							charu(i);
							 UI_1();
							
													}	 }
//�ж��Ƿ��г�Ҫ����
					if(flog_out){
						for(i = 1;i<11;i++){
						if(charpoit[i].ID==CHAR.ID){
							
							
							FEE=(time_s-charpoit[i].MMM)*charge;		
							
							UI_2();
							
							
							USART_SendString("\n out okk!!\n");
							cccc[i]=0;
							qinglin(i);
							flog_out=0;
							i=99;
							
							
							
							
							}		
							}
					if(i<30){
							USART_SendString("\n out false!!\n");	
							}
							}
									

													
													
		USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
		RXOVER = 0;
		flog_out=0;
	}

//		sprintf(buff, " %d" , charpoit[1].ID);
//		LCD_DisplayStringLine(Line1,buff);
//		sprintf(buff, " %d" , charpoit[2].ID);
//		LCD_DisplayStringLine(Line2,buff);
//		sprintf(buff, " %d" , charpoit[3].ID);
//		LCD_DisplayStringLine(Line3,buff);
//		sprintf(buff, " %d" , charpoit[4].ID);
//		LCD_DisplayStringLine(Line4,buff);
//		sprintf(buff, " %d" , charpoit[5].ID);
//		LCD_DisplayStringLine(Line5,buff);
//		sprintf(buff, " %d" , charpoit[6].ID);
//		LCD_DisplayStringLine(Line6,buff);
//		sprintf(buff, " %d" , charpoit[7].ID);
//		LCD_DisplayStringLine(Line7,buff);
//		sprintf(buff, " %d" , charpoit[8].ID);
//		LCD_DisplayStringLine(Line8,buff);
//		sprintf(buff, " %d" , charpoit[9].ID);
//		LCD_DisplayStringLine(Line9,buff);
}
void	UI_1(){
							int H,M,S;
	
	
							S=CHAR.MMM%60;
							M=(CHAR.MMM/60)%60;
							H=CHAR.MMM/3600;
							
							sprintf((char*)buff,"           %.2d:%.2d:%.2d",H,M,S);
							LCD_DisplayStringLine(Line5,buff);
	
							sprintf(buff, "     %d" , CHAR.ID);
							LCD_DisplayStringLine(Line5,buff);
							
		
					
}
void	UI_2(){
	
		sprintf(buff, "     %d       %.2f" , CHAR.ID,FEE);
		LCD_DisplayStringLine(Line5,buff);									
						}


int panduan(int AA){

		
		
					if(CHAR.ID==charpoit[AA].ID){
					
			
					return 1;
					}
					return 0;
		
		

}



void qinglin(int AA){
	

	charpoit[AA].ID=0;
			
	charpoit[AA].carx=0;
			
	charpoit[AA].MMM=0;


}


void charu(int AA){
	
	


			charpoit[AA].ID=CHAR.ID;		//��Ϊ��10�Ĵ��ڣ���������λ��
			
	

			charpoit[AA].carx=CHAR.carx;
			charpoit[AA].MMM=time_s;					//��ȡ���ʱ��
	
}



void date(){//�������жϣ�����һ�������ṹ�彫���ܵ���ID��ֵ���ṹ���������һ���շ��������¿��غ�

		sprintf(buff,"%d",CHAR.ID);
		
		
	
		CHAR.carx=USART_RXBUF[1];
	
	sprintf(buff, "USART_RXBUF[1]: %c" ,CHAR.carx);
		USART_SendString("\n");
					USART_SendString(buff);
					USART_SendString("\n");
	
		
		if(CHAR.carx!='F'){							//�����
			
					
					
					CHAR.ID=USART_RXBUF[3]-48;	  			//�õ������ID,��Ϊ��ID12�ĸ�ʽ��������Ϣ��12λ
					
			if((USART_RXBUF[3]=='1')&&(USART_RXBUF[4]=='0')){CHAR.ID=10;}
//			if(USART_RXBUF[4]<'0'){	CHAR.ID=10;		}
					sprintf(buff,"calID:%d",CHAR.ID);
					USART_SendString("\n");
					USART_SendString(USART_RXBUF);
					USART_SendString("\n");
			
//					sprintf(buff, " %d" , charpoit[1].MMM);
//					LCD_DisplayStringLine(Line1,buff);
					Delay_Ms(100);
					USART_SendString("\n");
					USART_SendString(buff);
					CHAR.MMM=time_s;						//��¼�����ʱ��
					USART_SendString("\n");
	
//		sprintf(buff, " %d" , charpoit[10].ID);
//		LCD_DisplayStringLine(Line10,buff);
		
					
					//�ж����ͳ���
		}
		
		else{												//������������ID����ô�������õķ���
			
					USART_SendString("\n");
					sprintf(buff,"charge:%f",charge);
					USART_SendString(buff);
					USART_SendString("\n");
			
			for(i=0;i<2;i++){
			charge1[i]=USART_RXBUF[i+3];
			}	
			
			charge=atof(charge1);			//�õ�ʮ���Ƽ۸�,ֱ�Ӹı�
		}
	
}






void B1_K(){//������ռ��ͣ��λ���������ʱ��
	
		

	
if(RB1==0){
			Delay_Ms(100);
			if(RB1==0){
				
				j=0;
			for(i=1;i<11;i++){
			j=j+cccc[i];
			}
			if(j<10){
//				GPIO_RsetBits(GPIOB,BEEP);
				LED_Control(LED5,0);
			}
			else
				LED_Control(LED5,1);
//				GPIO_SetBits(GPIOB,BEEP);
				
				
				
				
				
				LCD_Clear(Black);
				LCD_DisplayStringLine(Line3," ID Code     Time");
				flog1=1;
				
				
				USART_SendString("try to going\n");
						for(i = 1;i<11;i++){
								
								if(cccc[i]==0){//����ճ�λ����˳�����λ�ã��е�λ��ͣ��λ���õ�λ�ġ�
															
								USART_SendString("\n");
	
								sprintf(buff, " %d" ,i);
								USART_SendString(buff);	
								USART_SendString("\n");	
								cccc[i]=2;//1�����г�ռ�˳�λ,2�����������,3�������ڳ���,�����ɺ���1���������0��
	
								i=99;			//��������ѭ��
									
									
//								LCD_Clear(Black);
								}
							
}}}}
		

		
void B2_K(){//������������У���ն�Ӧ��λ�ṹ�壬��ճ�λ������ʱ�䣬�۸�
if(RB2==0){
			Delay_Ms(100);
			if(RB2==0){
				USART_SendString("try to out\n");
				flog1=0;
				flog_out=1;		
				
				
				LCD_Clear(Black);
				LCD_DisplayStringLine(Line3," ID Code     Fee");
			
			
			
			}
}
	
}


//
void Delay_Ms(u32 nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}
