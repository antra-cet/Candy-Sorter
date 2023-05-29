//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
//  lcd.setCursor(3,0);
//  lcd.print("A");
//
//    lcd.setCursor(4,0);
//  lcd.print("N");
//
//    lcd.setCursor(5,0);
//  lcd.print("D");
//
//    lcd.setCursor(6,0);
//  lcd.print("R");
//
//    lcd.setCursor(7,0);
//  lcd.print("A");

  lcd.print("Hello");
}


void loop()
{
}
