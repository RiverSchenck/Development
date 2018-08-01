int digital=17;
int analog=12;
int gas_value;
void setup()
{

pinMode(digital,INPUT);
Serial.begin(9600);

}

void loop()
{
gasvalue = analogRead(analog);
gasvalue= ((gasvalue-500)/34);
  if(gasvalue <= 0) {
    gasvalue = 0;
  }
  else if(gasvalue > 100) {
    gasvalue = 100;
  }
Serial.print("Hydrogen: ");
Serial.print(gasvalue);
Serial.print("%");
Serial.println("");


}
