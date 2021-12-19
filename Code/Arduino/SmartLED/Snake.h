class cSnake {
  private:
    CRGB *_leds;
    cLEDMatrix *_LEDMatrix;
    cLEDText *_LEDText;
    CRGB _MyColor;
    uint8_t SnakeLength;
    int8_t SpeedX, SpeedY;
    uint8_t SnakeX[51], SnakeY[51];
    uint8_t FoodX, FoodY;
    uint8_t LastCommand;
  public:
    cSnake(CRGB *_leds, cLEDMatrix *_LEDMatrix, cLEDText *_LEDText, CRGB _MyColor) {
      this ->_leds = _leds;
      this ->_MyColor = _MyColor;
      this ->_LEDMatrix = _LEDMatrix;
      this ->_LEDText = _LEDText;
      SpeedX = 1, SpeedY = 0;
      SnakeLength = 2;
      LastCommand = 2;
      FoodX = random((*_LEDMatrix).Width() - 1), FoodY = random((*_LEDMatrix).Height() - 1);
      SnakeX[0] = random((*_LEDMatrix).Width() - 1), SnakeY[0] = random((*_LEDMatrix).Height() - 1);
    }
    void Direction(uint8_t Command);
    void EatFood();
    void Death();
    void Update();
    void Show();
    void Run();
};

void cSnake::Direction(uint8_t Command) {
  if (Command != (3 - LastCommand)) {
    switch (Command) {
      case 0: SpeedX = 0, SpeedY = -1; break;
      case 3: SpeedX = 0, SpeedY = 1; break;
      case 1: SpeedX = -1, SpeedY = 0; break;
      case 2: SpeedX = 1, SpeedY = 0; break;
    }
    LastCommand = Command;
  }
}

void cSnake::EatFood() {
  if (SnakeX[0] == FoodX && SnakeY[0] == FoodY) {
    if (SnakeLength < 50)SnakeLength++;
    while (true) {
      uint8_t count = 0;
      FoodX = random((*_LEDMatrix).Width() - 1);
      FoodY = random((*_LEDMatrix).Height() - 1);
      for (uint8_t i = 0, j = 0; i < SnakeLength; i++) {
        if (FoodX == SnakeX[i] && FoodY == SnakeY[i])count++;
      }
      if (count == 0)break;
    }
  }
}

void cSnake::Death() {
  for (uint8_t i = 1; i < SnakeLength; i++) {
    if (SnakeX[0] == SnakeX[i] && SnakeY[0] == SnakeY[i]) {
      SnakeLength--;
      String message = "SCORE:" + (String)(SnakeLength);
      (*_LEDText).SetCursor((*_LEDMatrix).Width(), 5);
      for (uint8_t i = 0; i < message.length() + (*_LEDMatrix).Width() * 3; i++) {
        FastLED.clear();
        (*_LEDText).Print(message, _MyColor);
        (*_LEDText).ScrollLeft();
        FastLED.show();
        delay(100);
      }

      SnakeLength = 2;
      FoodX = random((*_LEDMatrix).Width() - 1), FoodY = random((*_LEDMatrix).Height() - 1);
      SnakeX[0] = random((*_LEDMatrix).Width() - 1), SnakeY[0] = random((*_LEDMatrix).Height() - 1);

      break;
    }
  }
}

void cSnake::Update() {
  SnakeX[0] = (SnakeX[0] + SpeedX) % ((*_LEDMatrix).Width());
  SnakeY[0] = (SnakeY[0] + SpeedY) % ((*_LEDMatrix).Height());
}

void cSnake::Show() {
  for (uint8_t i = SnakeLength; i > 0; i--) {
    SnakeX[i] = SnakeX[i - 1];
    SnakeY[i] = SnakeY[i - 1];
  }
  for (uint8_t i = 0; i < SnakeLength; i++) {
    _leds[(*_LEDMatrix).GetXY(SnakeX[i], SnakeY[i])] = CRGB::Green;
  }
  _leds[(*_LEDMatrix).GetXY(FoodX, FoodY)] = CRGB::Red;
}

void cSnake::Run() {
  Update();
  EatFood();
  Death();
  Show();
}
