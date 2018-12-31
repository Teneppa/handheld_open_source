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


/********************************************************************************************/
/* Sprite structure                                                                         */
/********************************************************************************************/
class Sprite {
  public:
    virtual uint16_t getWidth() = 0;
    virtual uint16_t getHeight() = 0;
    virtual uint16_t isPixelOn(uint16_t x, uint16_t y) = 0;
};

class tilengine {
    public:
      tilengine(uint8_t width, uint8_t height);   // Define the width and height of the tile-engine
      /********************************************************************************************/
      /* This deconstructor frees the memory that has been allocated from the constructor         */
      /********************************************************************************************/
      ~tilengine() {
        delete [] puskuri;
      };   // Deconstructor to free the allocated memory
  
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

/********************************************************************************************/
/* This function allocates memory for the tile-engine and saves the size of the buffer.     */
/********************************************************************************************/
tilengine::tilengine(uint8_t getWidth, uint8_t getHeight) {
  tilecount = getWidth * getHeight / 16;    // Calculate how many uint16_t variables we need to contain the map
  width = getWidth;     // Get the amount of the tiles in x-direction
  height = getHeight;   // Get the amount of the tiles in y-direction

  puskuri = new uint16_t[tilecount];   // Allocate memory for the tiles
}

inline bool tilengine::checkTile(uint8_t getX, uint8_t getY) {
  uint8_t x = getX/8;
  uint8_t y = getY/8;
  
  return ((puskuri[y] & (1 << x)) >> x);
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
        oled.drawBitmap(xpos, ypos, block, 8, 8, 1);
      }

    }
  }
}

/********************************************************************************************/
/* This function checks if (x,y) contains a tile                                            */
/********************************************************************************************/
uint8_t tilengine::checkCollision(uint16_t xpos, uint16_t ypos, Sprite* sprite) {

  uint8_t flags_to_return = COLLISION_NONE;
  uint8_t width = sprite->getWidth()-1;
  uint8_t height = sprite->getHeight()-1;

  /* Check upper left corner */
  if(checkTile(xpos, ypos)) {
    //flags_to_return |= COLLISION_UP_LEFT;
    bitSet(flags_to_return, 1);
  }

  /* Check upper right corner */
  if(checkTile(xpos+width, ypos)) {
    //flags_to_return |= COLLISION_UP_RIGHT;
    bitSet(flags_to_return, 2);
  }

  /* Check lower right corner */
  if(checkTile(xpos+width, ypos+height)) {
    //flags_to_return |= COLLISION_DOWN_RIGHT;
    bitSet(flags_to_return, 3);
  }

  /* Check lower left corner */
  if(checkTile(xpos, ypos+height)) {
    //flags_to_return |= COLLISION_DOWN_LEFT;
    bitSet(flags_to_return, 4);
  }
  
  return flags_to_return;
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

    void handleInputs(bool keyLeft, bool keyRight, bool keyJump, uint16_t updateInterval);
    void handlePhysics(uint8_t readCollisionFlags);

    uint16_t x = 0;
    uint16_t y = 0;

    uint8_t width = 0;
    uint8_t height = 0;

    long oldMovementUpdate = 0;

    private:
      bool flagMoveLeft = false;
      bool flagMoveRight = false;
      bool flagJump = false;
};

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
  oled.drawBitmap(x, y, playerBitmap, width, height, 1);
}

// Move player to decided position
void playerCharacter::move(uint16_t movex, uint16_t movey) {
  x = movex;
  y = movey;
}

// Control player movement
void playerCharacter::handleInputs(bool keyLeft, bool keyRight, bool keyJump, uint16_t updateInterval) {
  
  if(millis() - oldMovementUpdate > updateInterval) {
    if(keyLeft) {
      flagMoveLeft = true;
    }

    if(keyRight) {
      flagMoveRight = true;
    }
    
    oldMovementUpdate = millis();
  }

}

// This function calculates the physics
void playerCharacter::handlePhysics(uint8_t readCollisionFlags) {

  bool upLeft = bitRead(readCollisionFlags, 1);      // UP_LEFT
  bool upRight = bitRead(readCollisionFlags, 2);     // UP_RIGHT
  bool downRight = bitRead(readCollisionFlags, 3);   // DOWN_RIGHT
  bool downLeft = bitRead(readCollisionFlags, 4);    // DOWN_LEFT

  //Serial.println(String(upLeft)+","+String(upRight)+","+String(downRight)+","+String(downLeft));

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
  
}

/********************************************************************************************/
/* The actual code starts from here!                                                        */
/********************************************************************************************/

//#include "tilengine.h"
#include "io.h"

tilengine engine(16, 8);  // Create an engine object with width of 16 and height of 8
playerCharacter player(8, 8, mushroom);   // Create an avatar

void setup() {
  Serial.begin(115200);

  beginDisplay();     // Configure the OLED

  IObegin();

  oled.clearDisplay();    // Clear the buffer
  oled.display();         // Push the clear buffer to the screen

  engine.copyBuffer(map1);   // Copy the map to the tile-engine's buffer

  player.move(8, 64 - 16);
}

const String cstr[5] = {"NONE", "UP_LEFT", "UP_RIGHT", "DOWN_RIGHT", "DOWN_LEFT"};

/*
const uint8_t COLLISION_NONE = 0;
const uint8_t COLLISION_UP_LEFT = 0b1;
const uint8_t COLLISION_UP_RIGHT = 0b10;
const uint8_t COLLISION_DOWN_RIGHT = 0b100;
const uint8_t COLLISION_DOWN_LEFT = 0b1000;
*/

#define ENABLE_DEBUGGING
//#undef  ENABLE_DEBUGGING

void loop() {
  oled.clearDisplay();

  engine.draw();
  uint8_t collisionFlags = engine.checkCollision(player.x, player.y, &player);

  #ifdef ENABLE_DEBUGGING
    Serial.print(collisionFlags, BIN);
    Serial.print("\t");

    String msg = "";
    String erotusmerkki = " | ";
  
    if(collisionFlags == 0) { msg += cstr[0]; }
    if(bitRead(collisionFlags, 1)) { msg += cstr[1] + erotusmerkki; }
    if(bitRead(collisionFlags, 2)) { msg += cstr[2] + erotusmerkki; }
    if(bitRead(collisionFlags, 3)) { msg += cstr[3] + erotusmerkki; }
    if(bitRead(collisionFlags, 4)) { msg += cstr[4] + erotusmerkki; }
  
    Serial.println(msg);

    delay(20);
  #endif

  player.draw();
  player.handleInputs(KEY_LEFT, KEY_RIGHT, KEY_X, 16);
  player.handlePhysics(collisionFlags);

  oled.display();
}
