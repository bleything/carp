#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_seesaw.h>
#include <MIDI.h>

#include "display.h"

void scene_ui_test();
void scene_midi_test();

#define BUTTON_DISPLAY_A 9
#define BUTTON_DISPLAY_B 8
#define BUTTON_DISPLAY_C 7

#define LED_PIN 13

// copypasta from the seesaw library's examples
#define BUTTON_JOY_A    6
#define BUTTON_JOY_B    7
#define BUTTON_JOY_Y    9
#define BUTTON_JOY_X   10
#define BUTTON_JOY_SEL 14
uint32_t button_mask = (1 << BUTTON_JOY_A) | (1 << BUTTON_JOY_B) |
                       (1 << BUTTON_JOY_Y) | (1 << BUTTON_JOY_X) |
                       (1 << BUTTON_JOY_SEL);

#define JOY_X 2
#define JOY_Y 3

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
Adafruit_seesaw ss;

struct MIDISettings : public midi::DefaultSettings
{
  static const long BaudRate = 31250;
};

MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial1, MIDI, MIDISettings);

void setup() {
  // configure the display
  delay(250); // wait for the OLED to power up
  display.begin(0x3C, true); // Address 0x3C default
  reset_screen();

  // configure display buttons
  pinMode(BUTTON_DISPLAY_A, INPUT_PULLUP);
  pinMode(BUTTON_DISPLAY_B, INPUT_PULLUP);
  pinMode(BUTTON_DISPLAY_C, INPUT_PULLUP);

  // configure the built-in LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // configure joywing
  if(!ss.begin(0x49)) {
    display.print("FATAL: error initializing seesaw");
    display.display();
    digitalWrite(LED_PIN, HIGH);
    while(1) delay(10);
  } else {
    ss.pinModeBulk(button_mask, INPUT_PULLUP);
    ss.setGPIOInterrupts(button_mask, 1);
  }

  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void clear_screen() {
  display.clearDisplay();
  display.setCursor(0, 0);
}

void reset_screen() {
  display.setRotation(1);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  clear_screen();
  display.display();
}

void loop() {
  // scene_ui_test();
  scene_midi_test();
}

void scene_midi_test() {
   if(digitalRead(BUTTON_DISPLAY_A) == LOW) {
    digitalWrite(LED_BUILTIN, HIGH);
    MIDI.sendNoteOn(42, 127, 1);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    MIDI.sendNoteOff(42, 0, 1);
  }
}

void scene_ui_test() {
  clear_screen();
  display.print(">");

  if(digitalRead(BUTTON_DISPLAY_A) == LOW) {
    display.print("A");
  } else {
    display.print(" ");
  }

  if(digitalRead(BUTTON_DISPLAY_B) == LOW) {
    display.print("B");
  } else {
    display.print(" ");
  }

  if(digitalRead(BUTTON_DISPLAY_C) == LOW) {
    display.print("C");
  } else {
    display.print(" ");
  }

  display.println("<");

  ///////////////////////

  display.print(">");

  uint32_t joy_buttons = ss.digitalReadBulk(button_mask);

  if(! (joy_buttons & (1 << BUTTON_JOY_A))) {
    display.print("A");
  } else {
    display.print(" ");
  }

  if(! (joy_buttons & (1 << BUTTON_JOY_B))) {
    display.print("B");
  } else {
    display.print(" ");
  }

  if(! (joy_buttons & (1 << BUTTON_JOY_Y))) {
    display.print("Y");
  } else {
    display.print(" ");
  }

  if(! (joy_buttons & (1 << BUTTON_JOY_X))) {
    display.print("X");
  } else {
    display.print(" ");
  }

  if(! (joy_buttons & (1 << BUTTON_JOY_SEL))) {
    display.print("Sel");
  } else {
    display.print("   ");
  }

  display.println("<");

  int x = ss.analogRead(JOY_X);
  int y = ss.analogRead(JOY_Y);

  display.print("(");
  display.print(x);
  display.print(",");
  display.print(y);
  display.print(")");

  display.display();
}
