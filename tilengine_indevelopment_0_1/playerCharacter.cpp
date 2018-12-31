#include "playerCharacter.h"

void playerCharacter::pointToPlayerDrawingFunction(void(*f)(uint8_t x, uint8_t y)) {
  //f(x,y);
  drawWithPointedFunction = f;
}

/********************************************************************************************/
/* playerCharacter library starts form here!                                                */
/********************************************************************************************/
playerCharacter::playerCharacter(uint8_t bitmapWidth, uint8_t bitmapHeight, uint8_t * bitmap) {
  playerBitmap = bitmap;
  width = bitmapWidth;
  height = bitmapHeight;
}

// Draw player
void playerCharacter::draw() {
  //oled.drawBitmap(x, y, playerBitmap, width, height, 1);
  drawWithPointedFunction(x,y);
}

// Move player to decided position
void playerCharacter::move(uint16_t movex, uint16_t movey) {
  x = movex;
  y = movey;
}

/********************************************************************************************/
/* This function reads the inputs and sets flags based on them                              */
/********************************************************************************************/
void playerCharacter::handleInputs(bool keyLeft, bool keyRight, bool keyJump, uint16_t updateInterval) {
  
  if(millis() - oldMovementUpdate > updateInterval) {
    if(keyLeft) {
      flagMoveLeft = true;
    }

    if(keyRight) {
      flagMoveRight = true;
    }

    if(keyJump) {
      flagJump = true;
    }
    
    oldMovementUpdate = millis();
  }

}

/********************************************************************************************/
/* This function calculates the physics for the playerCharacter                             */
/********************************************************************************************/
void playerCharacter::handlePhysics(uint8_t readCollisionFlags, uint16_t updateInterval) {

  if(millis() - oldPhysics > updateInterval) {

    bool upLeft = bitRead(readCollisionFlags, 1);      // UP_LEFT
    bool upRight = bitRead(readCollisionFlags, 2);     // UP_RIGHT
    bool downRight = bitRead(readCollisionFlags, 3);   // DOWN_RIGHT
    bool downLeft = bitRead(readCollisionFlags, 4);    // DOWN_LEFT
    bool onGround = bitRead(readCollisionFlags, 5);    // DOWN-1_MIDDLE
  
    //Serial.println(String(upLeft)+","+String(upRight)+","+String(downRight)+","+String(downLeft));
  
  
    /***************************-= Movement in sideways =-***************************/
  
    // If got input for left movement
    if(flagMoveLeft) {
      if((downLeft == false) && (upLeft == false)) {
        x--;                    // Move player left
        flagMoveLeft = false;   // Set flag to false
      }
    }
  
    // If got input for left movement
    if(flagMoveRight) {
      if((downRight == false) && (upRight == false)) {
        x++;                    // Move player left
        flagMoveRight = false;   // Set flag to false
      }
    }
    
    // If player is still in the wall
    if(downRight | upRight) {
      x--;
    }
  
    if(downLeft | upLeft) {
      x++;
    }
  
    /***************************-= Jumping =-***************************/
  
    // If jump key has been pressed
    if(flagJump) {
      if(onGround) {
        jump_enable = 1;
        vSpeed = -2;
    
        flagJump = false;
      }

      if(jump_enable == 1) {
        flagJump = false;
      }
    }
  
    // If jumping
    if(jump_enable == 1) {
      y += vSpeed;
      vSpeed += vAccel;
  
      if(vSpeed>=0) {
        vSpeed = 0;
        jump_enable = 0;
      }
    }
  
    // If falling
    if(jump_enable == 0) {
  
      // If player is on the air
      if(!onGround) {
        y += vSpeed;
        vSpeed += vAccel;
      }
    }

    oldPhysics = millis();
  }
}
