#ifndef F_TIMER_H
#define F_TIMER_H

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include "Component.h"
#include <DS1302.h>

class FeedTimer: public Component
{
private:
  DS1302 *rtc;
  unsigned long secCount;
  unsigned long startMS;
public:
    FeedTimer(DS1302 *ds1302);
    bool isRedrawRequired();
    void print(LCD_1602_RUS *lcd);
    componentType getType();    
};

#endif