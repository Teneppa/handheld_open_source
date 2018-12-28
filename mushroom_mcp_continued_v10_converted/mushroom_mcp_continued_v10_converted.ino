/*

Tämä koodin on käännetty STM32 alustalta ATmegalle.
Konsolissa oli myös tarkoitus olla START, SELECT ja Q napit.
Saa nähdä millä ilveellä saan tämän käännettyä konsolille, jossa ei ole kuin kahdeksan nappia.

*/


/*****************************ADAFRUIT OLED LIBRARY*****************************/

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

#define OLED_DC     6
#define OLED_CS     7
#define OLED_RESET  8

Adafruit_SSD1306 m_oled(OLED_DC, OLED_RESET, OLED_CS);

/********************************My own libraries******************************/

#include "io.h"

/************************************VARIABLES*********************************/

//variables
long oldtime = 0;

uint8_t mAvatarX = 9;
uint8_t mAvatarY = 20;

uint8_t m_jump_enable = 0;

float velocity = 2;

uint8_t oldup = 0;
uint8_t olddown = 0;

uint8_t selected_game = 1;

/* variables for the space invaders */
uint8_t xcoord = 0;
uint8_t ycoord = 0;

uint8_t bx = 0; //bullet x
uint8_t by = 0; //bullet y

//moves the enemies around
uint8_t enemy_x_offset = 0;
uint8_t enemy_y_offset = 0;
int8_t enemydir = 1;

uint8_t spinv_count = 0;
uint8_t spinv_enemies[20] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

//variables for controlling the shooting
uint8_t shooting = 0;
uint8_t bullet = 0;

//variables for the pingpong
uint8_t ballx = 20;
uint8_t bally = 20;

uint8_t bat_l = 24;
uint8_t bat_r = 24;

int8_t dirx = 1;
int8_t diry = -1;

uint8_t fps_pingpong = 60;

/*
   Collision works in 8x8 cubes
   Top = left
   Bottom = right

   Collision has total of 6x14 resolution

   It uses only the first 6 bits

   It doesn't contain the edges of the game arena!
*/

/*define here the amount of levels you've done into the m_collision_boxes array to optimize the memory usage*/
#define LEVELSMADE 5

//array of the levels
const uint8_t m_collision_boxes[14 * LEVELSMADE] = {
  //level 1
  B01000000,
  B00000000,
  B00000000,
  B10000000,
  B00000000,
  B00000000,
  B10000000,
  B11000000,
  B11100000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,

  //level 2
  B00000000,
  B00000000,
  B10000000,
  B00100000,
  B00000000,
  B00010000,
  B00000000,
  B00100000,
  B00000000,
  B11110000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,

  //level 3
  B00000000,
  B10000000,
  B11000000,
  B11100000,
  B11110000,
  B11111000,
  B11111000,
  B00000000,
  B00001000,
  B00000000,
  B00001000,
  B00000000,
  B00000000,
  B00000000,

  //level 4
  B00000000,
  B10000000,
  B00000000,
  B11000000,
  B11010000,
  B10111000,
  B00000000,
  B00000000,
  B00001000,
  B00000000,
  B00001000,
  B00000000,
  B00000000,
  B00000000,

  //level 5
  B00000000,
  B10000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
};

//array of the levels
const uint8_t m_coin_boxes[14 * LEVELSMADE] = {
  //level 1
  B00000000,
  B00000000,
  B00000000,
  B01000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,

  //level 2
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,

  //level 3
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,

  //level 4
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,

  //level 5
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
};

//array of the levels
const uint8_t m_spike_boxes[14 * LEVELSMADE] = {
  //level 1
  B00000000,
  B10000000,
  B10000000,
  B00000000,
  B10000000,
  B10000000,
  B00000000,
  B00000000,
  B00000000,
  B10000000,
  B10000000,
  B00000000,
  B00000000,
  B00000000,

  //level 2
  B00000000,
  B00000000,
  B00000000,
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  B00000000,
  B10000000,
  B00000000,
  B00000000,
  B00000000,

  //level 3
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,

  //level 4
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,

  //level 5
  B00000000,
  B00000000,
  B10000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
};

