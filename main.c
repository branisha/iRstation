#ifndef F_CPU
#define F_CPU 16000000
#endif
#ifndef BAUD
#define BAUD 9600
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "USART_lib.h"

void T1_init(){
  TCCR1A = 0;
  TCCR1B = (1 << CS10);
  TCNT1 = 0;
  TIFR1 |= (1<<TOV1);
}

void T1_reset(){
  TCNT1 = 0;
}

uint8_t getIR(){
  return !(PIND & (1 << PD2));
}

int main() {

  USART_init();

 DDRD = 0;
 PORTD = 0;

 uint16_t t_value = 0;

 uint16_t IR_buff[64][2];

 uint8_t status = getIR();
 uint8_t c = 0;
 uint8_t start = 0;

 while(1){

   if(getIR() && !start){
     start = 1;
     c = 0;
     T1_init();
   }
   while(start){
     while(status == getIR()){
       if((TIFR1 & (1 << TOV1))){
         start = 0;
         // USART_sendArray16(IR_buff, 32);
         parseResults(IR_buff, 32);
         c=0;
         for(uint8_t i = 0; i<64;i++){
           IR_buff[i][0] = 0;
           IR_buff[i][1] = 0;
         }
        break;
      }
     }
     IR_buff[c][0] = status;
     IR_buff[c][1] = TCNT1;
     T1_reset();
     c++;
     status = getIR();
   }

/*
   if(start && (TIFR1 & (1 << TOV1))){
     USART_sendArray16((uint16_t *)IR_buff, 32);
     for(uint8_t i = 0; i<64;i++){
       IR_buff[i][0] = 0;
       IR_buff[i][1] = 0;
     }
     start = 0;
     TIFR1 |= (1<<TOV1);
   }
   */
   status = getIR();
 }
  return 0;
}
