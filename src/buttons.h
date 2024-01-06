#include <Arduino.h>
#include <Adafruit_seesaw.h>

#define BUTTON_DISPLAY_A 9
#define BUTTON_DISPLAY_B 8
#define BUTTON_DISPLAY_C 7

// copypasta from the seesaw library's examples
#define BUTTON_JOY_A    6
#define BUTTON_JOY_B    7
#define BUTTON_JOY_X   10
#define BUTTON_JOY_Y    9
#define BUTTON_JOY_SEL 14
uint32_t button_mask = (1 << BUTTON_JOY_A) | (1 << BUTTON_JOY_B) |
                       (1 << BUTTON_JOY_X) | (1 << BUTTON_JOY_Y) |
                       (1 << BUTTON_JOY_SEL);

#define JOY_X 2
#define JOY_Y 3

Adafruit_seesaw ss;
