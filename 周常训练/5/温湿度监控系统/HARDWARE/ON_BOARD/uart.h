#ifndef __UART_H
#define __UART_H 

void NVIC_Configuration_usart(void);
void USART_Config(void);
void USART_SendString(char *str);

#endif