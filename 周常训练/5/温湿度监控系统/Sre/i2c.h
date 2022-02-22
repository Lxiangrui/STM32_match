#ifndef  __I2C_H__
#define  __I2C_H__

void i2c_init(void);
void delay1(unsigned int n);

void I2CStart(void);
void I2CStop(void);
void I2CSendAck(void);
void I2CSendNotAck(void);
unsigned char I2CWaitAck(void);

void I2CSendByte(unsigned char cSendByte);
unsigned char I2CReceiveByte(void);
void i2c_init();
uint8_t x24c02_read(uint8_t address);
void x24c02_write(unsigned char address,unsigned char info);

#endif
