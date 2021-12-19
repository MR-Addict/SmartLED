class cButton {
  private:
    uint8_t _ButtonPin;
  public:
    cButton(uint8_t ButtonPin) {
      _ButtonPin = ButtonPin;
      pinMode(_ButtonPin, INPUT_PULLUP);
    }
    boolean isPressed();
};

boolean cButton::isPressed() {
  if (!digitalRead(_ButtonPin)) {
    delay(10);
    while (!digitalRead(_ButtonPin));
    return true;
  }
  else return false;
}
