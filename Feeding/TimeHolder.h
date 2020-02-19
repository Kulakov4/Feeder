#ifndef F_TIME_HOLDER_H
#define F_TIME_HOLDER_H

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class TimeHolder
{
protected:
    byte hour;
    byte min;
    byte sec;
public:
    // Конструктор
    TimeHolder(byte h, byte m, byte s);

    // Получить часы
    byte getHour();

    // Получить минуты
    byte getMin();

    // Получить секунды
    byte getSec();    

    // Сколько всего секунд (от начала суток)
    uint32_t getTotalSec();
};

#endif