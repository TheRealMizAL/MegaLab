#include <stdint.h>
#include "pins.h"
#include "../shared/ITickable.h"

#pragma pack(push, 1)
typedef struct {
  bool prev_state : 1;
  bool is_released : 1;
  bool is_pressed : 1;
  bool is_holded : 1;
  bool inversed : 1;
} ButtonFlags;
#pragma pack(pop)

class Button : public ITickable {
private:
  ButtonFlags _flags;

  Pin* _pin;
  uint32_t _last_check;
  uint8_t _released_reset = 100;
  uint8_t _hold_time = 150;
  uint8_t _gap_time = 500;  // time after we should reset _press_counter after last click

  uint8_t _press_counter = 0;

public:
  Button(uint8_t pin, bool with_pullup, bool inverted);

  bool isPressed();
  bool isReleased();
  bool isHolded();
  uint8_t timesPressed();

  void tick();
};
