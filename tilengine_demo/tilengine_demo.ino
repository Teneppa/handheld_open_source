/*****************************ADAFRUIT OLED LIBRARY*****************************/

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

#define OLED_DC     6
#define OLED_CS     7
#define OLED_RESET  8

Adafruit_SSD1306 oled(OLED_DC, OLED_RESET, OLED_CS);

void beginDisplay() {
  oled.begin(SSD1306_SWITCHCAPVCC); //use internal voltage generator
  oled.setRotation(2); //set rotation to 2
}

/****************************BITMAPS*******************************************/

const unsigned char PROGMEM block [] = {
  0xFF, 0xC3, 0x81, 0x99, 0x99, 0x81, 0xC3, 0xFF,
};

const unsigned char PROGMEM mushroom [] = {
  0x3C, 0x5A, 0x5A, 0xFF, 0x24, 0x24, 0x24, 0x66,
};

/**************************-= TILENGINE=-***************************************/


/*****-= map =-********/
const uint16_t map1[8] {
  0b1111111111111111,
  0b1000000000000001,
  0b1000000000000001,
  0b1000000000000001,
  0b1000000000000001,
  0b1011000000000001,
  0b1000000000000001,
  0b1111111111111111,
};

struct tilengine {
  tilengine(uint8_t width, uint8_t height);   // Define the width and height of the tile-engine


  uint16_t * puskuri;       // Buffer for the tiles

  void copyBuffer(uint16_t * bufferToCopy);    // Copy a buffer to the tilebuffer

  String returnConfig();  // Just some configuration integers for debugging

  uint16_t tilecount = 0; // Sum of the tiles
  uint8_t width = 0;      // Tiles in x-direction
  uint8_t height = 0;     // Tiles in y-direction

  void draw(uint16_t * xpos, uint16_t * ypos, uint8_t * drawFlag, uint8_t * clearFlag);   // This is the function that draws the entire tilemap
  
  private:
    uint16_t drawXCursor = 0;   // Cursor coordinates for the tiles
    uint16_t drawYCursor = 0;
    
};

/* Allocate memory and save the size of the tile-engine */
tilengine::tilengine(uint8_t getWidth, uint8_t getHeight) {
  tilecount = getWidth*getHeight/16;
  width = getWidth;     // Get the amount of the tiles in x-direction
  height = getHeight;   // Get the amount of the tiles in y-direction
  
  puskuri = new uint16_t[tilecount];   // Allocate memory for the tiles
}

/* This function converts the tilemap to graphics */
void tilengine::draw(uint16_t * xpos, uint16_t * ypos, uint8_t * drawFlag, uint8_t * clearFlag) {


      /*  Special thanks to An0num0us from the Together C & C++ discord group who created the algorithm
          For the x- and y-coordinate bitmap check

          You can find the discord group from this link: https://discord.gg/mTT559R
      */
      if(((map1[drawYCursor] & (1<<drawXCursor))>>drawXCursor)) { // Check if there is a bitmap to draw on this tile
        *xpos = uint16_t((width-1)*8-drawXCursor*8);  // Get the coordinates for the bitmap
        *ypos = uint16_t(drawYCursor*8);
        *drawFlag = 1;                    // Set the draw flag to 1
      }else{
        *drawFlag = 0;
      }

      if(drawXCursor<width-1) {
        drawXCursor++;
      }else{
        drawXCursor = 0;          // If the cursor goes out of the screen
        drawYCursor++;
      }

      if(drawYCursor>=height) {   // If the entire tilemap has been drawn
        drawXCursor = 0;
        drawYCursor = 0;
        *clearFlag = 1;
      }
}

/* This function moves a buffer to the tile-engines buffer */
void tilengine::copyBuffer(uint16_t * bufferToCopy) {
  memcpy(puskuri, bufferToCopy, sizeof(uint16_t)*width*height/16);
}

/* Just a debugging function */
String tilengine::returnConfig() {
  String str = "";

  str += "Width: " + String(width) + "\n";
  str += "Height: " + String(height) + "\n";

  return str;
}

/******************************************************************************/

//#include "tilengine.h"

tilengine engine(16, 8);  // Create an engine object with width of 16 and height of 8

void setup() {
  //Serial.begin(115200);
  
  beginDisplay();     // Configure the OLED

  oled.clearDisplay();    // Clear the buffer
  oled.display();         // Push the clear buffer to the screen

  engine.copyBuffer(map1);   // Copy the map to the tile-engine's buffer

  //Serial.println(engine.returnConfig());
}

void loop() {
  uint16_t tilex, tiley;    // Coordinates for the bitmaps
  uint8_t drawFlag = 0;     // If 1, you need to draw a bitmap to (tilex, tiley) on the display
  uint8_t clearFlag = 0;    // If 1, you need to draw everything and then clear the display
  
  engine.draw(&tilex, &tiley, &drawFlag, &clearFlag);

  // Draw a bitmap
  if(drawFlag == 1) {
    oled.drawBitmap(tilex, tiley, block, 8, 8, 1);
  }

  // Push the buffer to the display and clear it
  if(clearFlag == 1) {
    oled.display();
    oled.clearDisplay();
  }
}