#pragma once

#include <time.h>

class Timer
{
public:
    Timer();

    void setAlarm(float value);
    bool isReady();
    void reset()
    {
        timeLeft_ = 0.0f;
    };

private:
    float timeLeft_;
    clock_t timePrevious_;

    float getDeltaTime();
};