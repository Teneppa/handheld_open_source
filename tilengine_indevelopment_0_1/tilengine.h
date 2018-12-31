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

      void pointToBlockDrawingFunction(void(*f)(uint8_t x, uint8_t y));
      void (*drawWithPointedFunction)(uint8_t, uint8_t);
  
      uint16_t * puskuri;       // Buffer for the tiles
  
      inline bool checkTile(uint8_t getX, uint8_t getY);    // This function checks if there is a tile in this position of the map
  
      void copyBuffer(uint16_t * bufferToCopy);    // Copy a buffer to the tilebuffer
  
      String returnConfig();  // Just some configuration integers for debugging
  
      uint16_t tilecount = 0; // Sum of the tiles
      uint8_t width = 0;      // Tiles in x-direction
      uint8_t height = 0;     // Tiles in y-direction
  
      /* Different collision types */
      const uint8_t COLLISION_NONE = 0;
      const uint8_t COLLISION_UP_LEFT = 0b1;
      const uint8_t COLLISION_UP_RIGHT = 0b10;
      const uint8_t COLLISION_DOWN_RIGHT = 0b100;
      const uint8_t COLLISION_DOWN_LEFT = 0b1000;
  
      void draw();
  
      uint8_t checkCollision(uint16_t xpos, uint16_t ypos, Sprite* sprite);      // Check if this coordinate has a tile
};

#endif
