#ifndef F_CPU
#define F_CPU 16000000
#endif
#ifndef BAUD
#define BAUD 9600
#endif

#include <avr/io.h>
#include <util/setbaud.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void USART_init(){
  // From setbaud.h
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;
  // Enable TX & RX
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  // 8-bit; Parity-Even; 2-bits stop
  UCSR0C = (1<<UPM01)|(1<<USBS0)|(3<<UCSZ00);
}

void USART_sendByte(uint8_t byte)
{
/* Wait for empty transmit buffer */
while ( !( UCSR0A & (1<<UDRE0)) )
;
/* Put data into buffer, sends the data */
UDR0 = byte;
}

void USART_sendArray(uint8_t *a, size_t n)
{
    for(uint8_t i = 0; i < n; i++){
      USART_sendByte(a[i]);
    }
}
void USART_sendArray16(uint16_t a[][2], size_t n)
{
  uint8_t send=0;
  for(uint16_t i = 18; i < n; i++){
    if(a[i][1] > 0){
      send = 1;
    }
  }
    if(send){
      for(uint16_t i = 0; i < n; i++){
        if((a[i][0] & 0xFF) == 0 && i == 0) continue;
         USART_sendByte(a[i][0] & 0xFF);
         USART_sendByte(a[i][1]>>8 & 0xFF);
         USART_sendByte(a[i][1] & 0xFF);
    }
  }

}

void parseResults(uint16_t a[][2], size_t n){

    char buff[64];
    uint8_t send=0;
    for(uint16_t i = 18; i < n; i++){
      if(a[i][1] > 0){
        send = 1;
      }
    }
    if(send){
      for(uint16_t i = 0; i < n; i++){
        if(a[i][1] > 20000){
          sprintf(buff, "%d",a[i][0]);
          for(uint8_t j = 0; j < strlen(buff); j++ ){
            USART_sendByte(buff[j]);
          }
          USART_sendByte('\n');
        }
        sprintf(buff, "%d",a[i][0]);
        for(uint8_t j = 0; j < strlen(buff); j++ ){
          USART_sendByte(buff[j]);
        }
        USART_sendByte('\n');
      }
    }
}
