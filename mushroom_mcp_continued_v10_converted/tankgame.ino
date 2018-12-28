float tank_x = 20;
float tank_y = 20;
float tank_rot = 90;
uint8_t tank_shooting = 0;

float tank2_x = 103;
float tank2_y = 40;
float tank2_rot = 270;
uint8_t tank2_shooting = 0;

float tank_speed = 0.6;

float bullet_x = 0;
float bullet_y = 0;
float bullet_rot = 0;

float bullet2_x = 0;
float bullet2_y = 0;
float bullet2_rot = 0;

float bullet_speed = 0.5;

uint8_t tank_score = 0;
uint8_t tank2_score = 0;

#define DWIDTH 127
#define DHEIGHT 63

#define drot dtr(tank_rot)
#define d2rot dtr(tank2_rot)
#define BDROT dtr(bullet_rot)
#define BD2ROT dtr(bullet2_rot)

#define RESTART_TANK_GAME_KEY (KEYLEFT & KEYRIGHT & KEYX & KEYB)

float dtr(double degree) {
  return ((2 * PI * degree) / 360);
}

void tankgame() {
  m_oled.drawRect(0, 0, 127, 63, 1);
  drawTank(tank_x, tank_y, dtr(tank_rot));
  drawTank(tank2_x, tank2_y, dtr(tank2_rot));
  handleTankShooting();
  handleTank2Shooting();
  checkAllShootingCollisions();
  if (refresh(60)) {
    handleTankMovement();
  }
}

void checkAllShootingCollisions() {
  /*
    Serial.print(tank_x);
    Serial.print(":");
    Serial.print(tank_y);
    Serial.print(" bullet ");
    Serial.print(bullet_x);
    Serial.print(":");
    Serial.print(bullet_y);

    Serial.print(" <--Tank 2--> ");

    Serial.print(tank2_x);
    Serial.print(":");
    Serial.print(tank2_y);
    Serial.print(" bullet2 ");
    Serial.print(bullet2_x);
    Serial.print(":");
    Serial.println(bullet2_y);
  */


  if (tank_shooting) {

    //left player bullet
    if ((bullet_x > tank2_x - 2) && (bullet_x < tank2_x + 2)) {
      if ((bullet_y > tank2_y - 2) && (bullet_y < tank2_y + 2)) {
        tank_score++;
        tank_over(1);
      }
    }
  }

  //right player bullet
  if (tank2_shooting) {
    if ((bullet2_x > tank_x - 2) && (bullet2_x < tank_x + 2)) {
      if ((bullet2_y > tank_y - 2) && (bullet2_y < tank_y + 2)) {
        tank2_score++;
        tank_over(2);
      }
    }
  }

}

void tank_clear_scores() {
  tank_score = 0;
  tank2_score = 0;
}

void tank_clear_vars() {
  bullet_x = 0;
  bullet_y = 0;

  bullet2_x = 0;
  bullet2_y = 0;

  tank_x = 20;
  tank_y = 20;
  tank_rot = 90;
  tank_shooting = 0;

  tank2_x = 103;
  tank2_y = 40;
  tank2_rot = 270;
  tank2_shooting = 0;
}

void tank_over(uint8_t winner) {
  tank_clear_vars();

  while (!(RESTART_TANK_GAME_KEY)) {
    m_oled.clearDisplay();
    m_oled.setCursor(0, 0);
    if (winner == 1) {
      m_oled.println(F("Left won!"));
    } else {
      m_oled.println(F("Right won!"));
    }
    m_oled.print(tank_score);
    m_oled.print(":");
    m_oled.println(tank2_score);
    m_oled.println(F("\nGame over!"));
    m_oled.println(F("Press LEFT, RIGHT, X and B to restart!"));
    m_oled.display();

    if (KEYQ) {
      break;
    }
  }
}

