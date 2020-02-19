#include "FeedTimer.h"

FeedTimer::FeedTimer(DS1302 *ds1302, TimeHolder **th, byte count) : TimeManager()
{
    // Запоминаем ссылку на часы реального времени
    rtc = ds1302;

    // Запоминаем, сколько милисекунд прошло с момента старта ардуино
    startMS = millis();

    // Столько секунд уже прошло
    secCount = 0;

    timeHolders = th;
    timeHolderCount = count;
	foodTime = false;
}

void FeedTimer::getTime()
{
    Time t = rtc->getTime(); // Получаем текущее время
    // Запоминаем часы, минуты и секунды
    hour = t.hour;
    min = t.min;
    sec = t.sec;
}

bool FeedTimer::isRedrawRequired()
{
    // Если включен режим редактирования времени
    if (isEditMode())
    {
        // Если перерисовывать не надо, но мы в режиме редактирования времени
        if (!redrawRequired)
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

    //Получаем, сколько милисекунд прошло с момента старта ардуино
    unsigned long currentMS = millis();

    unsigned long n = (currentMS - startMS) / 1000;

    // Если прошло более чем 1 секунда c момента последнего обновления
    if ((n > secCount) || (n == 0))
    {
        // запоминаем сколько секунд уже прошло
        secCount = n;
        return true;
    }
    return false;
}

TimeHolder FeedTimer::getNearestTime()
{
    uint32_t x;

    byte indexAbsoluteMin = 255;
    uint32_t absoluteMin = UINT32_MAX;

    byte indexRelativeMin = 255;
    uint32_t relativeMin = UINT32_MAX;

    // Получаем текущее время в секундах
    uint32_t totalSec = getTotalSec();

    // Цикл по всем временам кормления
    for (byte i = 0; i < timeHolderCount; i++)
    {
        // Получаем время кормления в секундах
        x = timeHolders[i]->getTotalSec();

        // Если время этого кормления меньше чем у предыдущих
        if (x < absoluteMin)
        {
            absoluteMin = x;
            indexAbsoluteMin = i;
        }

        // Если время кормления ещё не наступило и оно меньше чем предыдущие
        if ((x > totalSec) && (x < relativeMin))
        {
            relativeMin = x;
            indexRelativeMin = i;
        }
    }

    // Если нашли минимальное время кормления, которое ещё не наступило
    if (indexRelativeMin < 255)
    {
        // Получаем время кормления в секундах
        x = timeHolders[indexRelativeMin]->getTotalSec();
        // Вычитаем текущее время
        x -= totalSec;
    }
    else
    {
        // Получаем время кормления в секундах
        x = timeHolders[indexAbsoluteMin]->getTotalSec();
        // Добавляем время в секундах до конца суток
        x += (24UL * 3600UL - totalSec);
    }

    // Времени осталось больше часа
    if (x > 3600)
    {
        byte s = (x % 3600) % 60;        
        x += (60 - s); // Добавляем оставшиеся секунды        
    }
    // Времени осталось меньше часа но больше минуты
    if ((x >= 60) && (x <= 3600))
    {
        byte s = (x % 3600) % 60;
        // Если до конца минуты осталось больше 30 секунд
        if (s > 30)
            x += (60 - s); // Добавляем оставшиеся секунды
    }
	foodTime = (x <= 1);

    // Переводим время из секунд в часы, минуты и секунды
    byte h = x / 3600;
    byte m = (x % 3600) / 60;
    byte s = (x % 3600) % 60;

    return TimeHolder(h, m, s);
}

void FeedTimer::print(LCD_1602_RUS *lcd)
{
    String s = "                ";
    if (!isEditMode())
    {
        getTime();                       // Получаем текущее время
                                         /*
        s = "";
        uint32_t x = timeHolders[2]->getTotalSec();
        uint32_t y = getTotalSec();
        s = String(x) + " " + String(y);

*/
        TimeHolder t = getNearestTime(); // Получаем, сколько минут до ближайшего кормления
        s = "Осталось";
        if (t.getHour() > 0)
            s = s + " " + String(t.getHour()) + "ч. " + String(t.getMin()) + "м. ";
        else if (t.getMin() > 0)
            s = s + " " + String(t.getMin()) + " мин.  ";
        else
            s = s + " " + String(t.getSec()) + " cек.  ";
    }

    // Подготовим строки с текущим временем
    char hourTextBuffer[4] = "  ";
    char minTextBuffer[4] = "  ";

    // Если часы сейчас не должны мерцать
    if ((mode != editHourMode) || !blink)
        // Преобразуем часы в строку состоящую из 2-х символов
        sprintf(hourTextBuffer, "%02d", hour);

    // Если минуты сейчас не должны мерцать
    if ((mode != editMinMode) || !blink)
        // Преобразуем минуты в строку состоящую из 2-х символов
        sprintf(minTextBuffer, "%02d", min);

    // Формируем строку с текущим временем
    String timeStr = "Время " + String(hourTextBuffer) + ":" + String(minTextBuffer) + "      ";

    lcd->setCursor(0, 0);
    lcd->print(timeStr);
    lcd->setCursor(0, 1);
    lcd->print(s);
    redrawRequired = false;
}

componentType FeedTimer::getType()
{
    return feedTimer;
}

void FeedTimer::saveTime()
{
    // Запоминаем новое время
    rtc->setTime(hour, min, 0);
}

void FeedTimer::setMode(workMode new_mode)
{
    // Если изменения режима не произошло - ничего не делаем
    if (mode == new_mode)
        return;

    // Если время было изменено
    if (timeWasChanged)
    {
        saveTime(); // Сохраняем время
        timeWasChanged = false;
    }
    mode = new_mode;

    // Часы или минуты будут мерцать, если перешли в режим редактирования
    blink = isEditMode();

    if (blink)
    {
        startBlinkT = millis(); // Запомнили время начала мерцания
        getTime();              // Получаем текущее время
    }
    redrawRequired = true; // Запомнили что нужно перерисовать экран
}