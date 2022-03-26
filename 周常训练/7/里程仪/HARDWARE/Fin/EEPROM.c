#include "stm32f10x.h"

/**
  * @˵��     ��AT24C02ָ����ַ����һ���ֽ�����
  * @����     address:AT24C02�ڲ��洢��ַ
  * @����ֵ   val:��������
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
  * @˵��     ��AT24C02ָ����ַд��һ���ֽ�����
  * @����     address:AT24C02�ڲ��洢��ַ
  * @����     info:д������
  * @����ֵ   None
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
