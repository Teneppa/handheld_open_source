/*
 * IO.cpp
 *
 *  Created: 14.7.2017 14.41.54
 *  Author: Teemu Laurila
 */ 

#include "io.h"
#include <Arduino.h>

void IObegin() {
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);

  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);

  pinMode(A4, INPUT_PULLUP);
}

/*****************************************************************************************************************************/

/*
uint8_t read(uint8_t port, uint8_t pin){
  if(port & (1<<pin)){
    return 1;
  }else{
    return 0;
  }
}
*/

/*****************************************************************************************************************************/

/*
uint8_t returnKey(bool side, uint8_t bdir) {
  //select_left = A4 = PC4
  //select_right = A5 = PC5
  if(side){
    PORTC &= ~(1<<4);
    PORTC |= (1<<5);
    }else{
    PORTC &= ~(1<<5);
    PORTC |= (1<<4);
  }
  _delay_ms(1);
  return read(PINC,bdir);
}
*/

/*****************************************************************************************************************************/

uint8_t checkKeyBit(uint8_t reg, uint8_t b){
  /*
  uint8_t var = mcp2.readGPIO(reg);
  if((var & (1<<b))){
    return 0;
  }
  return 1;
  */
}
