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

#include<LiquidCrystal.h>

int digital=17;
int analog=12;
int redled = 15;
int greenled = 2;
int gasvalue;
LiquidCrystal lcd(22, 23, 5, 18, 19, 21);


void setup() {
  Serial.begin(9600);
  pinMode(digital,INPUT);
  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);
  lcd.begin(16, 2);
  lcd.clear();
 
}

void loop() {
gasvalue = analogRead(analog);
gasvalue= ((gasvalue-500)/34);
  if(gasvalue <= 0) {
    gasvalue = 0;
  }
  else if(gasvalue > 100) {
    gasvalue = 100;
  }

  lcd.clear();
  lcd.print("Hydrogen:    %");
  lcd.setCursor(10,0);  
  lcd.print(gasvalue);
  delay(200);

  Serial.println(gasvalue);
 

  if(gasvalue < 90) {
  digitalWrite(redled, HIGH);
  digitalWrite(greenled, LOW);
  }
  else {
  digitalWrite(redled, LOW);
  digitalWrite(greenled, HIGH); 
  }

}
