#pragma once

#include <time.h>


class Timer
{
public:
    Timer();

    void setAlarm(const float value);
    bool isReady();
    void reset();

private:
    float timeLeft_;
    clock_t timePrevious_;

    float getDeltaTime();
};