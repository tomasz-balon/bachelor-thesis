#ifndef UART_H
#define UART_H

#include <stdbool.h>
#include <stdint.h>

#define F_CPU 48000000UL

void UART1_init(uint32_t baud);
void UART3_init(uint32_t baud);
bool UART1_data_incoming(void);
void UART1_write_char(char character);
void UART1_write_str(char *str);
char UART1_read(void);

#endif /* UART_H */