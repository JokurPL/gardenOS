#include <Arduino.h>
#include <SoftwareSerial.h>

// CONSTANTS
#define LED 10
#define firstAnalogPin 54

int moistureSensorsAmonut = 0;
float averageMoisture;

char incomingValue;
char outcomingValue;

SoftwareSerial hc06(2, 3); // 2 - Rx, 3 - Tx | Arduino Rx -> HC Tx # Arduino Tx -> Hc Rx by divider

// ################################ FUNCTIONS ################################

void initAnalogs(int moistureSensorsAmonut)
{
  if (moistureSensorsAmonut > 0)
  {
    int endIteration = firstAnalogPin + moistureSensorsAmonut;

    for (int i = firstAnalogPin; i < endIteration; i++)
    {
      pinMode(i, INPUT);
    }
  }
}

float toAverage(int moistureSensorsAmonut)
{
  float avg = 0;

  if (moistureSensorsAmonut > 0)
  {
    float sum;

    int endIteration = firstAnalogPin + moistureSensorsAmonut;

    for (int i = firstAnalogPin; i < endIteration; i++)
    {
      sum += analogRead(uint8_t(i));
    }

    avg = sum / moistureSensorsAmonut;
  }
  return avg;
}

void setup()
{
  Serial.begin(9600);
  hc06.begin(9600);

  Serial.println("ENTER AT Commands:");

  initAnalogs(moistureSensorsAmonut);

  hc06.write("AT");

  pinMode(LED, OUTPUT); // simple LED
}

void loop()
{

  //Write data from HC06 to Serial Monitor
  if (hc06.available() > 0)
  {
    incomingValue = hc06.read();
    Serial.write(incomingValue);
    switch (incomingValue)
    {
      // simple turn on/off LED
    case 'a':
      if (digitalRead(LED) == 1)
      {
        digitalWrite(LED, LOW);
        hc06.write("off");
      }
      else
      {
        digitalWrite(LED, HIGH);
        hc06.write("off");
      }

      break;

    default:
      break;
    }
  }

  //Write from Serial Monitor to HC06
  if (Serial.available() > 0)
  {
    hc06.write(Serial.read());
  }
}