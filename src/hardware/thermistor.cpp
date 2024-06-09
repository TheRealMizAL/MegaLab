#include "thermistor.h"
#include "Arduino.h"

Thermistor::Thermistor(uint8_t pin) {
    _pin = new AnalogPin(pin);
}

void Thermistor::setBorders(float min_temp, float max_temp) {
    this->min_temp = min_temp;
    this->max_temp = max_temp;
}

void Thermistor::setOwnResistance(float resistance) {
    own_resistance = resistance;
}

void Thermistor::setDivResistance(float resistance) {
    div_resistance = resistance;
}

void Thermistor::tick() {
    float read_val = _pin->analogRead();
    float mes_res = div_resistance / (1023.0 / read_val - 1);
    float reverse_temp = 1 / 298.15 + log(mes_res / own_resistance) / 3950;

    temp = 1 / reverse_temp;
}
