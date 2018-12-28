const unsigned char PROGMEM character_right [] = {
  0x38, 0x38, 0x10, 0x17, 0x1C, 0x10, 0x10, 0x18,
};

const unsigned char PROGMEM character_left [] = {
  0x1C, 0x1C, 0x08, 0xE8, 0x38, 0x08, 0x08, 0x18,
};

float gm_x = 0;
float gm_y = 0;

float gm2_x = 110;
float gm2_y = 0;

float gm_y_accel = 0;
float gm2_y_accel = 0;

uint8_t gm_shooting = 0;
uint8_t gm2_shooting = 0;

/* DEFINE GM MOVEMENTS AND AMMUNITIONS */
#define GM_STARTING_AMMO 20
#define GM_JUMPHEIGHT 10

#define GM_BULLET_SPEED 2

/* DEFINE GM LIMITS */
#define GM_MAX_X 127-10
#define GM_MIN_X 0
#define GM_HEALTH 10

uint8_t gm_ammo_left = GM_STARTING_AMMO;
uint8_t gm2_ammo_left = GM_STARTING_AMMO;

uint8_t gm_magazine = GM_STARTING_AMMO;
uint8_t gm2_magazine = GM_STARTING_AMMO;

uint8_t gm_lives = GM_HEALTH;
uint8_t gm2_lives = GM_HEALTH;

uint8_t gm_jumping = 0;
uint8_t gm2_jumping = 0;

int8_t gm_dir = 1;
int8_t gm2_dir = -1;

uint8_t gm_jumping_height = 0;
uint8_t gm2_jumping_height = 0;

uint8_t gm_bullet_x = 0;
uint8_t gm_bullet_y = 0;
int8_t gm_bullet_dir = 0;

uint8_t gm2_bullet_x = 0;
uint8_t gm2_bullet_y = 0;
int8_t gm2_bullet_dir = 0;

#define GUNMAN_RESTART_GAME_BUTTON (KEYUP & KEYY)

/* Variables to control the speed and collisions */
float gm_player_speed = 0.6;

#define GM_ACCELERATION_JUMP 0.02
#define GM_ACCELERATION_FALL 0.04

void gunman() {
  gm_handle_movement();
  gm_handle_graphics();
  gm_handle_shooting();
}

void gm_handle_shooting() {

  if (gm_shooting == 0) {
    if (gm2_shooting == 0) {
      if ((gm_ammo_left == 0) && (gm2_ammo_left == 0)) {
        gm_gameover(0);
      }
    }
  }

  /* LEFT PLAYER SHOOTING CODE */

  if (KEYDOWN) {
    if (gm_shooting == 0) {
      if (gm_ammo_left > 0) {
        gm_shooting = 1;
        gm_bullet_x = gm_x + gm_dir;
        gm_bullet_y = gm_y + 4;
        gm_bullet_dir = gm_dir;
        gm_ammo_left -= 1;
      }
    }
  }

  if (gm_shooting == 1) {
    gm_bullet_x += gm_bullet_dir * GM_BULLET_SPEED;

    //check if bullet hits other player
    if ((gm_bullet_x >= gm2_x) && (gm_bullet_x <= gm2_x + 5)) {
      if ((gm_bullet_y > gm2_y) && (gm_bullet_y < gm2_y + 8)) {
        //left player hit the right player
        gm2_lives -= 1;
        if (gm2_lives < 1) {
          gm_gameover(1);
        }
        gm_shooting = 0;
      }
    }

    //check if bullet goes out of the level
    if ((gm_bullet_x < 0) || (gm_bullet_x > 127)) {
      gm_shooting = 0;
    } else {
      m_oled.drawPixel(gm_bullet_x, gm_bullet_y, 1);
    }

  }

  /* RIGHT PLAYER SHOOTING CODE */

  if (KEYA) {
    if (gm2_shooting == 0) {
      if (gm2_ammo_left > 0) {
        gm2_shooting = 1;
        gm2_bullet_x = gm2_x + gm2_dir;
        gm2_bullet_y = gm2_y + 4;
        gm2_bullet_dir = gm2_dir;
        gm2_ammo_left -= 1;
      }
    }
  }

  if (gm2_shooting == 1) {
    gm2_bullet_x += gm2_bullet_dir * GM_BULLET_SPEED;

    //check if bullet hits other player
    if ((gm2_bullet_x >= gm_x) && (gm2_bullet_x <= gm_x + 5)) {
      if ((gm2_bullet_y > gm_y) && (gm2_bullet_y < gm_y + 8)) {
        //left player hit the right player
        gm_lives -= 1;
        if (gm_lives < 1) {
          gm_gameover(2);
        }
        gm2_shooting = 0;

      }
    }

    //check if bullet goes out of the level
    if ((gm2_bullet_x < 0) || (gm2_bullet_x > 127)) {
      gm2_shooting = 0;
    } else {
      m_oled.drawPixel(gm2_bullet_x, gm2_bullet_y, 1);
    }

  }
}

