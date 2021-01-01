#include <Arduino.h>

#include <SoftwareSerial.h>

char incoming_value = 0;

SoftwareSerial hc06(2, 3);

void setup() {
  Serial.begin(9600);
  Serial.println("ENTER AT Commands:");
  hc06.begin(9600);
  pinMode(10, OUTPUT);
}

void loop() {
  //Write data from HC06 to Serial Monitor
  if (hc06.available()) {

    incoming_value = hc06.read();
    Serial.write(incoming_value);
    
    if (incoming_value == 'a' && digitalRead(10) == LOW)
      digitalWrite(10, HIGH);
    else if (incoming_value == 'a' && digitalRead(10) == HIGH)
      digitalWrite(10, LOW);
  }

  //Write from Serial Monitor to HC06
  if (Serial.available()) {
    Serial.read();
    hc06.write(incoming_value);
  }
}

