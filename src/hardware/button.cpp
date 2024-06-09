#include "button.h"
#include "Arduino.h"

Button::Button(uint8_t pin, bool with_pullup, bool inversed) {
  this->_pin = new Pin(pin, with_pullup ? 2 : 0); 
  this->_flags.inversed = inversed;
  this->_flags.is_holded = false;
  this->_flags.is_pressed = false;
  this->_flags.is_released = false;
  this->_flags.prev_state = inversed;
}


void Button::tick() {
  bool state = this->_pin->digitalRead() ^ this->_flags.inversed;
  uint32_t this_check = millis();

  if (state != this->_flags.prev_state) {
    this->_last_check = this_check;
    this->_flags.prev_state = state;
    if (state) {
      this->_flags.is_pressed = true;
      this->_flags.is_released = false;
    }
    else {
      this->_flags.is_pressed = false;
      this->_flags.is_released = true;
      this->_flags.is_holded = false;
    }
  }
  
  if (this->_flags.prev_state && state && this_check - this->_last_check >= this->_hold_time) {
    this->_flags.is_holded = true;
  }
  if(this->_flags.is_released && this_check - this->_last_check >= this->_released_reset) {
    this->_flags.is_released = false;
  }
}


bool Button::isPressed() {
  if (this->_flags.is_pressed) {
    this->_flags.is_pressed = false;
    return true;
  }
  return false;
}

bool Button::isReleased() {
  if (this->_flags.is_released) {
    this->_flags.is_released = false;
    return true;
  }
  return false;
}

bool Button::isHolded() {
  return this->_flags.is_holded;
}