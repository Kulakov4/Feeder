#include <EEPROM.h>
#include "Feeding.h"

Feeding::Feeding(byte number): TimeManager () 
{
    n = number;
    hour = EEPROM.read(number * 2);
    min = EEPROM.read(number * 2 + 1);
};

bool Feeding::isRedrawRequired()
{
    // Если перерисовывать не надо, но мы в режиме редактирования времени
    if (!redrawRequired && isEditMode())
    {
        // Получаем текущее время
        unsigned long t = millis();
        // Если с момента начала мерцания прошло более 0.5 секунды
        if (t - startBlinkT > 500)
        {
            startBlinkT = t;       // запоминаем новое время начала мерцания
            blink = !blink;        // если мерцали, то перестанем и наоборот
            redrawRequired = true; // запоминаем, что надо перерисовать экран
        }
    }
    return redrawRequired;
}

void Feeding::print(LCD_1602_RUS *lcd)
{
    // Подготовим строки с временем кормления
    char hourTextBuffer[4] = "  ";
    char minTextBuffer[4] = "  ";

    // Если часы сейчас не должны мерцать
    if ((mode != editHourMode) || !blink)
        // Преобразуем часы кормления в строку состоящую из 2-х символов
        sprintf(hourTextBuffer, "%02d", hour);

    // Если минуты сейчас не должны мерцать
    if ((mode != editMinMode) || !blink)
        // Преобразуем минуты кормления в строку состоящую из 2-х символов
        sprintf(minTextBuffer, "%02d", min);

    // Формируем строку с временем кормления
    String timeStr = String(hourTextBuffer) + ":" + String(minTextBuffer) + "           ";

    String s = "Кормление " + String(n + 1);

    lcd->setCursor(0, 0);
    lcd->print(s);
    lcd->setCursor(0, 1);
    lcd->print(timeStr);
    redrawRequired = false;
}

void Feeding::saveTime()
{
    // Анализируем текущий режим
    switch (mode)
    {
    case editHourMode:
        // Сохраняем часы кормления в EEPROM
        EEPROM.put(n * 2, hour);
        break;
    case editMinMode:
        // Сохраняем минуты кормления в EEPROM
        EEPROM.put(n * 2 + 1, min);
        break;
    }
}

void Feeding::setMode(workMode new_mode)
{
    // Если изменения режима не произошло - ничего не делаем
    if (mode == new_mode)
        return;

    // Если время было изменено
    if (timeWasChanged)
    {
        saveTime();
        timeWasChanged = false;
    }
    mode = new_mode;

    // Часы или минуты будут мерцать, если перешли в режим редактирования
    blink = isEditMode();

    if (blink)
    {
        startBlinkT = millis(); // Запомнили время начала мерцания
    }
    redrawRequired = true; // Запомнили что нужно перерисовать экран
}

componentType Feeding::getType()
{
    return feeding;
}