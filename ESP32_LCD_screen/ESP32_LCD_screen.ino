#include<LiquidCrystal.h>
//VSS GRND
//VDD +
//V0 variable resistor (contrast)
//RS pin 22
//RW GRND
//Enable pin 23
//D4 pin 5
//D5 pin 18
//D6 pin 19
//D7 pin 21
//A 220 resistor (led)
//K led GRND

LiquidCrystal lcd(22, 23, 5, 18, 19, 21);


void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("2x16 LCD Display");
 
}

void loop() {
  lcd.setCursor(0,1);
  
  lcd.print("TEST");

}
