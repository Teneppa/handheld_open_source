long blink_millis = 0;
uint8_t blink_state = 0;

uint8_t blinkTextCenter(String str, uint16_t interval) {

if(millis()-blink_millis>interval) {
  blink_state = 1-blink_state;
  blink_millis = millis();
}

if(blink_state == 1) {
  uint8_t sz = 0;
  sz = str.length();
  m_oled.setCursor((128 - (sz * 6)) / 2, 30);
  m_oled.setTextColor(1);
  m_oled.println(str);
}

return(blink_state);
}

void drawTextCenter(String str) {
  uint8_t sz = str.length();
  m_oled.setCursor((128 - (sz * 6)) / 2, 30);
  m_oled.setTextColor(1);
  m_oled.println(str);
}


void scrollText(String str1, uint16_t stay_time, uint8_t delay_time) {
  uint8_t x = 0;
  uint8_t sz = 0;

  sz = str1.length();

  m_oled.setCursor(0, 0);

  while (x < 32) {
    m_oled.clearDisplay();
    m_oled.setCursor((128 - (sz * 6)) / 2, x);
    m_oled.setTextColor(1);
    m_oled.println(str1);
    x++;
    delay(delay_time);
    m_oled.display();
  }

  delay(stay_time);

  while (x < 65) {
    m_oled.clearDisplay();
    m_oled.setCursor((128 - (sz * 6)) / 2, x);
    m_oled.setTextColor(1);
    m_oled.println(str1);
    x++;
    delay(delay_time);
    m_oled.display();
  }
}
