#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "src/hardware/encoder.h"
#include "src/hardware/pins.h"
#include "src/soft/dispatcher.h"
#include "src/hardware/thermistor.h"
#include "src/soft/menu.h"
#include "src/soft/pid_regulator.h"
#include "src/MegaLab.h"

int8_t inside_temp = 0;
int8_t outside_temp = 0;

LiquidCrystal_I2C lcd(0x27, 20, 4);
Encoder encoder(5, 6, 7, false, false, false, true);
Dispatcher dispatcher(3);
Thermistor thermistor_inisde(A0);
Thermistor thermistor_outside(A1);
PIDR pid_reg(10.0, 5.0, 3.0);

MenuNode root("root", 2);
ActionNode start("Start", start_work);

MenuNode settings("Settings", 4);
BackNode settings_back = *new BackNode();
IntValueNode temp_inside(&inside_temp, "T in ");
IntValueNode temp_outside(&outside_temp, "T out");

MenuNode pid_settings("PID settings", 4);
BackNode pid_back = *new BackNode();
FloatValueNode pid_kp(&pid_reg.Kp, "Kp");
FloatValueNode pid_kd(&pid_reg.Kd, "Kd");
FloatValueNode pid_ki(&pid_reg.Ki, "Ki");

Menu main_menu(&root);

Task enter_main_menu_check_task(enter_main_menu_check, "enter_main_menu_check", 50);
Task exit_main_menu_check_task(exit_main_menu_check, "enter_main_menu_check", 50);
Task menu_navigate_task(menu_navigate, "menu_navigate", 100);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.init();
  lcd.backlight();

  main_menu.addScreen(&lcd);
  root.addChild(&start);
  root.addChild(&settings);
  settings.addChild(&settings_back);
  settings.addChild(&temp_inside);
  settings.addChild(&temp_outside);
  settings.addChild(&pid_settings);
  pid_settings.addChild(&pid_back);
  pid_settings.addChild(&pid_kp);
  pid_settings.addChild(&pid_kd);
  pid_settings.addChild(&pid_ki);

  dispatcher.add_ticker(&encoder);
  dispatcher.add_ticker(&thermistor_inisde);
  dispatcher.add_ticker(&thermistor_outside);

  dispatcher.add(&enter_main_menu_check_task);
  dispatcher.add(&exit_main_menu_check_task);
  dispatcher.add(&menu_navigate_task);

  enter_main_menu_check_task.enable();

  main_screen();
}

void loop() {
  dispatcher.tick();
}

void start_work() {
  
}

void main_screen() {
  lcd.clear();
  lcd.setCursor(4, 1);
  lcd.print("Main Screen");
}

void enter_main_menu_check() {
  if(encoder.button->isPressed()) {
    enter_main_menu_check_task.disable();
    exit_main_menu_check_task.enable();
    menu_navigate_task.enable();
    main_menu.drawChilds();
    main_menu.last_update = millis();
  }
}

uint32_t current_millis;
void exit_main_menu_check() {
  if((current_millis = millis()) - main_menu.last_update > 10000) {
    main_screen();
    enter_main_menu_check_task.enable();
    exit_main_menu_check_task.disable();
    menu_navigate_task.disable();
  }
}

void menu_navigate() {
  if(encoder.isLeftTurn()) main_menu.cursorUp();
  else if(encoder.isRightTurn()) main_menu.cursorDown();
  else if(encoder.button->isPressed()) main_menu.enterNode();
}