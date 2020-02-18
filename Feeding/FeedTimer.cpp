#include "FeedTimer.h"

FeedTimer::FeedTimer(DS1302 *ds1302) 
{
    // Запоминаем, сколько милисекунд прошло с момента старта ардуино
    startMS = millis();
    // Запоминаем ссылку на часы реального времени
    rtc = ds1302;
    // Столько времени по полсекунды уже прошло
    secCount = 0;
}

bool FeedTimer::isRedrawRequired()
{
    if (secCount == 0)
        return true;

    //Получаем, сколько милисекунд прошло с момента старта ардуино
    unsigned long currentMS = millis();
    
    unsigned long n = (currentMS - startMS) % 1000;

    // Если прошло более чем 1 секунда c момента последнего обновления
    if (n > secCount)
    {
        // запоминаем сколько секунд уже прошло
        secCount = n;
        return true;
    }
    return false;
}

void FeedTimer::print(LCD_1602_RUS *lcd)
{
    Time t = rtc->getTime(); // Получаем текущее время
    // Подготовим строки с текущим временем
    char hourTextBuffer[4] = "  ";
    char minTextBuffer[4] = "  ";
    sprintf(hourTextBuffer, "%02d", t.hour);    
    sprintf(minTextBuffer, "%02d", t.min); 
    // Формируем строку с текущим временем
    String timeStr = String(hourTextBuffer) + ":" + String(minTextBuffer);    
    lcd->setCursor(0, 0);
    lcd->print(timeStr);      
}

componentType FeedTimer::getType()
{
    return feedTimer;
}