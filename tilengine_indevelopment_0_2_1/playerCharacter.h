/*
 *  This mess is created by Teemu Laurila in 31.12.2018
 */

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

    /*  This would be used in pixel perfect collision detection 
        Currently this function does nothing
    */
    uint16_t isPixelOn(uint16_t x, uint16_t y) {
      return 0;
    }
    
    playerCharacter(uint8_t bitmapWidth, uint8_t bitmapHeight);    // Constructor for the player

    void draw();  // Draw the player

    void move(uint16_t movex, uint16_t movey);
    void moveRelative(uint16_t movex, uint16_t movey);

    void pointToPlayerDrawingFunction(void(*f)(uint8_t x, uint8_t y));
    void (*drawWithPointedFunction)(uint8_t, uint8_t);

    //void pointToPlayerCollisionFunction(void(*collision)(uint8_t x, uint8_t y));

    void handleInputs(bool keyLeft, bool keyRight, bool keyJump);
    void handlePhysics(uint8_t readCollisionFlags, bool playerCollides, uint16_t updateInterval);

    uint16_t x = 0;   // Player coordinates
    uint16_t y = 0;

    uint16_t oldX = 0;   // Old player coordinates
    uint16_t oldY = 0;

    uint8_t jump_enable = 0;   // This value checks wheter the player is jumping or not

    float vSpeed = 0;   // Vertical speed for the player
    float hSpeed = 0;   // Horizontal speed for the player

    float vAccel = 0.2;   // Vertical and horizontal acceleration for the player
    float hAccel = 0;

    uint8_t width = 0;
    uint8_t height = 0;

    long oldMovementUpdate = 0;   // This is used to give the inputs a refresh rate

    long oldPhysics = 0;          // This is used to give the physics a refresh rate

    private:
      bool flagMoveLeft = false;
      bool flagMoveRight = false;
      bool flagJump = false;
};


#endif
