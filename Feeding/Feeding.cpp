#include <EEPROM.h>
#include "Feeding.h"

Feeding::Feeding(uint8_t number)
{
    n = number;
    hour = EEPROM.read(number * 2);
    min = EEPROM.read(number * 2 + 1);
    timeWasChanged = false;
    redrawRequired = false;
    mode = F_VIEW_MODE;
    blink = false;
};

void Feeding::incHour()
{
    hour++;
    if (hour > 23)
        hour = 0;
    timeWasChanged = true; // запоминаем, что время изменилось
    redrawRequired = true; // запоминаем, что надо перерисовать экран
}

void Feeding::decHour()
{
    if (hour == 0)
        hour = 23;
	else
		hour--;	
    timeWasChanged = true; // запоминаем, что время изменилось
    redrawRequired = true; // запоминаем, что надо перерисовать экран
}

void Feeding::incMin()
{
    min++;
    if (min > 59)
        min = 0;
    timeWasChanged = true; // запоминаем, что время изменилось
    redrawRequired = true; // запоминаем, что надо перерисовать экран
}

void Feeding::decMin()
{
    if (min == 0)
        min = 59;
	else
		min--;
    timeWasChanged = true; // запоминаем, что время изменилось
    redrawRequired = true; // запоминаем, что надо перерисовать экран
}

// Сохраняем часы кормления в EEPROM
void Feeding::saveHour()
{
    EEPROM.put(n * 2, hour);
}

// Сохраняем минуты кормления в EEPROM
void Feeding::saveMin()
{
    EEPROM.put(n * 2 + 1, min);
}

// Находимся ли мы в режиме редактирования часов или минут?
bool Feeding::isEditMode()
{
    return (mode == F_EDIT_HOUR_MODE) || (mode == F_EDIT_MIN_MODE);
}

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
	if ((mode != F_EDIT_HOUR_MODE) || (!blink))
        // Преобразуем часы кормления в строку состоящую из 2-х символов
        sprintf(hourTextBuffer, "%02d", hour);

    // Если минуты сейчас не должны мерцать
    if ((mode != F_EDIT_MIN_MODE) || (!blink))
        // Преобразуем минуты кормления в строку состоящую из 2-х символов
        sprintf(minTextBuffer, "%02d", min);

    // Формируем строку с временем кормления
    String timeStr = String(hourTextBuffer) + ":" + String(minTextBuffer) + "       ";

	String s = "Кормление " + String(n + 1);

    lcd->setCursor(0, 0);
    lcd->print(s);
    lcd->setCursor(0, 1);
    lcd->print(timeStr);
	redrawRequired = false;
}

void Feeding::setMode(uint8_t new_mode)
{
    // Если изменения режима не произошло - ничего не делаем
    if (mode == new_mode)
        return;

    // Если время кормления было изменено
    if (timeWasChanged)
    {
        // Анализируем текущий режим
        switch (mode)
        {
        case F_EDIT_HOUR_MODE:
            saveHour(); // Сохраняем часы кормления
            break;
        case F_EDIT_MIN_MODE:
            saveMin(); // Сохраняем минуты кормления
            break;
        }
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

void Feeding::toggleMode()
{
    switch (mode)
    {
    case F_VIEW_MODE:
        setMode(F_EDIT_HOUR_MODE);
        break;
    case F_EDIT_HOUR_MODE:
        setMode(F_EDIT_MIN_MODE);
        break;
    case F_EDIT_MIN_MODE:
        setMode(F_VIEW_MODE);
        break;        
    }
}

void Feeding::incTime()
{
    switch (mode)
    {
    case F_EDIT_HOUR_MODE:
        incHour();
        break;
    case F_EDIT_MIN_MODE:
        incMin();
        break;        
    }	
}

void Feeding::decTime()
{
    switch (mode)
    {
    case F_EDIT_HOUR_MODE:
        decHour();
        break;
    case F_EDIT_MIN_MODE:
        decMin();
        break;        
    }	
}