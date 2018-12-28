uint8_t settings_selection = 0;

uint8_t dimmer = 0;

#define SETTINGS_KEY_BACK KEYSTART
#define SETTINGS_KEY_LEFT KEYLEFT
#define SETTINGS_KEY_RIGHT KEYRIGHT

void settings_begin() {
  settings_selection = 0;
}

void settings() {
  taulukko(F("Display"), F("Ping Pong"), F("Space Invaders"), F("Mushroom"), F("Gunman"), F("Tanks"), settings_selection);
  handle_settings_keys();
  if (KEYSELECT) {
    switch (settings_selection) {
      //display
      case 1:
        {
          while (!SETTINGS_KEY_BACK) {
            if(KEYQ){break;}
            adjuster(0, 1, dimmer);
            if (SETTINGS_KEY_RIGHT) {
              dimmer = 1;
            }
            if (SETTINGS_KEY_LEFT) {
              dimmer = 0;
            }
            m_oled.dim(dimmer);
          }
        }
        break;

      //ping pong
      case 2:
        while (!SETTINGS_KEY_BACK) {
          if(KEYQ){break;}
          adjuster(20, 100, fps_pingpong);
          if (SETTINGS_KEY_RIGHT) {
            fps_pingpong += 1;
          }
          if (SETTINGS_KEY_LEFT) {
            fps_pingpong -= 1;
          }
          delay(10);

          if (fps_pingpong > 100) {
            fps_pingpong = 100;
          }

          if (fps_pingpong < 20) {
            fps_pingpong = 20;
          }
        }
        break;
    }
  }
}

uint8_t adjuster(uint8_t min, uint8_t max, uint8_t value_to_set) {
  m_oled.clearDisplay();
  m_oled.drawRect(0, 16, 128, 32, 1);
  m_oled.fillRect(0, 16, map(value_to_set, min, max, 0, 128), 32, 1);
  m_oled.setCursor(0, 0);
  m_oled.setTextColor(1);
  m_oled.setTextSize(1);
  m_oled.println(value_to_set);
  m_oled.display();
}

void handle_settings_keys() {
  if (KEYUP) {
    if (oldup == 0) {
      settings_selection--;
    }
    oldup = 1;
  } else {
    oldup = 0;
  }

  if (KEYDOWN) {
    if (olddown == 0) {
      settings_selection++;
    }
    olddown = 1;
  } else {
    olddown = 0;
  }
  if (settings_selection < 1) {
    settings_selection = 1;
  }
  if (settings_selection > 6) {
    settings_selection = 6;
  }
}
