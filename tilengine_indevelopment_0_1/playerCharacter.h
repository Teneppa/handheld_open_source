#ifndef _PLAYERCHARACTER_H_
#define _PLAYERCHARACTER_H_

#include "Arduino.h"
#include "sprite.h"

/********************************************************************************************/
/* playerCharacter library starts form here!                                                */
/********************************************************************************************/
struct playerCharacter : public Sprite {

    /* Sprite */
    uint16_t getWidth() {
      return width;
    }
    
    uint16_t getHeight() {
      return height;
    }
    
    uint16_t isPixelOn(uint16_t x, uint16_t y) {
      return 0;
    }
    
    playerCharacter(uint8_t bitmapWidth, uint8_t bitmapHeight, uint8_t * bitmap);    // Constructor for the player

    uint8_t * playerBitmap; // Store the pointer of the player's bitmap

    void draw();  // Draw the player

    void move(uint16_t movex, uint16_t movey);
    void moveRelative(uint16_t movex, uint16_t movey);

    void pointToPlayerDrawingFunction(void(*f)(uint8_t x, uint8_t y));
    void (*drawWithPointedFunction)(uint8_t, uint8_t);

    void handleInputs(bool keyLeft, bool keyRight, bool keyJump, uint16_t updateInterval);
    void handlePhysics(uint8_t readCollisionFlags, uint16_t updateInterval);

    uint16_t x = 0;
    uint16_t y = 0;

    uint8_t jump_enable = 0;

    float vSpeed = 0;
    float hSpeed = 0;

    float vAccel = 0.6;
    float hAccel = 0;

    uint8_t width = 0;
    uint8_t height = 0;

    long oldMovementUpdate = 0;

    long oldPhysics = 0;

    private:
      bool flagMoveLeft = false;
      bool flagMoveRight = false;
      bool flagJump = false;
};


#endif
