#include "Clock.h"

Time::Time(LargeInt time) : time(time)
{
}

Time::LargeInt Time::asMicroseconds()
{
    return time;
}

Time::LargeInt Time::asMilliseconds()
{
    return time / 1000;
}

Time::LargeInt Time::asSeconds()
{
    return time / 1000000;
}

Time Time::Seconds(float seconds)
{
    return (LargeInt)(seconds * 1000000);
}

Time Time::Milliseconds(LargeInt milliseconds)
{
    return milliseconds * 1000;
}

Clock::Clock(bool start) : clockPaused(!start)
{
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&beginTime);
}

Time Clock::GetElapsedTime()
{
    if (!clockPaused)
    {
        QueryPerformanceCounter(&endTime);
        elapsedTime += ((endTime.QuadPart - beginTime.QuadPart) * 1000000) / frequency.QuadPart;
        QueryPerformanceCounter(&beginTime);
    }

    return Time(elapsedTime);
}

void Clock::Reset()
{
    elapsedTime = 0;
}

void Clock::Pause()
{
    clockPaused = true;
}

void Clock::Restart()
{
    if (clockPaused)
    {
        QueryPerformanceCounter(&beginTime);
        clockPaused = false;
    }
}

bool Clock::IsPaused()
{
    return clockPaused;
}
