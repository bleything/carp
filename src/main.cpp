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
void scene_main(void);

void midi_clock_pulse(void);
void midi_start(void);
void midi_stop(void);
void midi_note_on(byte, byte, byte);
void midi_note_off(byte, byte, byte);

bool redraw_display = true;

bool seq_running = false;
uint32_t midi_pulses = 0;
uint8_t channel_out = 1;
uint8_t current_note, base_note = 60;
uint8_t current_step = 0;
uint8_t offsets [16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t velocities[16] = {80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80};

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
  MIDI.setHandleStart(midi_start);
  MIDI.setHandleStop(midi_stop);
}

void loop() {
  MIDI.read();

  if(redraw_display) {
    redraw_display = false;

    clear_screen();
    // scene_ui_test();
    // scene_midi_test();
    // scene_clock_test();
    scene_main();
    display.display();
  }
}

void midi_start() {
  seq_running = true;
  current_step = 0;

  redraw_display = true;
}

void midi_stop() {
  seq_running = false;

  for(uint8_t i = 0; i < 128; i++) {
    MIDI.sendNoteOff(i, 0, channel_out);
  }

  redraw_display = true;
}

void midi_note_on(byte channel, byte note, byte velocity) {
  base_note = note;
}

void midi_note_off(byte channel, byte note, byte velocity) {
  // no-op
}

void midi_clock_pulse() {
  int sixteenth = 6;
  int eighth = 2;

  if( seq_running && (++midi_pulses % sixteenth == 0)) {
    // sending the note off message throws the timing off by a lot and I'm going
    // to kick that can down the road by using a monosynth for testing and
    // forgetting about this for now.
    // MIDI.sendNoteOff(current_note, 0, channel_out);

    current_note = base_note + offsets[current_step];
    MIDI.sendNoteOn(current_note, velocities[current_step], channel_out);

    if(++current_step > 15) current_step = 0;

    redraw_display = true;
  }
}

void scene_main() {
  if(seq_running) {
    display.fillTriangle(0, 0, 0, 5, 5, 2, 1);
  } else {
    display.fillRect(0, 0, 5, 5, 1);
  }

  for(uint8_t i = 0; i < 16; i++) {
    int x = 2 + i * 8;
    display.fillRect(x, 29, 6, 6, 1);
  }

  // current step marker
  int x = 4 + current_step * 8;
  display.fillCircle(x, 42, 2, 1);
}

void scene_clock_test() {
  display.printf("%08d", midi_pulses);

  redraw_display = true;
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

  redraw_display = true;
}
