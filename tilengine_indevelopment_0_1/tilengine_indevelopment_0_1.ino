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

/********************************************************************************************/
/* The actual code starts from here!                                                        */
/********************************************************************************************/

#include "levels.h"
#include "bitmaps.h"
#include "tilengine.h"
#include "playerCharacter.h"
#include "io.h"

tilengine engine(16, 8);  // Create an engine object with width of 16 and height of 8
playerCharacter player(8, 8, mushroom);   // Create an avatar

void drawBlock(uint8_t x, uint8_t y) {
  oled.drawBitmap(x,y,block,8,8,1);
}

void drawPlayer(uint8_t x, uint8_t y) {
  oled.drawBitmap(x,y,mushroom,8,8,1);
}

void setup() {
  Serial.begin(115200);   // Serial communication for debugging

  beginDisplay();     // Configure the OLED

  IObegin();          // Set digital pins to input and apply pullup-resistors

  oled.clearDisplay();    // Clear the buffer
  oled.display();         // Push the clear buffer to the screen

  engine.copyBuffer(map1);   // Copy the map to the tile-engine's buffer

  player.move(8, 64 - 16);  // Move player to the lower left corner of the level

  player.pointToPlayerDrawingFunction(drawPlayer);    // Give playeCharacter a function to draw the player's bitmap
  engine.pointToBlockDrawingFunction(drawBlock);      // Give tilengine a function to draw the blocks
}

#define ENABLE_DEBUGGING
#undef  ENABLE_DEBUGGING


#ifdef ENABLE_DEBUGGING
  const String PROGMEM cstr[6] = {"NONE", "UP_LEFT", "UP_RIGHT", "DOWN_RIGHT", "DOWN_LEFT", "ON_GROUND"};
#endif

void loop() {
  oled.clearDisplay();  // Clear the display

  engine.draw();        // Draw the map
  uint8_t collisionFlags = engine.checkCollision(player.x+player.hSpeed, player.y+player.vSpeed, &player);    // Get the collision flags

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
    if(bitRead(collisionFlags, 5)) { msg += cstr[5] + erotusmerkki; }
  
    Serial.println(msg);

    delay(20);
  #endif

  player.draw();    // Draw player
  player.handleInputs(KEY_LEFT, KEY_RIGHT, KEY_X, 16);    // Give inputs to the player
  player.handlePhysics(collisionFlags, 16);               // Handle player physics and collisions

  oled.display(); // Push the buffer to the display
}
