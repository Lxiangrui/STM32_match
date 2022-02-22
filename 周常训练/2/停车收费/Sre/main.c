#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "key.h"
#include "lcd.h"
#include "pwm.h"
#include "USART.h"
extern uint8_t USART_RXBUF[22];


int time_s;			//秒钟，时刻在变化
int flog_out;
float charge=1;//价格
char charge1[5];
char cccc[11];//代表11个车位
extern uint8_t RXOVER;
int flog1,flog2;//切换界面,flog1，flog2分别显示两个界面
float FEE;




int i,j;//for循环数
typedef struct {
	char	carx;						//用来判断是出入库，还是改价格。
	int ID;				//用来判断那辆车
	int MMM;							//记录时间
	
	
} car;
car CHAR;							//串口返回判断车位N
car charpoit[11];				//10个车位



u32 TimingDelay = 0;
char buff[50];
void Delay_Ms(u32 nTime);
void B2_K();
void B1_K();
void	UI_1();
void	UI_2();


void date();		//处理数据
void qinglin();
void panduanjinchu();//判断进出

void charu(int AA); //插入几个车位
int panduan(int AA);//判断是否是同一台车
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
		

////		date();//判断是改价格，还是出入库，如果是改价格，就改变价格，如果是出入库，就改变，正在出入库的车的参量
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
	
	
//判断是否有车要入库		
						for(i=1;i<11;i++){
						if((cccc[i]==2)){		//代表i好车位有车驶入
							cccc[i]=1;//入库成功，标志位=1；	
							USART_SendString("\nin okk!!\n");
							charu(i);
							 UI_1();
							
													}	 }
//判断是否有车要出库
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
	
	


			charpoit[AA].ID=CHAR.ID;		//因为有10的存在，所以是两位数
			
	

			charpoit[AA].carx=CHAR.carx;
			charpoit[AA].MMM=time_s;					//获取入库时间
	
}



void date(){//分两种判断，定义一个出入库结构体将接受到的ID赋值给结构体变量，和一个收费数，按下开关后

		sprintf(buff,"%d",CHAR.ID);
		
		
	
		CHAR.carx=USART_RXBUF[1];
	
	sprintf(buff, "USART_RXBUF[1]: %c" ,CHAR.carx);
		USART_SendString("\n");
					USART_SendString(buff);
					USART_SendString("\n");
	
		
		if(CHAR.carx!='F'){							//出入库
			
					
					
					CHAR.ID=USART_RXBUF[3]-48;	  			//得到输入的ID,因为是ID12的格式，数字信息在12位
					
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
					CHAR.MMM=time_s;						//记录出入库时间
					USART_SendString("\n");
	
//		sprintf(buff, " %d" , charpoit[10].ID);
//		LCD_DisplayStringLine(Line10,buff);
		
					
					//判断入库和出库
		}
		
		else{												//如果不是输入的ID，那么就是设置的费率
			
					USART_SendString("\n");
					sprintf(buff,"charge:%f",charge);
					USART_SendString(buff);
					USART_SendString("\n");
			
			for(i=0;i<2;i++){
			charge1[i]=USART_RXBUF[i+3];
			}	
			
			charge=atof(charge1);			//得到十进制价格,直接改变
		}
	
}






void B1_K(){//入库程序，占领停车位，分配入库时间
	
		

	
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
								
								if(cccc[i]==0){//代表空车位，按顺序分配位置，有低位的停车位就用低位的。
															
								USART_SendString("\n");
	
								sprintf(buff, " %d" ,i);
								USART_SendString(buff);	
								USART_SendString("\n");	
								cccc[i]=2;//1代表有车占了车位,2代表正在入库,3代表正在出库,入库完成后变成1，出库后变成0；
	
								i=99;			//用于跳出循环
									
									
//								LCD_Clear(Black);
								}
							
}}}}
		

		
void B2_K(){//出库程序，任务有：清空对应车位结构体，清空车位，计算时间，价格
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
