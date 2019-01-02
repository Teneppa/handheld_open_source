/*
 *  This mess is created by Teemu Laurila in 31.12.2018
 */

#include "tilengine.h"

/********************************************************************************************/
/* This function allocates memory for the tile-engine and saves the size of the buffer.     */
/********************************************************************************************/
tilengine::tilengine(uint8_t getWidth, uint8_t getHeight) {
  tilecount = getWidth * getHeight / 16;    // Calculate how many uint16_t variables we need to contain the map
  width = getWidth;     // Get the amount of the tiles in x-direction
  height = getHeight;   // Get the amount of the tiles in y-direction

  puskuri = new uint16_t[tilecount];   // Allocate memory for the tiles
}

void tilengine::pointToBlockDrawingFunction(void(*f)(uint8_t x, uint8_t y)) {
  drawWithPointedFunction = f;
}

/********************************************************************************************/
/* This function draws the map                                                              */
/********************************************************************************************/
void tilengine::draw() {
  for (uint8_t y = 0; y < height; y++) {
    for (uint8_t x = 0; x < width; x++) {

      /*  Special thanks to An0num0us from the Together C & C++ discord group who created the algorithm
          For the x- and y-coordinate bitmap check

          You can find the discord group from this link: https://discord.gg/mTT559R
      */

      if (((puskuri[y] & (1 << x)) >> x)) { // Check if there is a bitmap to draw on this tile
        uint16_t xpos = uint16_t((width - 1) * 8 - x * 8); // Get the coordinates for the bitmap
        uint16_t ypos = uint16_t(y * 8);
        drawWithPointedFunction(xpos,ypos);                // Use the function pointer to drwa the bitmap
      }

    }
  }
}

/********************************************************************************************/
/* This function checks whether there is a tile at (x,y) or not                             */
/********************************************************************************************/
inline bool tilengine::checkTile(uint8_t getX, uint8_t getY) {
  
  /* New method (I tested this in Visual Studio and it gave perfect values) */
  uint8_t x = width-1-((getX-getX%8)/8);
  uint8_t y = (getY-getY%8)/8;
  
  return ((puskuri[y] & (1 << x)) >> x);  // Return true if tile was found from the coordinates
}

/********************************************************************************************/
/* This function checks if (x,y) contains a tile                                            */
/********************************************************************************************/
uint8_t tilengine::checkCollision(uint16_t xpos, uint16_t ypos, Sprite* sprite) {

  uint8_t flags_to_return = 0;
  uint8_t sprWidth = sprite->getWidth()-1;
  uint8_t sprHeight = sprite->getHeight()-1;

  //Serial.println(String(sprWidth)+" x "+String(sprHeight));

  /* Check upper left corner */
  if(checkTile(xpos, ypos)) {
    bitSet(flags_to_return, 1);
  }

  //Serial.println(ypos+sprHeight);

  /* Check upper right corner */
  if(checkTile(xpos+sprWidth, ypos)) {
    bitSet(flags_to_return, 2);
  }

  /* Check lower right corner */
  if(checkTile(xpos+sprWidth, ypos+sprHeight)) {
    bitSet(flags_to_return, 3);
  }

  /* Check lower left corner */
  if(checkTile(xpos, ypos+sprHeight)) {
    bitSet(flags_to_return, 4);
  }

  /* Check if player is on the ground (Left down-1 OR Right down-1)*/
  if(checkTile(xpos, ypos+sprHeight+1) || checkTile(xpos+sprWidth, ypos+sprHeight+1)) {
    bitSet(flags_to_return, 5);
  }

  /* Check if tile above */
  if(checkTile(xpos, ypos-1) || checkTile(xpos+sprWidth, ypos-1)) {
    bitSet(flags_to_return, 6);
  }
  
  return flags_to_return;   // Return the flags as an uint8_t
}

/********************************************************************************************/
/* This function returns boolean                                                            */
/********************************************************************************************/
bool tilengine::isColliding(uint16_t xpos, uint16_t ypos, Sprite* sprite) {

  uint8_t width = sprite->getWidth()-1;
  uint8_t height = sprite->getHeight()-1;

  // If any of the corners overlap a tile
  if(checkTile(xpos, ypos) || checkTile(xpos+width, ypos) || checkTile(xpos, ypos+height) || checkTile(xpos+width, ypos+height)) {
    uint16_t tile_x = xpos-xpos%8;
    uint16_t tile_y = ypos-ypos%8;

    bool x_overlaps = (xpos < tile_x+width) && (xpos+width > tile_x);
    bool y_overlaps = (ypos < tile_y+height) && (ypos+height > tile_y);

    bool collision = x_overlaps && y_overlaps;

    return collision;
    
  }else{
    return false;
  }
  
}
/********************************************************************************************/
/* This function moves a buffer to the tile-engines buffer                                  */
/********************************************************************************************/
void tilengine::copyBuffer(uint16_t * bufferToCopy) {
  memcpy(puskuri, bufferToCopy, sizeof(uint16_t)*width * height / 16);
}

/********************************************************************************************/
/* This function is for debugging the configuration                                         */
/********************************************************************************************/
String tilengine::returnConfig() {
  String str = "";

  str += "Width: " + String(width) + "\n";
  str += "Height: " + String(height) + "\n";

  return str;
}
