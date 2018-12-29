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


/********************************************************************************************/
/* This array contains the map                                                              */
/********************************************************************************************/
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
    /********************************************************************************************/
    /* This deconstructor frees the memory that has been allocated from the constructor         */
    /********************************************************************************************/
    ~tilengine() {
      delete [] puskuri;
    };   // Deconstructor to free the allocated memory

    uint16_t * puskuri;       // Buffer for the tiles

    void copyBuffer(uint16_t * bufferToCopy);    // Copy a buffer to the tilebuffer

    String returnConfig();  // Just some configuration integers for debugging

    uint16_t tilecount = 0; // Sum of the tiles
    uint8_t width = 0;      // Tiles in x-direction
    uint8_t height = 0;     // Tiles in y-direction

    void draw();

    bool checkCollision(uint16_t xpos, uint16_t ypos);

  private:
    uint16_t drawXCursor = 0;   // Cursor coordinates for the tiles
    uint16_t drawYCursor = 0;

};

/********************************************************************************************/
/* This function allocates memory for the tile-engine and saves the size of the buffer.     */
/********************************************************************************************/
tilengine::tilengine(uint8_t getWidth, uint8_t getHeight) {
  tilecount = getWidth * getHeight / 16;
  width = getWidth;     // Get the amount of the tiles in x-direction
  height = getHeight;   // Get the amount of the tiles in y-direction

  puskuri = new uint16_t[tilecount];   // Allocate memory for the tiles
}

/********************************************************************************************/
/* This function draws the map */
/********************************************************************************************/
void tilengine::draw() {
  for (uint8_t y = 0; y < height; y++) {
    for (uint8_t x = 0; x < width; x++) {

      if (((map1[y] & (1 << x)) >> x)) { // Check if there is a bitmap to draw on this tile
        uint16_t xpos = uint16_t((width - 1) * 8 - x * 8); // Get the coordinates for the bitmap
        uint16_t ypos = uint16_t(y * 8);
        oled.drawBitmap(xpos, ypos, block, 8, 8, 1);
      }

    }
  }
}

/********************************************************************************************/
/* This function checks if (x,y) contains a tile                                            */
/********************************************************************************************/
bool tilengine::checkCollision(uint16_t xpos, uint16_t ypos) {
  uint16_t x = xpos / 8;
  uint16_t y = ypos / 8;

  if (((map1[y] & (1 << x)) >> x)) { // Check if there is a tile
    return 1;
  }

  return 0;
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

/********************************************************************************************/
/* playerCharacter library starts form here!                                                */
/********************************************************************************************/
struct playerCharacter {
    playerCharacter(uint8_t bitmapWidth, uint8_t bitmapHeight, uint8_t * bitmap);    // Constructor for the player

    uint8_t * playerBitmap; // Store the pointer of the player's bitmap

    void draw();  // Draw the player

    void move(uint16_t movex, uint16_t movey);
    void moveRelative(uint16_t movex, uint16_t movey);

    uint16_t x = 0;
    uint16_t y = 0;

  private:
    uint8_t width = 0;
    uint8_t height = 0;

};

playerCharacter::playerCharacter(uint8_t bitmapWidth, uint8_t bitmapHeight, uint8_t * bitmap) {
  playerBitmap = bitmap;
  width = bitmapWidth;
  height = bitmapHeight;
}

void playerCharacter::draw() {
  oled.drawBitmap(x, y, playerBitmap, width, height, 1);
}

// Move player to decided position
void playerCharacter::move(uint16_t movex, uint16_t movey) {
  x = movex;
  y = movey;
}

/********************************************************************************************/
/* The actual code starts from here!                                                        */
/********************************************************************************************/

//#include "tilengine.h"

tilengine engine(16, 8);  // Create an engine object with width of 16 and height of 8
playerCharacter player(8, 8, mushroom);   // Create an avatar

void setup() {
  //Serial.begin(115200);

  beginDisplay();     // Configure the OLED

  oled.clearDisplay();    // Clear the buffer
  oled.display();         // Push the clear buffer to the screen

  engine.copyBuffer(map1);   // Copy the map to the tile-engine's buffer

  //Serial.println(engine.returnConfig());

  player.move(8, 64 - 16);
}

void loop() {
  oled.clearDisplay();

  engine.draw();

  player.draw();

  oled.display();
}
