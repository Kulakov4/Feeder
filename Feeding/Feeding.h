#ifndef FEEDING_H
#define FEEDING_H

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include "Component.h"

#define F_VIEW_MODE  1
#define F_EDIT_HOUR_MODE  2
#define F_EDIT_MIN_MODE 3

static const wchar_t *caption = L"Кормление X";

class Feeding: public Component
{
private:
    uint8_t n;
    bool timeWasChanged;
    bool redrawRequired;
    uint8_t hour;
    uint8_t min;
    bool blink;
    unsigned long startBlinkT;
    uint8_t mode;
    bool isEditMode();
    void saveHour();
    void saveMin();
public:
    Feeding(uint8_t number);
    void incHour();
    void decHour();
    void incMin();
    void decMin();
    void setMode(uint8_t new_mode);
    void toggleMode();
    bool isRedrawRequired();
    void print(LCD_1602_RUS *lcd);
    componentType getType();
};

#endif