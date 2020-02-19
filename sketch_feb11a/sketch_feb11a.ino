#include <GyverButton.h>
#include <Stepper.h>
#include <EEPROM.h>
#include <TimeManager.h>
#include <Feeding.h>
#include <FeedTimer.h>

GButton btnMenu(5);
GButton btnInc(6);
GButton btnDec(7);

LCD_1602_RUS lcd(0x27, 16, 2);   // Устанавливаем дисплей

const int stepsPerRevolution = 200;   // Количество шагов
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

DS1302 rtc(2, 3, 4);

// Создаём массив из 4-х объектов-управляющих временем кормления.
Feeding f[4] = {Feeding(0), Feeding(1), Feeding(2), Feeding(3)};

// Адреса этих объектов записываем в массив
TimeHolder* th[4] = {&f[0], &f[1], &f[2], &f[3]};

// Создаём таймер кормления
FeedTimer ft = FeedTimer(&rtc, th, 4);

TimeManager* c[5] = {&ft, &f[0], &f[1], &f[2], &f[3]};
byte ci = 0;

void setup() {
  // в ячейке 1023 должен быть записан флажок, если его нет - делаем (ПЕРВЫЙ ЗАПУСК)
  if (EEPROM.read(1023) != 5) {
    EEPROM.write(1023, 5);
    for (byte i = 0; i < 4; i++) {
      EEPROM.write(i * 2, 0);
      EEPROM.write(i * 2 + 1, 0);
    }
  }
  lcd.init();
  myStepper.setSpeed(60);             // Установка скорости 60 об/мин
  Serial.begin(9600);

  // Set the clock to run-mode, and disable the write protection
  rtc.halt(false);
  rtc.writeProtect(false);

  // The following lines can be commented out to use the values already stored in the DS1302
  //  rtc.setDOW(FRIDAY);        // Set Day-of-Week to FRIDAY
  // rtc.setTime(8, 37, 0);     // Set the time to 12:00:00 (24hr format)
  //  rtc.setDate(6, 8, 2010);   // Set the date to August 6th, 2010

  lcd.clear();
  lcd.backlight();// Включаем подсветку дисплея
}
void btnIncTick()
{
  btnInc.tick();
  if ( !( btnInc.isPress() || btnInc.isStep() ) )
    return;

  Serial.println("btnInc.isClick");

  // Если текущий компонент сейчас в режиме изменения времени
  if (c[ci]->isEditMode()) {
    // Увеличиваем в нём время
    c[ci]->incTime();
  }
  else  // Иначе переключаемся на следующий компонент
  {
    ci++;
    if (ci > 4)
      ci = 0;
    c[ci]->print(&lcd);
  }
}

void btnDecTick()
{
  btnDec.tick();
  if ( !( btnDec.isPress() || btnDec.isStep() ) )
    return;

  Serial.println("btnDec.isClick");

  // Если текущий компонент сейчас в режиме изменения времени
  if (c[ci]->isEditMode()) {
    // Уменьшаем в нём время
    c[ci]->decTime();
  }
  else  // Иначе переключаемся на предыдущий компонент
  {
    if (ci == 0)
      ci = 4;
    else
      ci--;
    c[ci]->print(&lcd);
  }
}

void btnMenuTick()
{
  btnMenu.tick();
  if (!btnMenu.isPress())
    return;

  Serial.println("btnMenu.isPress");
  // Переключаем текущий компонент в другой режим
  c[ci]->toggleMode();
}

void loop() {
  /*
  byte h = 23;
  byte m = 59;
  byte s = 59;
  uint32_t x = h * 3600UL + m * 60UL + s;
  lcd.setCursor(0, 0);
  lcd.print(String(x));
  return;
  */
  btnIncTick();
  btnDecTick();
  btnMenuTick();

  // Если текущему компоненту надо перерисовать себя
  if (c[ci]->isRedrawRequired())
  {
    c[ci]->print(&lcd);
  }
}