//array of the spikes in the mushroom levels

//variables to help finding the positions of the cubes
uint8_t mAvaColX = 0;
uint8_t mAvatar_level = 0;

/****************************BITMAPS*******************************************/

const unsigned char PROGMEM block [] = {
  0xFF, 0xC3, 0x81, 0x99, 0x99, 0x81, 0xC3, 0xFF,
};

const unsigned char PROGMEM mushroom [] = {
  0x3C, 0x5A, 0x5A, 0xFF, 0x24, 0x24, 0x24, 0x66,
};


const unsigned char PROGMEM spike [] = {
  0x18, 0x18, 0x24, 0x24, 0x24, 0x42, 0x42, 0x42,
};


const unsigned char PROGMEM alien [] = {
  0x42, 0x24, 0x7E, 0xDB, 0xFF, 0xBD, 0x24, 0x18,
};

//12x8
const unsigned char PROGMEM ss_ship [] = {
  0xFF, 0x80, 0x15, 0x60, 0x0A, 0xBC, 0x05, 0x57, 0x06, 0xAB, 0x0D, 0x5C, 0x1B, 0xF0, 0xFF, 0x80
};

//logo 56x11
const uint8_t PROGMEM mush_logo [] = {
  0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xB8, 0x04, 0x8D, 0x2F, 0x18, 0xCE, 0x01, 0xAA, 0x84, 0x91, 0x29,
  0x25, 0x2A, 0xA1, 0x8D, 0x64, 0x99, 0xEF, 0x25, 0x23, 0x59, 0x89, 0x24, 0x85, 0x2A, 0x25, 0x22,
  0x49, 0x89, 0x27, 0x99, 0x29, 0x18, 0xC2, 0x49, 0x80, 0x30, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x80,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
};

//press start 42x5
const uint8_t PROGMEM press_start [] = {
  0xEE, 0xDD, 0xC1, 0xDD, 0xDD, 0xAA, 0x91, 0x01, 0x09, 0x54, 0xEE, 0xDD, 0xC1, 0xC9, 0xDC, 0x8C,
  0x84, 0x40, 0x49, 0x58, 0x8A, 0xDD, 0xC1, 0xC9, 0x54, 0x80,
};

const uint8_t PROGMEM coin1 [] = {
0x18, 0x24, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 
};

const uint8_t PROGMEM coin2 [] = {
0x10, 0x28, 0x44, 0x44, 0x44, 0x44, 0x28, 0x10, 
};

const uint8_t PROGMEM coin3 [] = {
0x10, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x10, 
};

/********************VOIDS***************************************************/

void setup() {
  pinMode(A7, INPUT);
  //randomSeed(A7);
  m_oled.begin(SSD1306_SWITCHCAPVCC); //use internal voltage generator
  m_oled.setRotation(2); //set rotation to 2
  IObegin(); //set IO pins

  //if Q is pressed while booting, dim the display to save power
  if (KEYQ) {
    m_oled.dim(1);
  }

  /*
    scrollText(F("Made by Teemu"),1500, 20);
  */
  scrollText(F("Teukkakonsoli"),500, 15);
}

