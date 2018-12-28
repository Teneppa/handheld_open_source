float ss_x = 0;
float ss_y = 10;

float ss_x_accel = 0;
float ss_y_accel = 0.1;

float SS_UPDOWN_ACCEL = 0.1;
float SS_MAX_ACCEL = 1.5;

uint8_t ss_shooting = 0;

uint8_t ss_bullet_x = 0;
uint8_t ss_bullet_y = 0;

uint8_t ss_bullet_speed = 2;

uint8_t ss_enemy[2] = {0,0};
uint8_t ss_enemy_onscreen = 0;
uint8_t ss_enemy_speed = 1;

void ss_clear_variables(){
ss_x = 0;
ss_y = 10;

ss_x_accel = 0;
ss_y_accel = 0.1;

ss_shooting = 0;

ss_bullet_x = 0;
ss_bullet_y = 0;

ss_enemy[0] = 0;
ss_enemy[1] = 0;

ss_enemy_onscreen = 0;
ss_enemy_speed = 1;
}

void ss_draw_background() {
  
}

void sidescroller() {
  ss_draw_background();
  ss_draw_ship();
  ss_handle_movement();
  ss_handle_shooting();
  ss_handle_enemies();
}

void ss_draw_ship() {
  m_oled.drawBitmap(ss_x, ss_y, ss_ship, 16, 8, 1);
}

void ss_handle_enemies() {
  if(ss_enemy_onscreen == 0) {
    ss_enemy[1] = random(0,50);
    ss_enemy[0] = 126;
    ss_enemy_speed = random(1,3);
    ss_enemy_onscreen = 1;
  }else{
    m_oled.drawBitmap(ss_enemy[0],ss_enemy[1],alien,8,8,1);
    ss_enemy[0] -= ss_enemy_speed;
    if(ss_enemy[0] < 0) {
      ss_enemy_onscreen = 0;
    }
  }

  if(ss_enemy[0] < ss_x + 16) {
    if((ss_enemy[1] < ss_y+8) && (ss_enemy[1] >= ss_y)){
      ss_game_over();
    }
  }
  
}

void ss_game_over(){
  ss_clear_variables();
  while(!KEYSTART) {
    m_oled.clearDisplay();
  blinkTextCenter("Game Over",500);
  m_oled.display();
  if(KEYQ){break;}
  }
}

void ss_handle_shooting() {
  if (KEYX) {
    if(shooting == 0) {
      shooting = 1;
      ss_bullet_x = ss_x+16;
      ss_bullet_y = ss_y+4;
    }
  }

  if(shooting == 1) {
    m_oled.drawPixel(ss_bullet_x,ss_bullet_y,1);
    ss_bullet_x += ss_bullet_speed;
    if(ss_bullet_x>127) {
      shooting = 0;
    }
  }
  
}

void ss_handle_movement() {
  if (KEYDOWN) {
    ss_y_accel += SS_UPDOWN_ACCEL;
  }

  if (KEYUP) {
    ss_y_accel -= SS_UPDOWN_ACCEL;
  }

  if ((!KEYDOWN) && (!KEYUP)) {
    if (ss_y_accel > 0) {
      ss_y_accel -= SS_UPDOWN_ACCEL;
    }
    if (ss_y_accel < 0) {
      ss_y_accel += SS_UPDOWN_ACCEL;
    }
  }

  ss_y += ss_y_accel;

  if (KEYRIGHT) {
    ss_x++;
  }
  if (KEYLEFT) {
    ss_x--;
  }

  if (ss_x < 5) {
    ss_x = 5;
  }

  if (ss_x > 30) {
    ss_x = 30;
  }

  if (ss_y_accel > SS_MAX_ACCEL) {
    ss_y_accel = SS_MAX_ACCEL;
  }

  if (ss_y_accel < SS_MAX_ACCEL * -1) {
    ss_y_accel = SS_MAX_ACCEL * -1;
  }

  if (ss_y < 1) {
    ss_y_accel = +0.5;
  }
  if (ss_y > 55) {
    ss_y_accel = -0.5;
  }
}
