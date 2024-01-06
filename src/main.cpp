#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_seesaw.h>
#include <MIDI.h>

#include "buttons.h"
#include "display.h"

void scene_ui_test(void);
void scene_midi_test(void);
void scene_clock_test(void);

void midi_clock_pulse(void);

// this more complicated setup is necessary to ensure that the midi library
// picks the right serial port
struct MIDISettings : public midi::DefaultSettings {
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
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // configure joywing
  if(!ss.begin(0x49)) {
    display.print("FATAL: error initializing seesaw");
    display.display();
    digitalWrite(LED_BUILTIN, HIGH);
    while(1) delay(10);
  } else {
    ss.pinModeBulk(button_mask, INPUT_PULLUP);
    ss.setGPIOInterrupts(button_mask, 1);
  }

  // configure MIDI
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleClock(midi_clock_pulse);
}

void loop() {
  MIDI.read();
  // scene_ui_test();
  // scene_midi_test();
  scene_clock_test();
}

uint32_t midi_pulses = 0;
void midi_clock_pulse() {
  if( midi_pulses++ % 24 == 0 ) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}

void scene_clock_test() {
  clear_screen();
  display.printf("%08d", midi_pulses);
  display.display();
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
