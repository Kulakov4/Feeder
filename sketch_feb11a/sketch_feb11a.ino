#include <GyverButton.h>
#include <Stepper.h>
#include <EEPROM.h>
#include <Component.h>
#include <Feeding.h>
#include <FeedTimer.h>

#define FEED_AMOUNT 4 // Количество кормлений в сутки

GButton btnMenu(5);
GButton btnUp(6);
GButton btnDown(7);

//LiquidCrystal_I2C lcd(0x27, 16, 2);
LCD_1602_RUS lcd(0x27, 16, 2);   // Устанавливаем дисплей

const int stepsPerRevolution = 200;   // Количество шагов
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

DS1302 rtc(2, 3, 4);
Component c[] = {FeedTimer(&rtc), Feeding(0), Feeding(1), Feeding(2), Feeding(3) };
uint8_t ci = 0;

void setup() {
  // в ячейке 1023 должен быть записан флажок, если его нет - делаем (ПЕРВЫЙ ЗАПУСК)
  if (EEPROM.read(1023) != 5) {
    EEPROM.write(1023, 5);
    for (byte i = 0; i < FEED_AMOUNT; i++) {
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
  //  rtc.setTime(21, 46, 0);     // Set the time to 12:00:00 (24hr format)
  //  rtc.setDate(6, 8, 2010);   // Set the date to August 6th, 2010
}


void loop() {
  btnMenu.tick();
  if (btnMenu.isPress())
  {
    if (c[ci].getType() == feeding)
    {
      Feeding f = (Feeding)c[ci];
      
    }
  }
  // Если текущему компоненту надо перерисовать себя
  if (c[ci].isRedrawRequired())
    c[ci].print(&lcd);
}