void loop() {
  m_oled.setTextSize(1);
  m_oled.setTextColor(1);
  taulukko(F("Ping Pong"), F("Space Invaders"), F("Mushroom"), F("Gunman"), F("Tanks"), F("Sidescroller"), selected_game);
  if (KEYUP) {
    if (oldup == 0) {
      selected_game--;
    }
    oldup = 1;
  } else {
    oldup = 0;
  }

  if (KEYDOWN) {
    if (olddown == 0) {
      selected_game++;
    }
    olddown = 1;
  } else {
    olddown = 0;
  }
  if (selected_game < 1) {
    selected_game = 1;
  }
  if (selected_game > 6) {
    selected_game = 6;
  }

  if (KEYRIGHT) {

    switch (selected_game) {

      //Ping pong
      case 1:
        while (!KEYQ) {
          if (refresh(fps_pingpong)) {
            m_oled.clearDisplay();
            pingpong();
            m_oled.display();
          }
        }
        break;

      //space invaders
      case 2:
        while (!KEYQ) {
          if (refresh(100)) {
            m_oled.clearDisplay();
            game_spaceinvaders();
            m_oled.display();
          }
        }
        setVarsNull();
        break;

      //mushroom game
      case 3:
        {
          uint8_t m_show_s_txt = 1;
          long old_mushstart = millis();
          while (!KEYX) {
            m_oled.clearDisplay();

            //handle the startup thing
            m_oled.drawBitmap(35, 6, mush_logo, 56, 11, 1);
            if (millis() - old_mushstart > 500) {
              m_show_s_txt = 1 - m_show_s_txt;
              old_mushstart = millis();
            }

            if (m_show_s_txt == 1) {
              m_oled.setCursor(32, 31);
              m_oled.println(F("PRESS START"));
            }

            m_oled.display();

            if (KEYQ) {
              break;
            }
          }
          while (!KEYQ) {
            if (refresh(60)) {
              m_oled.clearDisplay();
              mushroomGame();
              m_oled.display();
            }
          }
          clearMushVars();
        }
        break;

      //gunman
      case 4:
        while (!KEYQ) {
          if (refresh(80)) {
            m_oled.clearDisplay();
            gunman();
            m_oled.display();
          }
        }
        gm_clear_variables();
        break;

      //tank game
      case 5:
        while (!KEYQ) {
          m_oled.clearDisplay();
          tankgame();
          m_oled.display();
        }
        tank_clear_vars();
        tank_clear_scores();
        break;

      /*
            //Settings
            case 6:
              settings_begin();
              while (!KEYQ) {
                m_oled.clearDisplay();
                settings();
                m_oled.display();
              }
              break;
      */

      //sidescroller
      case 6:
        while (!KEYQ) {
          if (refresh(50)) {
            m_oled.clearDisplay();
            sidescroller();
            m_oled.display();
          }
        }
        break;

      default:
        while (!KEYQ) {
          m_oled.clearDisplay();
          m_oled.setTextColor(1);
          m_oled.setCursor(0, 0);
          m_oled.println(F("Error no code\n"));
          m_oled.println(F("Press Q to return to the menu"));
          m_oled.display();
        }
        break;
    }
  }
}

void taulukko(String line1, String line2, String line3, String line4, String line5, String line6, uint8_t selected_line) {
  m_oled.clearDisplay();
  //sides
  m_oled.drawLine(0, 0, 0, 63, 1);
  m_oled.drawLine(127, 0, 127, 63, 1);

  //line 1
  if (selected_line == 1) {
    m_oled.setTextColor(0, 1);
  } else {
    m_oled.setTextColor(1, 0);
  }
  m_oled.drawLine(0, 0, 127, 0, 1);
  m_oled.setCursor(2, 2);
  m_oled.println(line1);
  //line 2
  if (selected_line == 2) {
    m_oled.setTextColor(0, 1);
  } else {
    m_oled.setTextColor(1, 0);
  }
  m_oled.drawLine(0, 10, 127, 10, 1);
  m_oled.setCursor(2, 12);
  m_oled.println(line2);
  //line 3
  if (selected_line == 3) {
    m_oled.setTextColor(0, 1);
  } else {
    m_oled.setTextColor(1, 0);
  }
  m_oled.drawLine(0, 20, 127, 20, 1);
  m_oled.setCursor(2, 22);
  m_oled.println(line3);
  //line 4
  if (selected_line == 4) {
    m_oled.setTextColor(0, 1);
  } else {
    m_oled.setTextColor(1, 0);
  }
  m_oled.drawLine(0, 30, 127, 30, 1);
  m_oled.setCursor(2, 32);
  m_oled.println(line4);
  //line 5
  if (selected_line == 5) {
    m_oled.setTextColor(0, 1);
  } else {
    m_oled.setTextColor(1, 0);
  }
  m_oled.drawLine(0, 40, 127, 40, 1);
  m_oled.setCursor(2, 42);
  m_oled.println(line5);
  //line 6
  if (selected_line == 6) {
    m_oled.setTextColor(0, 1);
  } else {
    m_oled.setTextColor(1, 0);
  }
  m_oled.drawLine(0, 50, 127, 50, 1);
  m_oled.setCursor(2, 52);
  m_oled.println(line6);
  m_oled.drawLine(0, 63, 127, 63, 1);

  m_oled.display();
}

