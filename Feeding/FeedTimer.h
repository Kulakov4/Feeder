#ifndef F_TIMER_H
#define F_TIMER_H

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "TimeManager.h"
#include <DS1302.h>

class FeedTimer : public TimeManager
{
private:
  DS1302 *rtc;
  unsigned long secCount;
  unsigned long startMS;
  TimeHolder **timeHolders;
  byte timeHolderCount;
  void getTime();
  void saveTime();
  TimeHolder getNearestTime();
protected:
  void setMode(workMode new_mode);

public:
  FeedTimer(DS1302 *ds1302, TimeHolder **th, byte count);
  bool isRedrawRequired();
  void print(LCD_1602_RUS *lcd);
  componentType getType();
};

#endif