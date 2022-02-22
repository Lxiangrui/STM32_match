#include "stm32f10x.h"
#include "stdio.h"
#define HH 10  //时
#define MM 59  //分
#define SS 50  //秒

uint32_t TimeDisplay = 0;
/**
  * @说明     配置中断向量控制器
  * @参数     None
  * @返回值   None
  */
void NVIC_Configuration_RTC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @说明     配置RTC
  * @参数     None
  * @返回值   None
  */
void RTC_Configuration(void)
{
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	PWR_BackupAccessCmd(ENABLE);
	BKP_DeInit();
	RCC_LSICmd(ENABLE);
	/* Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	RCC_RTCCLKCmd(ENABLE);

	RTC_WaitForSynchro();
	RTC_WaitForLastTask();
	/* Enable the RTC Second */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	RTC_WaitForLastTask();
	/* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(39999); /* RTC period = RTCCLK/RTC_PR = (40 KHz)/(39999+1) */
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	RTC_SetCounter(HH*3600+MM*60+SS);
	RTC_WaitForLastTask();
	
	NVIC_Configuration_RTC();
	
}

