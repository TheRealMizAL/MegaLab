#pragma once

#include <stdint.h>
#include "pins.h"
#include "../shared/ITickable.h"

class Thermistor : public ITickable
{
public:
    float temp = 0;
    float min_temp = -50.0;
    float max_temp = 260.0;
    float own_resistance = 100.0;
    float div_resistance = 10000.0;

    Thermistor(uint8_t pin);
    void setBorders(float min_temp, float max_temp);
    void setOwnResistance(float resistance);
    void setDivResistance(float resistance);
    virtual void tick();

private:
    AnalogPin* _pin;

    Thermistor() = delete;
    Thermistor(Thermistor* thermistor) = delete;
};