const uint8_t ButtonPin1 = 33;
const uint8_t ButtonPin2 = 32;
const uint8_t ButtonPin3 = 19;
const uint8_t ButtonPin4 = 23;

const uint8_t LED_PIN = 18;
const uint8_t BRIGHTNESS = 10;
const uint8_t MATRIX_WIDTH = 16;
const uint8_t MATRIX_HEIGHT = 16;
const uint16_t NUM_LEDS = MATRIX_WIDTH * MATRIX_HEIGHT;

const char* ssid = "LED Matrix";
const char* password = "123456789";

uint8_t DrawX, DrawY;
boolean isShow;
boolean isDisplay;
boolean isAuto;
uint8_t MONTH, DATE, HOUR, MINUTE;
uint16_t YEAR;
uint8_t Mode;

CRGB MyColor = CRGB::Blue;

RTC_DS3231 rtc;
CRGB leds[NUM_LEDS + 1];
cLEDMatrix LEDMatrix(leds, VERTICAL_ZIGZAG_MATRIX, MATRIX_WIDTH, MATRIX_HEIGHT);
cLEDText LEDText(leds, &LEDMatrix);
cLEDSprite LEDSprite(leds, &LEDMatrix);
cSnake Snake(leds, &LEDMatrix, &LEDText, MyColor);
cButton Button1(ButtonPin1);
cButton Button2(ButtonPin2);
cButton Button3(ButtonPin3);
cButton Button4(ButtonPin4);

WebSocketsServer websocket(81);
