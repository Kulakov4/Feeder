#ifndef FEEDING_H
#define FEEDING_H

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "TimeManager.h"

class Feeding : public TimeManager
{
private:
  byte n;
  void saveTime();  
protected:  
  void setMode(workMode new_mode);
public:
  Feeding(byte number);
  bool isRedrawRequired();
  void print(LCD_1602_RUS *lcd);
  componentType getType();
};

#endif