#include <stdint.h>

#pragma once

class Pin {
protected:
  uint8_t _pin;
public:
  Pin(uint8_t pin);
  Pin(uint8_t pin, uint8_t mode);

  void setMode(uint8_t mode);
  void digitalWrite(bool state);
  bool digitalRead();
  void toggle();
};

class AnalogPin{
protected:
  uint8_t _pin;
public:
  AnalogPin(uint8_t pin);

  static void setReference(uint8_t reference);
  void analogWrite(uint8_t value);
  uint16_t analogRead();

public:
  static uint8_t reference;
};