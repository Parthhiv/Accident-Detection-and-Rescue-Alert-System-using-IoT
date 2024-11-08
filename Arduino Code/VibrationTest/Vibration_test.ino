#include <Arduino.h>
int vib_pin=34;
int val = 0;
void setup() {
  pinMode(vib_pin,INPUT);
  Serial.begin(9600);
}
void loop() {
  int curr = digitalRead(vib_pin);
  if(val>=1)
  {
    Serial.println("Accident");
    delay(500);
   }
   else{
    Serial.println("LOW");
    delay(500);
   }
   if(curr==0){
    val = 0;
  }else{
    val++;
  }
}
