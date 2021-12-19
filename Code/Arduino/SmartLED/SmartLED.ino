#include <FastLED.h>
#include <LEDMatrix.h>
#include <LEDText.h>
#include <LEDSprite.h>
#include <RTClib.h>

#include <WiFi.h>
#include <ArduinoJson.h>
#include <WebSocketsServer.h>

#include "Snake.h"
#include "Button.h"
#include "Header.h"
#include "Sprites.h"
#include "Effects.h"
#include "Websocket.h"

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  WiFi.softAP(ssid, password);
  Serial.print("Access Point:");
  Serial.println(WiFi.softAPIP());

  websocket.begin();
  websocket.onEvent(webSocketEvent);

  BootingUp();
  rtc.begin();
  if (rtc.lostPower())rtc.adjust(DateTime(2021, 10, 20, 12, 30, 0));
  DateTime now = rtc.now();
  YEAR = now.year(), MONTH = now.month(), DATE = now.day();
  HOUR = now.hour(), MINUTE = now.minute();
  FastLED.clear();
}

void loop() {
  if (isDisplay) {
    switch (Mode) {
      case 0: DisplayTime(); break;
      case 1: BouncingBall(); break;
      case 2: Halo01(); break;
      case 3: Halo02(); break;
      case 4: DisplaySprite(); break;
      case 5: SnakeGame(); break;
      case 6: Draw(); break;
    }
  }
  else FastLED.clear();
  websocket.loop();
  if (Button1.isPressed())isDisplay = !isDisplay;
  if (isDisplay) {
    if (Button2.isPressed())isAuto = !isAuto;
    else if (Button3.isPressed()) {
      if (Mode > 0)Mode--;
      else Mode = 4;
    }
    else if (Button4.isPressed())Mode = (Mode + 1) % 5;
    if (isAuto) {
      EVERY_N_SECONDS(30) {
        Mode = (Mode + 1) % 5;
      }
    }
  }
}
