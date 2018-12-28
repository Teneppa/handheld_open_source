/*
 * IO.h
 *
 *  Created: 14.7.2017 14.43.36
 *  Author: Teemu Laurila
 */ 

#ifndef IO_H_
#define IO_H_

#include <Arduino.h>

/*
#define KEYUP checkKeyBit(0,0)
#define KEYLEFT checkKeyBit(0,1)
#define KEYDOWN checkKeyBit(0,2)
#define KEYRIGHT checkKeyBit(0,3)

#define KEYSHOOTLEFT checkKeyBit(1,4)
#define KEYSHOOTRIGHT checkKeyBit(1,5)

#define KEYX checkKeyBit(1,3)
#define KEYY checkKeyBit(1,2)
#define KEYB checkKeyBit(1,1)
#define KEYA checkKeyBit(1,0)

#define KEYQ checkKeyBit(0,4)
#define KEYSTART checkKeyBit(0,5)
#define KEYSELECT checkKeyBit(0,6)
*/

#define KEYUP !digitalRead(A4)
#define KEYLEFT !digitalRead(3)
#define KEYDOWN !digitalRead(5)
#define KEYRIGHT !digitalRead(4)

#define KEYX !digitalRead(A2)
#define KEYY !digitalRead(A3)
#define KEYB !digitalRead(A0)
#define KEYA !digitalRead(A1)

#define KEYQ !digitalRead(2)
#define KEYSTART 1
#define KEYSELECT 1

#define KEYSHOOTLEFT 1
#define KEYSHOOTRIGHT 1

void IObegin();
uint8_t checkKeyBit(uint8_t reg, uint8_t b);

#endif /* IO_H_ */
