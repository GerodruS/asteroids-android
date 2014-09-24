#pragma once

#include <time.h>

class Timer
{
public:
    Timer();

    void setAlarm(float value);
    bool isReady();

private:
    float timeLeft_;
    clock_t timePrevious_;

    float getDeltaTime();
};