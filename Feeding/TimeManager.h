#ifndef F_TIME_MANAGER_H
#define F_TIME_MANAGER_H

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "TimeHolder.h"
#include <LCD_1602_RUS.h>

enum workMode
{
  viewMode,
  editHourMode,
  editMinMode
};

enum componentType
{
  component = 1,
  feedTimer,
  feeding
};

class TimeManager: public TimeHolder
{
private:
  void changeTime(bool inc);
protected:
  bool timeWasChanged;
  bool redrawRequired;
  bool blink;
  unsigned long startBlinkT;
  workMode mode;
  virtual void setMode(workMode new_mode) = 0;
public:
  TimeManager();
  virtual componentType getType();
  // Требуется ли обновить сообщения на экране?
  virtual bool isRedrawRequired() = 0;

  // Вывести сообщения на экран
  virtual void print(LCD_1602_RUS *lcd) = 0;

  // Находимся ли в режиме редактирования времени
  virtual bool isEditMode();

  // Переключить режим
  virtual void toggleMode();

  // Увеличить время
  virtual void incTime();

  // Уменьшить время
  virtual void decTime();
};

#endif