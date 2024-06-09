#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "src/hardware/encoder.h"
#include "src/hardware/pins.h"
#include "src/soft/dispatcher.h"
#include "src/hardware/thermistor.h"
#include "src/soft/menu.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);
Encoder encoder(5, 6, 7, false, false, false, true);
Dispatcher dispatcher(3);
Thermistor thermistor_inisde(A0);
Thermistor thermistor_outside(A1);

MenuNode root_menu("root_menu", 3);
MenuNode sub1("Sub 1", 0);
MenuNode sub2("Sub 2", 3);
BackNode back_sub2 = *new BackNode();
MenuNode sub2_1("Sub 1", 0);
MenuNode sub2_2("Sub 2", 0);

int8_t temp = 100;
IntValueNode temperature(&temp, "Temp.");
Menu menu(&root_menu);

void update_screen() {
  if (encoder.isRightTurn()) menu.cursorUp();
  else if (encoder.isLeftTurn()) menu.cursorDown();
  if (encoder.button->isPressed()) menu.enterNode();
}

Task update_screen_task(update_screen, "update_screen", 100);

void setup() {
  lcd.init();
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  Menu::addScreen(&lcd);
  root_menu.addChild(&sub1);
  root_menu.addChild(&sub2);
  root_menu.addChild(&temperature);
  sub2.addChild(&back_sub2);
  sub2.addChild(&sub2_1);
  sub2.addChild(&sub2_2);

  dispatcher.add_ticker(&encoder);
  dispatcher.add_ticker(&thermistor_inisde);
  dispatcher.add_ticker(&thermistor_outside);

  dispatcher.add(update_screen_task);
  dispatcher.enable_all();

  menu.drawChilds();
}


void loop(){
  dispatcher.tick();
}