void gm_gameover(uint8_t winner) {
  while (1) {
    m_oled.clearDisplay();
    m_oled.setCursor(0, 0);
    if (winner == 1) {
      m_oled.println(F("Left won!"));
    }
    if (winner == 2) {
      m_oled.println(F("Right won!"));
    }

    if (winner == 0) {
      m_oled.println(F("Draw!"));
    }

    m_oled.println(F("\nGame over!"));
    m_oled.println(F("Press UP and Y"));
    m_oled.display();

    if ((GUNMAN_RESTART_GAME_BUTTON)) {
      break;
    }
  }
  gm_clear_variables();
}

void gm_clear_variables() {
  gm_x = 0;
  gm_y = 0;

  gm2_x = 110;
  gm2_y = 0;

  gm_shooting = 0;
  gm2_shooting = 0;

  gm_ammo_left = GM_STARTING_AMMO;
  gm2_ammo_left = GM_STARTING_AMMO;

  gm_magazine = GM_STARTING_AMMO;
  gm2_magazine = GM_STARTING_AMMO;

  gm_lives = GM_HEALTH;
  gm2_lives = GM_HEALTH;

  gm_jumping = 0;
  gm2_jumping = 0;

  gm_dir = 1;
  gm2_dir = -1;

  gm_jumping_height = 0;
  gm2_jumping_height = 0;

  gm_bullet_x = 0;
  gm_bullet_y = 0;
  gm_bullet_dir = 0;

  gm_y_accel = 0;
  gm2_y_accel = 0;
}

void gm_handle_movement() {

  if (gm_jumping == 0) {
    if (gm_y < 53) {
      gm_y += gm_y_accel;
      gm_y_accel += GM_ACCELERATION_FALL;
    } else {
      if (KEYUP) {
        gm_jumping = 1;
        gm_jumping_height = gm_y - GM_JUMPHEIGHT;
        if (gm_jumping_height < 1) {
          gm_jumping_height = 0;
        }
      }
    }
  } else {
    if (gm_y > gm_jumping_height) {
      gm_y -= gm_y_accel;
      gm_y_accel += GM_ACCELERATION_JUMP;
    } else {
      gm_jumping = 0;
      gm_y_accel = 0;
    }
  }

  if (gm2_jumping == 0) {
    if (gm2_y < 53) {
      gm2_y += gm2_y_accel;
      gm2_y_accel += GM_ACCELERATION_FALL;
    } else {
      if (KEYY) {
        gm2_jumping = 1;
        gm2_jumping_height = gm2_y - GM_JUMPHEIGHT;
        if (gm2_jumping_height < 1) {
          gm2_jumping_height = 0;
        }
      }
    }
  } else {
    if (gm2_y > gm2_jumping_height) {
      gm2_y -= gm2_y_accel;
      gm2_y_accel += GM_ACCELERATION_JUMP;
    } else {
      gm2_jumping = 0;
      gm2_y_accel = 0;
    }
  }

  /* player number 1 (left side) */
  if (KEYRIGHT) {
    gm_x += gm_player_speed;
    gm_dir = 1;
  }
  if (KEYLEFT) {
    gm_x -= gm_player_speed;
    gm_dir = -1;
  }
  if (gm_x < GM_MIN_X) {
    gm_x = GM_MIN_X;
  }
  if (gm_x > GM_MAX_X) {
    gm_x = GM_MAX_X;
  }

  /* player number 2 (left side) */
  if (KEYB) {
    gm2_x += gm_player_speed;
    gm2_dir = 1;
  }
  if (KEYX) {
    gm2_x -= gm_player_speed;
    gm2_dir = -1;
  }
  if (gm2_x < GM_MIN_X) {
    gm2_x = GM_MIN_X;
  }
  if (gm2_x > GM_MAX_X) {
    gm2_x = GM_MAX_X;
  }
}

void gm_handle_graphics() {
  //draw players

  /* left player */
  if (gm_dir == 1) {
    m_oled.drawBitmap(gm_x, gm_y, character_right, 8, 8, 1);
  } else {
    m_oled.drawBitmap(gm_x, gm_y, character_left, 8, 8, 1);
  }

  /* right player */
  if (gm2_dir == 1) {
    m_oled.drawBitmap(gm2_x, gm2_y, character_right, 8, 8, 1);
  } else {
    m_oled.drawBitmap(gm2_x, gm2_y, character_left, 8, 8, 1);
  }

  //draw scoreboard

  /* left side of the scoreboard */
  m_oled.setCursor(3, 2);
  m_oled.println(gm_lives);
  m_oled.setCursor(3, 10);
  m_oled.print(gm_ammo_left);
  m_oled.print("/");
  m_oled.println(gm_magazine);

  /* right side of the scoreboard */
  m_oled.setCursor(90, 2);
  m_oled.println(gm2_lives);
  m_oled.setCursor(90, 10);
  m_oled.print(gm2_ammo_left);
  m_oled.print("/");
  m_oled.println(gm2_magazine);

  //draw edges
  m_oled.drawRect(0, 0, 127, 63, 1);
}
