#ifndef _IO_H_
#define _IO_H_

const uint8_t pinnit_nappaimille[8] = {A4, 3, 5, 4, A2, A3, A0, A1};

#define KEY_UP !digitalRead(pinnit_nappaimille[0])
#define KEY_LEFT !digitalRead(pinnit_nappaimille[1])
#define KEY_DOWN !digitalRead(pinnit_nappaimille[2])
#define KEY_RIGHT !digitalRead(pinnit_nappaimille[3])

#define KEY_X !digitalRead(pinnit_nappaimille[4])
#define KEY_Y !digitalRead(pinnit_nappaimille[5])
#define KEY_B !digitalRead(pinnit_nappaimille[6])
#define KEY_A !digitalRead(pinnit_nappaimille[7])

void IObegin();

#include <Arduino.h>

void IObegin() {
  pinMode(A4, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
}

#endif
