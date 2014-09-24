#include "timer.h"

Timer::Timer() :
    timeLeft_(0.0f),
    timePrevious_(0.0f)
{
}


void Timer::setAlarm(float value)
{
    timeLeft_ = value;
    timePrevious_ = clock();
}


bool Timer::isReady()
{
    if (0.0f < timeLeft_)
    {
        timeLeft_ -= getDeltaTime();
        return timeLeft_ <= 0.0f;
    }
    else
    {
        return true;
    }
}


float Timer::getDeltaTime()
{
    clock_t timeNow = clock();
    float result = float(timeNow - timePrevious_) / float(CLOCKS_PER_SEC);
    timePrevious_ = timeNow;

    return result;
}