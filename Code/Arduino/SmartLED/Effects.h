void BouncingBall() {
  static int8_t x = 10, y = 8;
  static int8_t DeltaX = -2, DeltaY = 1;
  const uint8_t Radius = 2;
  uint64_t Hue = millis() / 100;
  FastLED.clear();
  LEDMatrix.DrawFilledCircle(x, y, 2, CHSV(Hue + 64, 255, 255));
  LEDMatrix.DrawCircle(x, y, 1, CHSV(Hue + 32, 255, 255));
  LEDMatrix.DrawCircle(x, y, 0, CHSV(Hue, 255, 255));
  EVERY_N_MILLISECONDS(200) {
    x += DeltaX, y += DeltaY;
    if (x + Radius >= LEDMatrix.Width()) {
      DeltaX = -DeltaX;
      x = LEDMatrix.Width() - Radius - 1;
    }
    else if ((x - Radius) <= 0) {
      DeltaX = -DeltaX;
      x = Radius;
    }
    if (y + Radius >= LEDMatrix.Height()) {
      DeltaY = -DeltaY;
      y = LEDMatrix.Height() - Radius - 1;
    }
    else if ((y - Radius) <= 0) {
      DeltaY = -DeltaY;
      y = Radius;
    }
  }
  FastLED.show();
}

void Halo01() {
  static uint8_t ColorIndex;
  for (uint8_t i = 0; i < 8; i++)
    LEDMatrix.DrawRectangle(i, i, 15 - i, 15 - i, CHSV(ColorIndex + i * 20, 255, 255));
  ColorIndex = beatsin16(10, 0, 512, 0, 0) + beatsin16(20, 0, 512, 0, 0);
  FastLED.show();
}

void Halo02() {
  uint32_t ms = millis();
  int32_t yHueDelta32 = ((int32_t)cos16( ms * (27 / 1) ) * (350 / LEDMatrix.Width()));
  int32_t xHueDelta32 = ((int32_t)cos16( ms * (39 / 1) ) * (310 / LEDMatrix.Height()));
  uint8_t lineStartHue = ms / 65536;
  for ( uint8_t y = 0; y < LEDMatrix.Height(); y++) {
    lineStartHue += yHueDelta32 / 32768;
    uint8_t pixelHue = lineStartHue;
    for ( uint8_t x = 0; x < LEDMatrix.Width(); x++) {
      pixelHue += xHueDelta32 / 32768;
      leds[LEDMatrix.GetXY(x, y)]  = CHSV( pixelHue, 255, 255);
    }
  }
  FastLED.show();
}

void DisplaySprite() {
  static uint8_t ModeIndex;
  EVERY_N_MILLISECONDS(500) {
    LEDSprite.Show(Sprites[ModeIndex], 16, 16, 0, 0);
    FastLED.show();
    ModeIndex = (ModeIndex + 1) % 16;
  }
}

void SnakeGame() {
  EVERY_N_MILLISECONDS(300) {
    FastLED.clear();
    Snake.Run();
    FastLED.show();
  }
}

void Draw() {
  EVERY_N_MILLISECONDS(300) {
    if (DrawX < LEDMatrix.Width() && DrawY < LEDMatrix.Height()) {
      if (isShow)leds[LEDMatrix.GetXY(DrawX, DrawY)] = MyColor;
      else leds[LEDMatrix.GetXY(DrawX, DrawY)] = CRGB::Black;
    }
    FastLED.show();
  }
}

void DisplayTime() {
  static boolean flag;
  EVERY_N_MILLISECONDS(30000) {
    DateTime now = rtc.now();
    YEAR = now.year(), MONTH = now.month(), DATE = now.day(), HOUR = now.hour(), MINUTE = now.minute();
  }
  EVERY_N_MILLISECONDS(1000) {
    flag = !flag;
    String HourFormat, MinuteFormate;
    if (HOUR < 10)HourFormat += '0' + (String)HOUR;
    else HourFormat += (String)HOUR;
    if (MINUTE < 10)MinuteFormate += '0' + (String)MINUTE;
    else MinuteFormate += (String)MINUTE;
    FastLED.clear();
    LEDText.SetCursor(3, 0);
    LEDText.Print(HourFormat, MyColor);

    LEDText.SetCursor(5, LEDText.Height() - 1);
    if (flag)LEDText.Print(":", MyColor);

    LEDText.SetCursor(3, LEDText.Height() + 5);
    LEDText.Print(MinuteFormate, MyColor);
    FastLED.show();
  }
}

void BootingUp() {
  String message = "WELCOME MR.ADDICT!";
  LEDText.SetCursor(LEDMatrix.Width(), 5);
  for (uint8_t i = 0; i < message.length() + LEDMatrix.Width() * 4 + 8; i++) {
    FastLED.clear();
    LEDText.Print(message, MyColor);
    LEDText.ScrollLeft();
    FastLED.show();
    delay(100);
  }
}
