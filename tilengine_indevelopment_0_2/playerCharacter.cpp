/*
 *  This mess is created by Teemu Laurila in 31.12.2018
 */

#include "playerCharacter.h"

/********************************************************************************************/
/* This function stores the function pointer for the player drawing function                */
/********************************************************************************************/
void playerCharacter::pointToPlayerDrawingFunction(void(*f)(uint8_t x, uint8_t y)) {
  drawWithPointedFunction = f;
}

/********************************************************************************************/
/* playerCharacter library starts form here!                                                */
/********************************************************************************************/
playerCharacter::playerCharacter(uint8_t bitmapWidth, uint8_t bitmapHeight) {
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
void playerCharacter::handlePhysics(uint8_t readCollisionFlags, bool playerCollides, uint16_t updateInterval) {

  if(millis() - oldPhysics > updateInterval) {  

    bool upLeft = bitRead(readCollisionFlags, 1);      // UP_LEFT
    bool upRight = bitRead(readCollisionFlags, 2);     // UP_RIGHT
    bool downRight = bitRead(readCollisionFlags, 3);   // DOWN_RIGHT
    bool downLeft = bitRead(readCollisionFlags, 4);    // DOWN_LEFT
    bool onGround = bitRead(readCollisionFlags, 5);    // DOWN-1_MIDDLE
    bool above = bitRead(readCollisionFlags, 6);       // Tile above

    /*
    Serial.print("physics: ");
    Serial.print(upRight ? "up_r" : "");
    Serial.print(downRight ? "| down_r" : "");
    Serial.print(upLeft ? "| up_l" : "");
    Serial.print(downLeft ? "| down_l" : "");
    Serial.print(onGround ? "| on_ground" : "");
    Serial.print(above ? "| above" : "");

    Serial.println();
    */
    
    if((upLeft) || (downLeft)) {
      x++;
    }
    if((upRight) || (downRight)) {
      x--;
    }

    if((jump_enable == 0) && onGround) {
      y = oldY;
    }

    /*
    if(playerCollides) {
      x = oldX;
      y = oldY;

      return;
    }
    */
  
    // If got input for left movement
    if(flagMoveLeft) {
      if(!upLeft && !downLeft) {
        x--;                    // Move player left
        flagMoveLeft = false;   // Set flag to false
      }
    }
  
    // If got input for left movement
    if(flagMoveRight) {
      if(!upRight && !downRight) {
        x++;                    // Move player left
        flagMoveRight = false;   // Set flag to false
      }
    }
    /***************************-= Jumping =-***************************/
  
    // If jump key has been pressed
    if(flagJump) {
      if(onGround && !upLeft && !upRight) {
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

      // If in the highest point of the jump
      if(vSpeed>=0) {
        vSpeed = 0;
        jump_enable = 0;
      }

      // If player collided with a tile above
      if(above) {
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

    oldX = x;
    oldY = y;
  }  
}