/*****************************************************************************************************************************/

void game_spaceinvaders() {
  m_oled.clearDisplay();
  spinv_draw_avatar();
  spinv_draw_enemies();
  spinv_checkIfwon();
  m_oled.display();
}

/*****************************************************************************************************************************/

void spinv_draw_avatar()  {
  ycoord = 55;
  if (KEYLEFT) {
    if (xcoord >= 1) {
      xcoord -= 1;
    }
  }
  if (KEYRIGHT) {
    xcoord += 1;

    if (xcoord > 120) {
      xcoord = 120;
    }
  }

  if (KEYX) {
    if (!shooting) {
      bullet = 1;
      shooting = 1;
      bx = xcoord + 4;
      by = ycoord + 3;
    }
  }

  //bullet control
  if (bullet == 1) {
    if (by > 0) {
      m_oled.drawLine(bx, by, bx, by - 2, 1);
      by -= 1;

      if (by < 12 + enemy_y_offset) {
        for (int x = 0; x < 10; x++) {
          if ((bx >= enemy_x_offset + x * 10) && (bx <= (x * 10 + 10) + enemy_x_offset)) {
            if (spinv_enemies[x] == 1) {
              if (spinv_enemies[x + 10] == 0) {
                spinv_enemies[x] = 0;
                by = -1;
                bullet = 0;
              }
            }
          }
        }
      }

      if (by < 23 + enemy_y_offset) {
        for (int x = 0; x < 10; x++) {
          if ((bx >= enemy_x_offset + x * 10) && (bx <= (x * 10 + 10) + enemy_x_offset)) {
            if (spinv_enemies[x + 10] == 1) {
              spinv_enemies[x + 10] = 0;
              by = -1;
              bullet = 0;
            }
          }
        }
      }
      if (by < 1) {
        by = -1;
        bullet = 0;
      }
    }

  } else {
    shooting = 0;
  }
  m_oled.drawLine(xcoord + 3, ycoord, xcoord + 4, ycoord, 1);
  m_oled.drawLine(xcoord + 1, ycoord + 1, xcoord + 6, ycoord + 1, 1);
  m_oled.drawRect(xcoord, ycoord + 2, 8, 2, 1);
}

/*****************************************************************************************************************************/

void spinv_draw_enemies() {
  if (enemydir == 1) {
    enemy_x_offset++;
    if (enemy_x_offset > 30) {
      enemydir = -1;
    }
  }
  if (enemydir == -1) {
    enemy_x_offset--;
    if (enemy_x_offset < 1) {
      enemydir = 1;
      spinv_count++;
    }
  }

  if (spinv_count >= 2) {
    enemy_y_offset += 10;
    spinv_count = 0;
  }

  //if enemies y_offset is
  if (enemy_y_offset > 40) {
    SPI_gameEnd(0);
  }

  //draw all enemies
  for (uint8_t x = 0; x < (sizeof(spinv_enemies)); x++) {
    //Serial.println(spinv_enemies[x]);
    if (spinv_enemies[x]) {
      if (x < 10) {
        m_oled.drawBitmap(x * 10 + enemy_x_offset, enemy_y_offset, alien, 8, 8, 1);
      } else {
        m_oled.drawBitmap((x - 10) * 10 + enemy_x_offset, enemy_y_offset + 11, alien, 8, 8, 1);
      }
    }
  }

}

