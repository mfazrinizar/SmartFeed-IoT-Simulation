#include "Display.h"
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

static Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

void initDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.display();
}

void showStatus(long level, String status) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.printf("Feed: %ld%%\n", level);
  display.setTextSize(1);
  display.setCursor(0, 32);
  display.print("Status: ");
  display.println(status);
  display.display();
}