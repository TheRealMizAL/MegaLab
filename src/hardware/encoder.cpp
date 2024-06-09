#include "encoder.h"
#include "Arduino.h"


Encoder::Encoder(uint8_t left_pin, uint8_t right_pin, uint8_t button_pin,
                 bool enc_with_pullup, bool button_with_pullup,
                 bool enc_inversed, bool button_inversed) {
  this->_left_pin = new Pin(left_pin, enc_with_pullup ? 2 : 0);
  this->_right_pin = new Pin(right_pin, enc_with_pullup ? 2 : 0);
  this->button = new Button(button_pin, button_with_pullup ? 2 : 0, button_inversed);
}


void Encoder::tick() {
  this->button->tick();
  uint8_t state = this->_left_pin->digitalRead() | (this->_right_pin->digitalRead() << 1);
  if (state != this->_prev_state) {
    this->_pos_count += ((0x49941661 >> ((state | (this->_prev_state << 2)) << 1)) & 0b11) - 1; // don't even ask me wtf is this shit but it works
    this->_prev_state = state;
    
    if(this->_pos_count > -ENC_CLICKS_PER_MOVE && this->_pos_count < ENC_CLICKS_PER_MOVE) return;
    this->_flags.turned_left = this->_pos_count < 0;
    this->_flags.turned_right = this->_pos_count > 0;
    this->_pos_count = 0;
  }
}

bool Encoder::isLeftTurn() {
  if (!this->_flags.manual) this->tick();

  if (this->_flags.turned_left) {
    this->_flags.turned_left = false;
    return true;
  }
  return false;
}

bool Encoder::isRightTurn() {
  if (!this->_flags.manual) this->tick();
  
  if (this->_flags.turned_right) {
    this->_flags.turned_right = false;
    return true;
  }
  return false;
}

void Encoder::reset() {
  this->_flags.turned_left = false;
  this->_flags.turned_right = false;
}

Encoder* Encoder::setManual(bool manual) {
  this->_flags.manual = manual;
  return this;
}