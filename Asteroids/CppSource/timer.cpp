#include "timer.h"


Timer::Timer() :
    timeLeft_(0.0f),
    timePrevious_(0.0f)
{
}


void Timer::setAlarm(const float value)
{
    timeLeft_ = value;
    timePrevious_ = clock();
}


bool Timer::isReady()
{
    if (0.0f < timeLeft_) {
        timeLeft_ -= getDeltaTime();
        return timeLeft_ <= 0.0f;
    }
    else {
        return true;
    }
}


void Timer::reset()
{
    timeLeft_ = 0.0f;
}


float Timer::getDeltaTime()
{
    const clock_t timeNow = clock();
    const float result = float(timeNow - timePrevious_) / float(CLOCKS_PER_SEC);
    
    timePrevious_ = timeNow;
    return result;
}