#include <Adafruit_SH110X.h>

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

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
