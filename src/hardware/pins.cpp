#include "pins.h"
#include "avr/io.h"
#include "Arduino.h"

#ifndef bitWrite
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? (value) |= _BV(bit) : (value) &= ~_BV(bit))
#endif

Pin::Pin(uint8_t pin): _pin{pin} {}

Pin::Pin(uint8_t pin, uint8_t mode): _pin{pin} {
  this->setMode(mode);
}

void Pin::setMode(uint8_t mode) {
  switch (mode) {
    case 0:  // INPUT
      if (this->_pin < 8) {
        DDRD &= ~_BV(this->_pin);
        PORTD &= ~_BV(this->_pin);
      } else if (this->_pin < 14) {
        DDRB &= ~_BV(this->_pin - 8);
        PORTB &= ~_BV(this->_pin - 8);
      } else if (this->_pin < 20) {
        DDRC &= ~_BV(this->_pin - 14);
        PORTC &= ~_BV(this->_pin - 14);
      }
      return;
    case 1:  // OUTPUT
      if (this->_pin < 8) {
        DDRD |= _BV(this->_pin);
        PORTD &= ~_BV(this->_pin);
      } else if (this->_pin < 14) {
        DDRB |= _BV(this->_pin - 8);
        PORTB &= ~_BV(this->_pin - 8);
      } else if (this->_pin < 20) {
        DDRC |= _BV(this->_pin - 14);
        PORTC &= ~_BV(this->_pin - 14);
      }
      return;
    case 2:  // INPUT_PULLUP
      if (this->_pin < 8) {
        DDRD &= ~_BV(this->_pin);
        PORTD |= _BV(this->_pin);
      } else if (this->_pin < 14) {
        DDRB &= ~_BV(this->_pin - 8);
        PORTB |= _BV(this->_pin - 8);
      } else if (this->_pin < 20) {
        DDRC &= ~_BV(this->_pin - 14);
        PORTC |= _BV(this->_pin - 14);
      }
      return;
  }
}

bool Pin::digitalRead() {
  if (this->_pin < 8) {
    return (PIND >> this->_pin) & 0x01;
  } else if (this->_pin < 14) {
    return (PINB >> (this->_pin - 8)) & 0x01;
  } else if (this->_pin < 20) {
    return (PINC >> (this->_pin - 14)) & 0x01;
  }
}

void Pin::digitalWrite(bool state) {
  if (this->_pin < 8) {
    bitWrite(PORTD, this->_pin, state);
  } else if (this->_pin < 14) {
    bitWrite(PORTB, (this->_pin - 8), state);
  } else if (this->_pin < 20) {
    bitWrite(PORTC, (this->_pin - 14), state);
  }
}

void Pin::toggle() {
  if (this->_pin < 8) {
    PIND |= this->_pin;
    PIND |= _BV(this->_pin);
  } else if (this->_pin < 14) {
    PINB |= _BV(this->_pin - 8);
  } else if (this->_pin < 20) {
    PINC |= _BV(this->_pin - 16);
  }
}

uint8_t AnalogPin::reference;

AnalogPin::AnalogPin(uint8_t pin) {
  this->_pin = ((pin >= 14) ? (pin) - 14 : pin);
}

static void AnalogPin::setReference(uint8_t reference) {
    AnalogPin::reference = reference;
}

void AnalogPin::analogWrite(uint8_t value) {
  switch (this->_pin) {
    case 5:
      TCCR0A |= _BV(COM0B1);
      OCR0B = value;          
      return;
    case 6:
      TCCR0A |= _BV(COM0A1);
      OCR0A = value;
      return;
    case 10:
      TCCR1A |= _BV(COM1B1);
      OCR1B = value;
      return;
    case 9:
      TCCR1A |= _BV(COM1A1);
      OCR1A = value;
      return;
    case 3:
      TCCR2A |= _BV(COM2B1);
      OCR2B = value;
      return;
    case 11:
      TCCR2A |= _BV(COM2A1);
      OCR2A = value;
      return;
  }
}

uint16_t AnalogPin::analogRead() {
  ADMUX = (AnalogPin::reference << 6) | (this->_pin & 0x07);
  ADCSRA |= _BV(ADSC);
  while(ADCSRA & _BV(ADSC));
  return ADC;
}