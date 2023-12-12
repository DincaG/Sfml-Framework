#pragma once
#include <windows.h>

class Time
{
    using LargeInt = long long;
    LargeInt time;

public:
    Time(LargeInt time);
    LargeInt asMicroseconds();
    LargeInt asMilliseconds();
    LargeInt asSeconds();

    static Time Seconds(float seconds);
    static Time Milliseconds(LargeInt milliseconds);
};

class Clock
{
    LARGE_INTEGER beginTime{};
    LARGE_INTEGER endTime{};
    LARGE_INTEGER frequency{};
    long long elapsedTime{};
    bool clockPaused;

public:
    Clock(bool start = true);
    Time GetElapsedTime();
    void Reset();
    void Pause();
    void Restart();
    bool IsPaused();
};