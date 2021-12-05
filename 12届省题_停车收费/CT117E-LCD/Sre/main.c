#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>

#include "key.h"
#include "lcd.h"
#include "pwm.h"
#include "USART.h"
extern uint8_t USART_RXBUF[23];


int time;
char tim_s1[10];
char tim_s2[10];
char charge[6];//价格
int time1,time2;
int cccc[10];//代表8个车位
extern uint8_t RXOVER;
int flog1;//切换界面
int flog2;
int i=1;
int CNBR,VNBR;
float FCNBR=3.5,FVNBR=2.0;
int iii;



int i,j;//for循环数
typedef struct {
	char	carx;
	uint8_t car1[6];
	char MMM;
	uint8_t cartime[16];
	
} car;
car CHAR;
car char1[15];



u32 TimingDelay = 0;
char buff[50];
void Delay_Ms(u32 nTime);
void D_UI(int CNBR,int VNBR,int IDLE);
void P_UI(float CNBR,float VNBR);
void qiehuan();//切换界面
void gaijia();	//改价格
void MC();
void date();
void qinglin();
void panduanjinchu();//判断进出;
void charu(int AA); //如第几个车位
int panduan(int AA);//判断是否是同一台车
void led();
void PWM();
//Main Body

int main(void)
{
	
	
	SysTick_Config(SystemCoreClock/1000);
	
	key_init();
	PWM_IO_Config();
	
	USART_Config();
	
	STM3210B_LCD_Init();
	LCD_Clear(White);
	LCD_SetBackColor(White);
	LCD_SetTextColor(Black);
	
	

	while(1){

		qiehuan();
		gaijia();

		date();
		panduanjinchu();
		
		led();
		PWM();	
	
		
				
				
if(flog1%2==0){
			D_UI(CNBR,VNBR,8-VNBR-CNBR);
			}
			else	P_UI(FCNBR,FVNBR);
			
	}

	}



	
	
void PWM(){
	
	
	if(RB4==0){
			Delay_Ms(100);
			if(RB4==0){
				flog2=flog2+1;
				
			}
}
	if(flog2%2==0){
			LED_Control(LED0,0);
			LED_Control(LED1,0);
      LED_Control(LED2,0);
			LED_Control(LED3,0);
			LED_Control(LED4,0);
			LED_Control(LED5,0);
			LED_Control(LED6,0);
			LED_Control(LED7,0);					
			TIM_Cmd(TIM3, DISABLE);
								}
	else{
			LED_Control(LED0,0);
			LED_Control(LED1,1);
      LED_Control(LED2,0);
			LED_Control(LED3,0);
			LED_Control(LED4,0);
			LED_Control(LED5,0);
			LED_Control(LED6,0);
			LED_Control(LED7,0);
		
			TIM_Config(499/100,499/100);
	
	
			}

}
	
	
void led(){
	
	if((CNBR+VNBR)==8){
			LED_Control(LED0,1);
			LED_Control(LED1,0);
      LED_Control(LED2,0);
			LED_Control(LED3,0);
			LED_Control(LED4,0);
			LED_Control(LED5,0);
			LED_Control(LED6,0);
			LED_Control(LED7,0);}
		else{
			LED_Control(LED0,0);
			LED_Control(LED1,0);
      LED_Control(LED2,0);
			LED_Control(LED3,0);
			LED_Control(LED4,0);
			LED_Control(LED5,0);
			LED_Control(LED6,0);
			LED_Control(LED7,0);}


}


