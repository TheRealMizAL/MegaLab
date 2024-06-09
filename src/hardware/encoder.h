#pragma once

#include <stdint.h>
#include "pins.h"
#include "button.h"
#include "../shared/ITickable.h"

#define ENC_CLICKS_PER_MOVE 2

#pragma pack(push, 1)
typedef struct {
  bool turned_left : 1;
  bool turned_right : 1;
  bool manual : 1;
} EncoderFlags;
#pragma pack(pop)

class Encoder : public ITickable {

public:
  Encoder(uint8_t left_pin, uint8_t right_pin, uint8_t button_pin,
          bool enc_with_pullup, bool button_with_pullup,
          bool enc_inversed, bool button_inversed);

  virtual void tick();
  void reset();

  Encoder* setManual(bool manual);

  bool isLeftTurn();
  bool isRightTurn();
  Button* button;

private:
  Pin* _left_pin;
  Pin* _right_pin;

  int8_t _prev_state : 2;
  int8_t _pos_count : 4;
  EncoderFlags _flags;
};