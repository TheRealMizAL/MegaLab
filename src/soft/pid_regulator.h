#pragma once
#include <stdint.h>

class PIDR{
public:
    float Kp = 0;
    float Ki = 0;
    float Kd = 0;
    float setpoint = 0;
    float integral = 0;
private:
    uint16_t _min_out = 0;
    uint16_t _max_out = 255;

    float _prev_err = 0.0;

public:
    PIDR(float Kp, float Ki, float Kd) {
        this->Kd = Kd;
        this->Ki = Ki;
        this->Kp = Kp;
    }

    uint16_t getResult(float input, float dt){
        dt /= 1000.0f; 
        float err = setpoint - input;
        integral += err * dt;
        float D = (err - _prev_err) / dt;
        _prev_err = err;
        return constrain(err*Kp + integral*Ki + D*Kd, _min_out, _max_out);
    }

    void setLimits(uint16_t min, uint16_t max) {
        this->_min_out = min;
        this->_max_out = max;
    }
};