void panduanjinchu(){
	
	
	

if(RXOVER){
	if((USART_RXBUF[1]=='N')&&(USART_RXBUF[2]=='B')&&(USART_RXBUF[3]=='R')&&(USART_RXBUF[4]==':')&&(USART_RXBUF[9]==':')){
	
		
		USART_SendString("\n");
		USART_SendString(USART_RXBUF);
		USART_SendString("\n");
		RXOVER = 0;
		iii++;//接受串口的次数，这里的1s是防抖,完成一次后执行的程序
		
		//判断出库,顺便计算价钱
		if((panduan(1)==1)||(panduan(2)==1)||(panduan(3)==1)||(panduan(4)==1)||(panduan(5)==1)||(panduan(6)==1)||(panduan(7)==1)||(panduan(8)==1)){
				
				for(i=1;i<9;i++){
				
						if((panduan(i)==1)){
						//具体哪一位车出去
						
							cccc[i]=0;
				
				
							
							
							
							for(j=0;j<2;j++){
							
							tim_s1[j]=CHAR.cartime[j+8];
								
								
							tim_s2[j]=char1[i].cartime[j+8];
								
								
							}							
							time1=atoi(tim_s1);
					
							time2=atoi(tim_s2);
							
							for(j=0;j<2;j++){
							
							tim_s1[j]=CHAR.cartime[j+6];
								
								
							tim_s2[j]=char1[i].cartime[j+6];
								
								
							}
							time1=time1+atoi(tim_s1)*60;
					
							time2=time2+atoi(tim_s2)*60;//算上分钟的秒
							
							for(j=0;j<2;j++){
							
							tim_s1[j]=CHAR.cartime[j+4];
								
								
							tim_s2[j]=char1[i].cartime[j+4];
								
								
							}
							time1=time1+atoi(tim_s1)*3600;
					
							time2=time2+atoi(tim_s2)*3600;//算上时钟的秒
							
							
							for(j=0;j<2;j++){
							
							tim_s1[j]=CHAR.cartime[j+2];
								
								
							tim_s2[j]=char1[i].cartime[j+2];
								
								
							}
							time1=time1+atoi(tim_s1)*3600*24;
					
							time2=time2+atoi(tim_s2)*3600*24;//算上一天的秒
							
							
							
								time=-time2+time1;
								time=time/60+1;						//这里的time代表小时，因为是按小时收费，所以未满1小时还是加了1
							
							
							
							//返回数据
							
								sprintf(charge, " %d" , time);
								
									if(char1[i].carx=='C'){	
													USART_SendString("C");
//												
												}
												else
												{
													USART_SendString("V");
														
												}
						
								USART_SendString("NBR");
												
								USART_SendString(":");
								USART_SendString(CHAR.car1);
								USART_SendString(":");			
								USART_SendString(charge);
								
								
												if(char1[i].carx=='C'){	
												
													sprintf(charge, " %.2f" , (time)*FCNBR);
													CNBR--;
												}
												else if(char1[i].carx=='V')
												{
													VNBR--;
													sprintf(charge, " %.2f" , (time)*FVNBR);				
												}
							
								
												
							
													
												USART_SendString(":");
												
												USART_SendString(charge);

												
						qinglin(i);	
				}
				
				}
			
		}
		else{
			
		//要先判断不是出库	
	for(i = 1;i<10;i++){
		
		if(cccc[i]==0){//代表空车位
											if((CHAR.carx=='C')){	
												
													CNBR++;
												USART_SendString("\nCNBRS\n");
												}
												else if((CHAR.carx=='V'))
												{
													
													VNBR++;
													USART_SendString("\nVNBRS\n");
												}						
												else{
													USART_SendString("EER");
					
												}
						
			
		cccc[i]=1;//1代表有车占了车位
		charu(i);
		
												
			
		i=9;	
		}
										
		
		
		
	}
										
	
	
		}
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
		
	}
	else{
		RXOVER = 0;
			USART_SendString("\n");
			USART_SendString("err");
			USART_SendString("\n");
		USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	}

}
}







int panduan(int AA){
	

		
		if(CHAR.car1[0]==char1[AA].car1[0]){
			if(CHAR.car1[1]==char1[AA].car1[1]){
				if(CHAR.car1[2]==char1[AA].car1[2]){
					if(CHAR.car1[3]==char1[AA].car1[3]){
			
					return 1;
					}}}}
					return 0;
		
		

}



void qinglin(int AA){
	
for(i=0;i<4;i++){
			char1[AA].car1[i]=0;
			}

	char1[AA].carx=0;
			
			
	for(i=0;i<12;i++){
			char1[AA].cartime[i]=0;
			}
		
			


}


void charu(int AA){
	
for(i=0;i<4;i++){
			char1[AA].car1[i]=CHAR.car1[i];
			}

	char1[AA].carx=CHAR.carx;
			
			
	for(i=0;i<12;i++){
			char1[AA].cartime[i]=CHAR.cartime[i];
			}
		
			
	
}

void date(){


	CHAR.carx=USART_RXBUF[0];
		for(i=5;i<9;i++){
		CHAR.car1[i-5]=USART_RXBUF[i];
		}
		
//			LCD_DisplayStringLine(Line2,CHAR.car1);
		
		for(i=10;i<22;i++){
		CHAR.cartime[i-10]=USART_RXBUF[i];
		}
		
//		LCD_DisplayStringLine(Line5,CHAR.cartime);

				
		
			
		
}


void gaijia(){
	
	
if(flog1%2==1){//副界面
		if(RB2==0){
			Delay_Ms(100);
			if(RB2==0){
			FCNBR=FCNBR+0.5;
			FVNBR=FVNBR+0.5;
			}
			}
			P_UI(FCNBR,FVNBR);
			
		 if(RB3==0){
			Delay_Ms(100);
			if(RB3==0){
			FCNBR=FCNBR-0.5;
			FVNBR=FVNBR-0.5;
			}
			}
				P_UI(FCNBR,FVNBR);
		}
	
	
}

void qiehuan(){
	
	
if(RB1==0){
			Delay_Ms(100);
			if(RB1==0){
				flog1=flog1+1;
				
			LCD_Clear(White);
			
			
			}
}
		}



void D_UI(int CNBR,int VNBR,int IDLE){
	
	
	
	
	sprintf(buff,"       CNBR:%d",CNBR);	
		LCD_DisplayStringLine(Line3,buff);
	sprintf(buff,"       VNBR:%d",VNBR);	
		LCD_DisplayStringLine(Line5,buff);
	sprintf(buff,"       IDLE:%d",IDLE);	
		LCD_DisplayStringLine(Line7,buff);	
}
void P_UI(float CNBR,float VNBR){
	sprintf((char*)buff,"      CNBR:%.2f",CNBR);	
	LCD_DisplayStringLine(Line4,(char*)buff);
	sprintf((char*)buff,"      VNBR:%.2f",VNBR);	
	LCD_DisplayStringLine(Line6,(char*)buff);
}	



//
void Delay_Ms(u32 nTime)
{
	
	
//VNBR:D518:210002140021
//VNBR:D517:210002140021
//VNBR:D516:210002140021
//VNBR:D515:210002140021
//VNBR:D514:210002140021
//VNBR:D513:210002140021
//VNBR:D512:210002140021
//VNBR:D511:210002140021
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}
