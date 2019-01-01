/*
 *  This mess is created by Teemu Laurila in 31.12.2018
 */

#ifndef _TILENGINE_H_
#define _TILENGINE_H_

#include "Arduino.h"
#include "sprite.h"

/********************************************************************************************/
/* tilengine starts from here!                                                              */
/********************************************************************************************/
class tilengine {
    public:
      tilengine(uint8_t width, uint8_t height);   // Define the width and height of the tile-engine
      /********************************************************************************************/
      /* This deconstructor frees the memory that has been allocated from the constructor         */
      /********************************************************************************************/
      ~tilengine() {
        delete [] puskuri;
      };   // Deconstructor to free the allocated memory

      void pointToBlockDrawingFunction(void(*f)(uint8_t x, uint8_t y));   // Points to the block drawing function
      void (*drawWithPointedFunction)(uint8_t, uint8_t);
  
      uint16_t * puskuri;       // Buffer for the tiles
  
      inline bool checkTile(uint8_t getX, uint8_t getY);    // This function checks if there is a tile in this position of the map
  
      void copyBuffer(uint16_t * bufferToCopy);    // Copy a buffer to the tilebuffer
  
      String returnConfig();  // Just some configuration integers for debugging
  
      uint16_t tilecount = 0; // Sum of the tiles
      uint8_t width = 0;      // Tiles in x-direction
      uint8_t height = 0;     // Tiles in y-direction
  
      void draw();  // Draws the entire map
  
      uint8_t checkCollision(uint16_t xpos, uint16_t ypos, Sprite* sprite);      // Check if this coordinate has a tile
      bool isColliding(uint16_t xpos, uint16_t ypos, Sprite* sprite);   // Return boolean if player is overlapping a tile
};

#endif
