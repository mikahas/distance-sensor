#include <Arduino.h>
#include <U8x8lib.h>
#include <NewPing.h>

#define TRIGGER_PIN  1
#define ECHO_PIN     1
#define MAX_DISTANCE 250
#define PING_ITERATIONS 10

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

U8X8_SSD1306_128X32_UNIVISION_SW_I2C u8x8(2, 0, U8X8_PIN_NONE);

void setup(void) {
  delay(2000);  // oled screen power up time
  u8x8.initDisplay();
  u8x8.clearDisplay();
  // u8x8.setFont(u8x8_font_amstrad_cpc_extended_r);
  // u8x8.setFont(u8x8_font_artosserif8_r);
  // u8x8.setFont(u8x8_font_artossans8_r);
  u8x8.setFont(u8x8_font_torussansbold8_r);
  u8x8.setPowerSave(0);
  u8x8.drawString(0, 3, "Mika Hassinen");
  delay(500);
}

char textBuffer[5];  // buffer to store microseconds and distance

bool heartBeat = true;

void loop(void) {
  
  int us = sonar.ping_median(PING_ITERATIONS);
  int cm = sonar.convert_cm(us);
  if (cm == 0) cm = MAX_DISTANCE;
  
  itoa(cm, textBuffer, DEC),
  u8x8.draw2x2String(0, 0, textBuffer);
  u8x8.draw2x2String(countDigits(cm) * 2, 0, " cm");
  int i = (countDigits(cm) * 2 + 6);
  for (i; i < 16; i = i + 2) {
    u8x8.draw2x2String(i, 0, i);
  }

  itoa(us, textBuffer, DEC);
  u8x8.drawString(0, 3, textBuffer);
  u8x8.drawString(countDigits(us), 3, " us");
  //for (int i = (countDigits(cm) + 3); i < 8) {
  //  u8x8.draw2x2String(i, 0, " ");
  //}

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
  while(num) {
    num = num / 10;
    count++;
  }
  return count;
}

