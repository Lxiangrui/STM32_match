#ifndef __EEPROM_H
#define __EEPROM_H


uint8_t x24c02_read(uint8_t address);
void x24c02_write(unsigned char address,unsigned char info);

#endif