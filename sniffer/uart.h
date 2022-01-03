#ifndef UART_H
#define UART_H

#define F_CPU 48000000UL
#include <stdbool.h>
#include <stdint.h>

void UART_init(uint32_t baud);
bool UART_data_incoming(void);
void UART_write_char(char character);
void UART_write_str(char *str);
char UART_read(void);

#endif /* UART_H */