void handleTankShooting() {
  if ((KEYSHOOTLEFT) && (tank_shooting == 0)) {
    tank_shooting = 1;
    bullet_x = tank_x;
    bullet_y = tank_y;
    bullet_rot = tank_rot;
  }

  if (tank_shooting == 1) {
    bullet_x += cos(BDROT) * bullet_speed;
    bullet_y += sin(BDROT) * bullet_speed;

    if ((bullet_x > DWIDTH - 4) || (bullet_x < 4) || (bullet_y > DHEIGHT - 4) || (bullet_y < 4)) {
      stop_shooting();
    } else {
      m_oled.drawPixel(bullet_x, bullet_y, 1);
    }
  }
}

void handleTank2Shooting() {
  if ((KEYSHOOTRIGHT) && (tank2_shooting == 0)) {
    tank2_shooting = 1;
    bullet2_x = tank2_x;
    bullet2_y = tank2_y;
    bullet2_rot = tank2_rot;
  }

  if (tank2_shooting == 1) {
    bullet2_x += cos(BD2ROT) * bullet_speed;
    bullet2_y += sin(BD2ROT) * bullet_speed;

    if ((bullet2_x > DWIDTH - 4) || (bullet2_x < 4) || (bullet2_y > DHEIGHT - 4) || (bullet2_y < 4)) {
      stop_shooting2();
    } else {
      m_oled.drawPixel(bullet2_x, bullet2_y, 1);
    }
  }
}

void stop_shooting() {
  bullet_x = 0;
  bullet_y = 0;

  tank_shooting = 0;
}

void stop_shooting2() {
  bullet2_x = 0;
  bullet2_y = 0;

  tank2_shooting = 0;
}

void handleTankMovement() {
  /* HANDLE MOVEMENT IN HERE */
  if (KEYLEFT) {
    tank_rot -= tank_speed * 5;
  }
  if (KEYRIGHT) {
    tank_rot += tank_speed * 5;
  }

  if (KEYX) {
    tank2_rot -= tank_speed * 5;
  }
  if (KEYB) {
    tank2_rot += tank_speed * 5;
  }

  if (tank_rot < 1) {
    tank_rot = 360;
  }

  if (tank_rot > 360) {
    tank_rot = 1;
  }

  if (tank2_rot < 1) {
    tank2_rot = 360;
  }

  if (tank2_rot > 360) {
    tank2_rot = 1;
  }

  if (KEYUP) {
    tank_x = tank_x + cos(drot) * tank_speed;
    tank_y = tank_y + sin(drot) * tank_speed;
  }

  if (KEYDOWN) {
    tank_x = tank_x - cos(drot) * tank_speed;
    tank_y = tank_y - sin(drot) * tank_speed;
  }

  if (KEYY) {
    tank2_x = tank2_x + cos(d2rot) * tank_speed;
    tank2_y = tank2_y + sin(d2rot) * tank_speed;
  }

  if (KEYA) {
    tank2_x = tank2_x - cos(d2rot) * tank_speed;
    tank2_y = tank2_y - sin(d2rot) * tank_speed;
  }

  if (tank_x > DWIDTH - 4) {
    tank_x = DWIDTH - 4;
  }
  if (tank_x < 4) {
    tank_x = 4;
  }

  if (tank_y > DHEIGHT - 4) {
    tank_y = DHEIGHT - 4;
  }
  if (tank_y < 4) {
    tank_y = 4;
  }

  if (tank2_x > DWIDTH - 4) {
    tank2_x = DWIDTH - 4;
  }
  if (tank2_x < 4) {
    tank2_x = 4;
  }

  if (tank2_y > DHEIGHT - 4) {
    tank2_y = DHEIGHT - 4;
  }
  if (tank2_y < 4) {
    tank2_y = 4;
  }
}

void drawTank(float x, float y, float rotation) {
  float px = 0;
  float py = 0;

  px = x + cos(rotation) * 4;
  py = y + sin(rotation) * 4;

  m_oled.drawLine(x, y, px, py, 1);

  float cx = x + cos(rotation - PI / 2);
  float cy = y + sin(rotation - PI / 2);

  px = cx + cos(rotation);
  py = cy + sin(rotation);

  m_oled.drawLine(cx, cy, px, py, 1);

  cx = x + cos(rotation + PI / 2);
  cy = y + sin(rotation + PI / 2);

  px = cx + cos(rotation);
  py = cy + sin(rotation);

  m_oled.drawLine(cx, cy, px, py, 1);
}
