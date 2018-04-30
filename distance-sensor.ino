#include <Arduino.h>
#include <U8x8lib.h>
#include <NewPing.h>

#define TRIGGER_PIN  1  // 11 for uno, 1 for tiny
#define ECHO_PIN     1  // 11 for uno, 1 for tiny
#define MIN_DISTANCE 5
#define MAX_DISTANCE 250
#define PING_ITERATIONS 10

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

U8X8_SSD1306_128X32_UNIVISION_SW_I2C u8x8(2, 0, U8X8_PIN_NONE); // ATtiny85
// U8X8_SSD1306_128X32_UNIVISION_SW_I2C u8x8(SCL, SDA, U8X8_PIN_NONE); // UNO

void setup(void) {
  delay(2000);  // oled screen power up time
  u8x8.initDisplay();
  u8x8.clearDisplay();
  // u8x8.setFont(u8x8_font_amstrad_cpc_extended_r);
  // u8x8.setFont(u8x8_font_artosserif8_r);
  // u8x8.setFont(u8x8_font_artossans8_r);
  // u8x8.setFont(u8x8_font_torussansbold8_r);
  u8x8.setFont(u8x8_font_pressstart2p_f);
  
  u8x8.setPowerSave(0);
  u8x8.drawString(0, 3, "Gepsunom");
  delay(500);
}

char textBuffer[5];  // buffer to store microseconds and distance

bool heartBeat = true;

uint8_t centerLine[8] = {
  B0001000, B0001000, B0001000, B0001000, B0001000, B0001000, B0001000, B0001000
};
uint8_t crossTile[8] = {
  B0001000, B0001000, B0001000, B1111111, B0001000, B0001000, B0001000, B0001000
};
uint8_t startTile[8] = {
  B1111111, B0001000, B0001000, B0001000, B0001000, B0001000, B0001000, B0001000
};
uint8_t endTile[8] = {
  B0001000, B0001000, B0001000, B0001000, B0001000, B0001000, B0001000, B1111111
};


void loop(void) {
  
  int us = sonar.ping_median(PING_ITERATIONS);
  int cm = sonar.convert_cm(us);
  if (cm == 0) cm = MAX_DISTANCE;
  // TODO: fix sonar sensor out of range issue

  cm = constrain(cm, MIN_DISTANCE, MAX_DISTANCE);
  
  itoa(cm, textBuffer, DEC),
  u8x8.draw2x2String(0, 0, textBuffer);
  u8x8.draw2x2String(countDigits(cm) * 2, 0, " cm");
  int i = (countDigits(cm) * 2 + 6);
  for (i; i < 16; i = i + 2) {
    u8x8.draw2x2String(i, 0, " ");
  }

  drawLine16(2, map(cm, MIN_DISTANCE, MAX_DISTANCE, 0, 15));

  itoa(us, textBuffer, DEC);
  u8x8.drawString(0, 3, textBuffer);
  u8x8.drawString(countDigits(us), 3, " us");
  for (int j = (countDigits(us) + 3); j < 16; j++) {
    u8x8.drawString(j, 3, " ");
  }

  if (heartBeat) {
    heartBeat = false;
    u8x8.drawString(15, 3, ".");
  } else {
    heartBeat = true;
    u8x8.drawString(15, 3, " ");
  }

  u8x8.refreshDisplay();
  delay(100);
}

byte countDigits(int num) {
  byte count = 0;
  if (num == 0) return 1;
  while(num) {
    num = num / 10;
    count++;
  }
  return count;
}

void drawLine16(uint8_t row, uint8_t col) {
  if (row < 0) row = 0;
  if (row > 3) row = 3;
  for (int i=0; i < 16; i++) {

    if (i == col) {
      if (col == 0) u8x8.drawTile(i, 2, 1, startTile);
      else if (col == 15) u8x8.drawTile(i, 2, 1, endTile);
      else u8x8.drawTile(i, 2, 1, crossTile);
    } else {
      u8x8.drawTile(i, row, 1, centerLine);
    }

  }
}
