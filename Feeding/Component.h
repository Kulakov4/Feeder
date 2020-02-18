#ifndef F_COMPONENT_H
#define F_COMPONENT_H

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <LCD_1602_RUS.h>

enum componentType {component = 1, feedTimer, feeding};

class Component
{
public:
    virtual componentType getType();
    virtual bool isRedrawRequired();
    virtual void print(LCD_1602_RUS *lcd);
};

#endif