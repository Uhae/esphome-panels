#include <Arduino.h>
#include "LGFX_MaTouchS3_Parallel_ILI9488.hpp"

static LGFX lcd;

static void draw_boot_screen() {
  lcd.fillScreen(TFT_BLACK);
  lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  lcd.setTextSize(2);
  lcd.setCursor(10, 10);
  lcd.println("MaTouch S3 Parallel");
  lcd.println("LovyanGFX bring-up");
  lcd.println();
  lcd.println("Touch: tap the screen");
}

void setup() {
  Serial.begin(115200);
  delay(200);

  Serial.println();
  Serial.println("BOOT: starting LovyanGFX...");

  lcd.init();
  lcd.setRotation(3);     // Landscape
  lcd.setBrightness(255); // Backlight on
  lcd.fillScreen(TFT_BLACK);
  draw_boot_screen();


  Serial.println("BOOT: lcd.init() done");
  draw_boot_screen();
}

void loop() {
  static uint32_t last = 0;

  uint16_t x, y;
  if (lcd.getTouch(&x, &y)) {
    Serial.printf("TOUCH: x=%u y=%u\n", x, y);
    lcd.fillCircle(x, y, 6, TFT_RED);
    delay(30);
  }

  if (millis() - last > 2000) {
    last = millis();
    Serial.println("HEARTBEAT: running");
  }
}
