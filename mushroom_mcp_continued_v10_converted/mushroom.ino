long oldMushCoinRot = 0;
uint8_t mushCoinRot = 1;

uint32_t mushroomScore = 0;

uint8_t fixedy = 0;

/**************************clearMushVars*************************************/

void clearMushVars() {
  mAvatarX = 9;
  mAvatarY = 20;
  m_jump_enable = 0;

  mAvaColX = 0;
  mAvatar_level = 0;

  velocity = 2;

  mushroomScore = 0;
  oldMushCoinRot = 0;
  mushCoinRot = 1;
}

/**************************mushroomGame**************************************/

void mushroomGame() {
  m_oled.clearDisplay();
  drawMushRoom();
  drawMushCoins();
  drawMushAvatar();
  m_oled.display();
}

void drawMushCoins() {
  fixedy = (48 - mAvatarY + 5) / 8;
  if ((m_coin_boxes[(mAvatarX - 8) / 8 + mAvatar_level * 14]) & 1 << (7 - fixedy)) {
    mushroomScore += 1;
  }

  m_oled.setCursor(10, 10);
  m_oled.println(mushroomScore);

  for (int x = 1; x <= 14; x++) {
    for (int d = 1; d <= 6; d++) {
      if (m_coin_boxes[x - 1 + mAvatar_level * 14] & (1 << (7 - (d - 1)))) {
        switch (mushCoinRot) {
          case 1:
            m_oled.drawBitmap(8 * x, 56 - (8 * d), coin1, 8, 8, 1);
            break;

          case 2:
            m_oled.drawBitmap(8 * x, 56 - (8 * d), coin2, 8, 8, 1);
            break;

          case 3:
            m_oled.drawBitmap(8 * x, 56 - (8 * d), coin3, 8, 8, 1);
            break;

          case 4:
            m_oled.drawBitmap(8 * x, 56 - (8 * d), coin2, 8, 8, 1);
            break;
        }
      }
    }
  }

  if (millis() - oldMushCoinRot > 100) {
    oldMushCoinRot = millis();
    mushCoinRot++;

    if (mushCoinRot > 4) {
      mushCoinRot = 1;
    }
  }

}

void mushOver() {
  clearMushVars();
  long old_mushstart = millis();
  uint8_t m_show_s_txt = 1;
  while (!KEYX) {
    m_oled.clearDisplay();

    drawTextCenter(F("Game over!"));

    //draw the logo
    m_oled.drawBitmap(35, 6, mush_logo, 56, 11, 1);
    if (millis() - old_mushstart > 500) {
      m_show_s_txt = 1 - m_show_s_txt;
      old_mushstart = millis();
    }

    if (m_show_s_txt == 1) {
      m_oled.setCursor(32, 40);
      m_oled.println(F("PRESS START"));
    }

    m_oled.display();
    if (KEYQ) {
      break;
    }
  }
}

/****************drawMushRoom***********************************************/

void drawMushRoom() {
  //if avatar moves beyond 110, move the camera forward
  if (mAvatarX >= 110) {
    mAvatar_level++;
    mAvatarX = 10;
  }

  //if avatar moves under 9 in x-axel, move the camera backwards
  if ((mAvatarX <= 9) && (mAvatar_level != 0)) {
    mAvatar_level--;
    mAvatarX = 109;
  }

  //draw walls
  for (int x = 0; x < 16; x++) {
    m_oled.drawBitmap(x * 8, 0, block, 8, 8, 1);
    m_oled.drawBitmap(x * 8, 56, block, 8, 8, 1);
  }
  for (int y = 1; y < 7; y++) {
    m_oled.drawBitmap(0, y * 8, block, 8, 8, 1);
    m_oled.drawBitmap(120, y * 8, block, 8, 8, 1);
  }

  //draw the map
  for (int x = 1; x <= 14; x++) {
    for (int d = 1; d <= 6; d++) {
      if (m_collision_boxes[x - 1 + mAvatar_level * 14] & (1 << (7 - (d - 1)))) {
        //draw blocks
        m_oled.drawBitmap(8 * x, 56 - (8 * d), block, 8, 8, 1);
      } else {
        if (m_spike_boxes[x - 1 + mAvatar_level * 14] & (1 << (7 - (d - 1)))) {
          //draw spikes
          m_oled.drawBitmap(8 * x, 56 - (8 * d), spike, 8, 8, 1);
        }
      }
    }
  }
}

/********************drawMushAvatar****************************************/

void drawMushAvatar() {
  fixedy = (48 - mAvatarY + 5) / 8;

  //draw avatar
  m_oled.drawBitmap(mAvatarX, mAvatarY, mushroom, 8, 8, 1);

  //check if player hits the spikes
  if ((m_spike_boxes[(mAvatarX - 8) / 8 + mAvatar_level * 14]) & 1 << (7 - fixedy)) {
    mushOver();
  }

  //if not jumping
  if (m_jump_enable == 0) {

    if (mAvatarX <= 9) {
      mAvatarX = 9;
    }

    mAvaColX = mAvatarX / 8;

    //check dropping collision
    if (((m_collision_boxes[(mAvatarX - 8) / 8 + mAvatar_level * 14] & 1 << (8 - fixedy)) || (m_collision_boxes[(mAvatarX) / 8 + mAvatar_level * 14] & (1 << (8 - fixedy))))) {
      velocity = 0;
    } else {
      mAvatarY += velocity;
      velocity += 0.5;
    }
  }

  //check jumping collision
  if (m_jump_enable == 1) {

    if ((m_collision_boxes[mAvaColX + mAvatar_level * 14] & 2 << (mAvatarY / 8 - 1)) || (m_collision_boxes[((mAvatarX - 1) / 8 - 1) + mAvatar_level * 14] & 2 << (mAvatarY / 8 - 1))) {
      velocity = 0;
      m_jump_enable = 0;
    } else {
      mAvatarY += velocity;
      velocity -= 0.2;
    }
    if (mAvatarY <= 44) {
      m_jump_enable = 0;
    }
  }

  if (mAvatarY >= 48) {
    mAvatarY = 48;
    velocity = 0;
  }

  if (mAvatarY <= 8) {
    mAvatarY = 8;
    velocity += 0.2;
  }

  if (KEYA) {
    if ((!m_jump_enable) && (velocity == 0)) {
      velocity = -2;
      m_jump_enable = 1;
    }
  }

  if (KEYLEFT) {
    //check left side collision
    mAvaColX = (mAvatarX - 1) / 8 - 1;
    if (m_collision_boxes[mAvaColX + mAvatar_level * 14] & 1 << (7 - fixedy)) {
    } else {
      mAvatarX--;
    }

    if (mAvatarX <= 9) {
      mAvatarX = 9;
    }
  }


  if (KEYRIGHT) {
    //check collision to the right
    mAvaColX = mAvatarX / 8;

    if (m_collision_boxes[mAvaColX + mAvatar_level * 14] & 1 << (7 - fixedy)) {
    } else {
      mAvatarX++;
    }

    if (mAvatarX >= 110) {
      mAvatarX = 110;
    }
  }

}
