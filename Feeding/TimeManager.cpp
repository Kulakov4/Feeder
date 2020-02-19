#include "TimeManager.h"

// Конструктор базового класса
TimeManager::TimeManager(): TimeHolder(0, 0, 0)
{
    timeWasChanged = false;
    redrawRequired = false;
    blink = false;
    mode = viewMode;
}

componentType TimeManager::getType()
{
    return component;
}

// Находимся ли мы в режиме редактирования часов или минут?
bool TimeManager::isEditMode()
{
    return (mode == editHourMode) || (mode == editMinMode);
}

// Переключение между режимами работы
void TimeManager::toggleMode()
{
    switch (mode)
    {
    case viewMode:
        setMode(editHourMode);
        break;
    case editHourMode:
        setMode(editMinMode);
        break;
    case editMinMode:
        setMode(viewMode);
        break;
    }
}

// Изменяет время
void TimeManager::changeTime(bool inc)
{
    switch (mode)
    {
    case editHourMode:
        if (inc)
        {
            hour++;
            if (hour > 23)
                hour = 0;
        }
        else
        {
            if (hour == 0)
                hour = 23;
            else
                hour--;
        }
        break;
    case editMinMode:
        if (inc)
        {
            min++;
            if (min > 59)
                min = 0;
        }
        else
        {
            if (min == 0)
                min = 59;
            else
                min--;
        }

        break;
    }
    startBlinkT = millis(); // запоминаем новое время начала мерцания
    blink = false;          // пока меняем время - не будем мерцать
    timeWasChanged = true;  // запоминаем, что время изменилось
    redrawRequired = true;  // запоминаем, что надо перерисовать экран
}

// Увеличить время
void TimeManager::incTime()
{
    changeTime(true);
}

// Уменьшить время
void TimeManager::decTime()
{
    changeTime(false);
}