/*****************************************************************************************************************************/

void SPI_gameEnd(uint8_t win_or_loss) {
  while (1) {
    m_oled.clearDisplay();

    m_oled.setTextSize(2);
    m_oled.setTextColor(WHITE);
    m_oled.setCursor(0, 0);

    if (win_or_loss) {
      m_oled.println(F("YOU WON!"));
    } else {
      m_oled.println(F("GAME OVER!"));
    }

    m_oled.setTextSize(1);
    m_oled.println("");
    m_oled.println(F("Press start to restart, Q to go home"));

    if ((KEYSTART) || (KEYQ)) {
      setVarsNull();
      break;
    }

    m_oled.display();
  }
}

/*****************************************************************************************************************************/

void spinv_checkIfwon() {
  for (uint8_t x = 0; x < (sizeof(spinv_enemies)); x++) {
    if (spinv_enemies[x] == true) {
      return;
    }
  }
  SPI_gameEnd(1);
}

/*****************************************************************************************************************************/

void setVarsNull() {
  bx = 0; //bullet x
  by = 0; //bullet y

  //moves the enemies around
  enemy_x_offset = 0;
  enemy_y_offset = 0;
  enemydir = 1;

  spinv_count = 0;

  for (uint8_t x = 0; x < sizeof(spinv_enemies); x++) {
    spinv_enemies[x] = 1;
  }

  //variables for controlling the shooting
  shooting = 0;
  bullet = 0;
}

/*****************************************************************************************************************************/

void pingpong() {
  if (bat_l < 2) {
    bat_l = 2;
  }
  if (bat_l > 51) {
    bat_l = 51;
  }
  if (bat_r < 2) {
    bat_r = 2;
  }
  if (bat_r > 51) {
    bat_r = 51;
  }

  ballx += dirx;
  bally += diry;

  if (KEYUP) {
    bat_l--;
  }
  if (KEYDOWN) {
    bat_l++;
  }

  if (KEYY) {
    bat_r--;
  }
  if (KEYA) {
    bat_r++;
  }

  //draw everything
  //draw room
  m_oled.drawRect(0, 0, 127, 63, 1);
  //draw ball
  m_oled.fillRect(ballx, bally, 8, 8, 1);
  //draw bats
  m_oled.fillRect(2, bat_l, 2, 10, 1);
  m_oled.fillRect(123, bat_r, 2, 10, 1);

  //check collisions
  if (ballx >= 115) {
    if ((bally + 8 >= bat_r) && (bally <= bat_r + 10)) {
      dirx = -1;
    } else {
      //When the game is lost
      m_oled.clearDisplay();
      m_oled.setCursor(0, 0);
      m_oled.println(F("Game over!\n"));
      m_oled.println(F("Q = menu\nStart = restart"));
      m_oled.display();
      while ((!KEYSTART) && (!KEYQ)) {

      }
      dirx = -1; //make sure that the game wont start bugging
    }
  }
  if (ballx <= 5) {
    if ((bally + 8 >= bat_l) && (bally <= bat_l + 10)) {
      dirx = 1;
    } else {
      //When the game is lost
      m_oled.clearDisplay();
      m_oled.setCursor(0, 0);
      m_oled.println(F("Game over!\n"));
      m_oled.println(F("Q = menu\nStart = restart"));
      m_oled.display();
      while ((!KEYSTART) && (!KEYQ)) {

      }
      dirx = 1; //make sure that the game wont start bugging
    }
  }

  if (bally >= 53) {
    diry = -1;
  }
  if (bally <= 2) {
    diry = 1;
  }
}

bool refresh(uint8_t fps) {
  if (millis() - oldtime > (1.0000 / fps) * 1000.0000) {
    oldtime = millis();
    return true;
  }
  return false;
}
