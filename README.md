相关配置文件

| 模块/对应模式 | 位置          |                                                              |      |
| ------------- | ------------- | ------------------------------------------------------------ | ---- |
| RTC时间       | RTC\LSI_Calib | 注意添加中断触发函数                                         |      |
| ADC           | ADC\ADC1_DMA  | 删除DMA部分，失能ADC_ScanConvMode                                                                                                                                  改ADC通道、GPIO，ADC_SoftwareStartConvCmd(ADC1,ENABLE)后面设一个延迟5ms |      |
| E2PROM        |               | 调用iic.c和iic.h                                             |      |
|               |               |                                                              |      |
|               |               |                                                              |      |
|               |               |                                                              |      |
|               |               |                                                              |      |
|               |               |                                                              |      |



## EEPROM配置

```c
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

```

## 串口

注意删除			USART_ITConfig(USART2, USART_IT_TXE, ENABLE);

```c
void USART2_IRQHandler(void)
{
	uint8_t temp;
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);		
		temp = USART_ReceiveData(USART2);
		if((temp == 'x') || (RXCUNT == 20)){
			RXCUNT = 0;
			RXOVER = 1;  //接收完成标志位置位
			USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
		}
		else{
			USART_RXBUF[RXCUNT] = temp;
			++RXCUNT;			
		}
	}
}
```

## 外部触发中断/按键中断

**在比赛提供的V3.5库的"Project->STM32F10x_StdPeriph_Examples->EXTI->EXTI_Config"文件夹下，打开"main.c":**

复制到对应模块的.c文件

![](https://i.niupic.com/images/2022/03/26/9Xlb.png)

**再打开"EXTI_Config"下的"stm32f10x_it.c"：**复制到自己过程的it.c中

![](https://i.niupic.com/images/2022/03/26/9Xlj.png)

## LED

注意输出的io口和输入io口配置时，多了一个设置输出频率的成员变量
