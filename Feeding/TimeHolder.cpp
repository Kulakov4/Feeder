#include "TimeHolder.h"

TimeHolder::TimeHolder(byte h, byte m,  byte s)
{
    hour = h;
    min = m;
    sec = s;
}

byte TimeHolder::getHour()
{
    return hour;
}

byte TimeHolder::getMin()
{
    return min;
}

// Получить секунды
byte TimeHolder::getSec()
{
    return sec;
}

// Сколько всего секунд (от начала суток)
uint32_t TimeHolder::getTotalSec()
{
    // Переводим всё в секунды
    return hour * 3600UL + min * 60UL + sec;
}