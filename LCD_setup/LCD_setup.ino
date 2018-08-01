#include<LiquidCrystal.h>
//VSS GRND
//VDD +
//V0 variable resistor (contrast)
//RS pin 12
//RW GRND
//Enable pin 11
//D4 pin 2
//D5 pin 3
//D6 pin 4
//D7 pin 5
//A 220 resistor (led)
//K led GRND

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


void setup() {
  lcd.begin(16, 2);

  lcd.print("2x16 LCD Display");
  lcd.clear();
}

void loop() {
  lcd.setCursor(10,1);
  
  lcd.print("TEST");

}
