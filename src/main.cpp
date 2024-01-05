#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include "display.h"

#define BUTTON_DISPLAY_A 9
#define BUTTON_DISPLAY_B 8
#define BUTTON_DISPLAY_C 7

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

void setup() {
  delay(250); // wait for the OLED to power up
  display.begin(0x3C, true); // Address 0x3C default
  display.setRotation(1);

  // clear the display
  display.clearDisplay();
  display.display();

  // configure display buttons
  pinMode(BUTTON_DISPLAY_A, INPUT_PULLUP);
  pinMode(BUTTON_DISPLAY_B, INPUT_PULLUP);
  pinMode(BUTTON_DISPLAY_C, INPUT_PULLUP);

  reset_screen();
}

void clear_screen() {
  display.clearDisplay();
  display.setCursor(0, 0);
}

void reset_screen() {
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  clear_screen();
  display.display();
}

void loop() {
  clear_screen();
  display.display();
}
