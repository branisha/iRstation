#include <stdint.h>
#include <stdio.h>

void USART_init( void );
void USART_sendByte( uint8_t );
void USART_sendArray(uint8_t*, size_t);
void USART_sendArray16(uint16_t [][2], size_t);
void parseResults(uint16_t a[][2], size_t);
