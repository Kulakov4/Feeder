#include <GyverButton.h>
#include <Stepper.h>
#include <EEPROM.h>
#include <Component.h>
#include <Feeding.h>
#include <FeedTimer.h>

GButton btnMenu(5);
GButton btnInc(6);
GButton btnDec(7);

//LiquidCrystal_I2C lcd(0x27, 16, 2);
LCD_1602_RUS lcd(0x27, 16, 2);   // Устанавливаем дисплей

const int stepsPerRevolution = 200;   // Количество шагов
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

DS1302 rtc(2, 3, 4);

FeedTimer f = FeedTimer(&rtc);
Feeding f1 = Feeding(0);
Feeding f2 = Feeding(1);
Feeding f3 = Feeding(2);
Feeding f4 = Feeding(3);

Component* c[5];
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
  c[0] = &f;
  c[1] = &f1;
  c[2] = &f2;
  c[3] = &f3;
  c[4] = &f4;
  lcd.init();
  myStepper.setSpeed(60);             // Установка скорости 60 об/мин
  Serial.begin(9600);

  // Set the clock to run-mode, and disable the write protection
  rtc.halt(false);
  rtc.writeProtect(false);

  // The following lines can be commented out to use the values already stored in the DS1302
  //  rtc.setDOW(FRIDAY);        // Set Day-of-Week to FRIDAY
  //rtc.setTime(19, 30, 0);     // Set the time to 12:00:00 (24hr format)
  //  rtc.setDate(6, 8, 2010);   // Set the date to August 6th, 2010

  lcd.backlight();// Включаем подсветку дисплея
}


void loop() {
  btnInc.tick();
  if (btnInc.isPress())
  {
    Serial.println("btnInc.isPress");
    bool done = false;
    if (c[ci]->getType() == feeding)
    {
      Feeding *f = (Feeding *)(c[ci]);
      if (f->isEditMode()) {
        f->incTime();
        done = true;
      }
    }
    if (!done)
    {
      ci++;
      if (ci > 4)
        ci = 0;
//      lcd.clear();          
      c[ci]->print(&lcd);
    }
  }
  
  btnDec.tick();
  if (btnDec.isPress())
  {
    Serial.println("btnDec.isPress");
    bool done = false;
    if (c[ci]->getType() == feeding)
    {
      Feeding *f = (Feeding *)(c[ci]);
      if (f->isEditMode()) {
        f->decTime();
        done = true;
      }
    }
    if (!done)
    {
      if (ci == 0)
        ci = 4;
       else
         ci--;
  //    lcd.clear();         
      c[ci]->print(&lcd);
    }
  }  
  
  btnMenu.tick();
  if (btnMenu.isPress())
  {
    Serial.println("btnMenu.isPress");

    switch (c[ci]->getType())
    {
      case feeding:
        Serial.println("feeding");
        ((Feeding *)c[ci])->toggleMode();
        break;
    }
  }

  // Если текущему компоненту надо перерисовать себя
  if (c[ci]->isRedrawRequired()) 
  {
    Serial.println("RedrawRequired");
    c[ci]->print(&lcd);
  }
}
