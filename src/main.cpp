#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DS3231.h>
#include <Wire.h>
#include <string.h>

// Time variables
DS3231 Clock;

bool Century = false;
bool h12;
bool PM;
bool ADy, A12h, Apm;

byte ADay, AHour, AMinute, ASecond, ABits;
byte year, month, date, DoW, hour, minute, second;

// CONSTANTS
#define LED 10
#define firstAnalogPin 54

int moistureSensorsAmonut = 0;
float averageMoisture;

char incomingValue;
char outcomingValue;

String dataFromPhone;

SoftwareSerial hc06(2, 3); // 2 - Rx, 3 - Tx | Arduino Rx -> HC Tx # Arduino Tx -> Hc Rx by divider

// ################################ FUNCTIONS ################################

void readTime()
{
  int second, minute, hour, date, month, year, temperature;
  second = Clock.getSecond();
  minute = Clock.getMinute();
  hour = Clock.getHour(h12, PM);
  date = Clock.getDate();
  month = Clock.getMonth(Century);
  year = Clock.getYear();

  temperature = Clock.getTemperature();

  hc06.print("T");
  hc06.print("20");
  hc06.print(year, DEC);
  hc06.print('.');
  if (month < 10)
    hc06.print("0");
  hc06.print(month, DEC);
  hc06.print('.');
  if (date < 10)
    hc06.print("0");
  hc06.print(date, DEC);
  hc06.print(' ');
  if (hour < 10)
    hc06.print("0");
  hc06.print(hour, DEC);
  hc06.print(':');
  if (minute < 10)
    hc06.print("0");
  hc06.print(minute, DEC);
  hc06.print(':');
  if (second < 10)
    hc06.print("0");
  hc06.print(second, DEC);
  hc06.print('\n');
  hc06.print("Temperatura = ");
  if (temperature < 10)
    hc06.print("0");
  hc06.print(temperature);
  hc06.print("°C");
  hc06.print('\n');
}

void communicationTest()
{
  float rValue1 = rand() % 100;
  float rValue2 = rand() % 100;
  float rValue3 = rand() % 100;
  float rValue4 = rand() % 100;

  hc06.write("A01:");
  hc06.print(rValue1);

  delay(500);

  hc06.write("A02:");
  hc06.print(rValue2);

  delay(500);

  hc06.write("A03:");
  hc06.print(rValue3);

  delay(500);

  hc06.write("A04:");
  hc06.print(rValue4);

  delay(500);
}

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

void setTime(int second, int minute, int hour, int dayOfWeek, int day, int month, int year)
{
  Clock.setSecond(second);
  Clock.setMinute(minute);
  Clock.setHour(hour);
  Clock.setDoW(dayOfWeek);
  Clock.setDate(day);
  Clock.setMonth(month);
  Clock.setYear(year);
}

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  hc06.begin(9600);

  initAnalogs(moistureSensorsAmonut);
  pinMode(LED, OUTPUT); // simple LED
}

void loop()
{

  //Write data from HC06 to Serial Monitor
  if (hc06.available() > 0)
  {
    // incomingValue = hc06.read();
    // Serial.write(incomingValue);

    dataFromPhone = hc06.readString();
    Serial.print(dataFromPhone);
    if (dataFromPhone[0] == 'R' && dataFromPhone[1] == 'T' && dataFromPhone[2] == 'C')
    {
      hc06.print(dataFromPhone);

      char data[dataFromPhone.length()];
      for (size_t i = 0; i < sizeof(data); i++)
      {
        data[i] = dataFromPhone[i];
      }

      int timeData[7];
      int iterator = 0;
      char *token = strtok(data, ",");
      // loop through the string to extract all other tokens
      while (token != NULL)
      {
        if (iterator > 0)
        {
          timeData[iterator] = atoi(token);
        }
        token = strtok(NULL, ",");
        iterator++;
      }

      setTime(timeData[1], timeData[2], timeData[3], timeData[4], timeData[5], timeData[6], timeData[7]);
      readTime();
    }
    else if (dataFromPhone[0] == 'p')
    {
      if (digitalRead(LED) == HIGH)
      {
        digitalWrite(LED, LOW);
      }
      else
      {
        digitalWrite(LED, HIGH);
      }
    }
  }

  //Write from Serial Monitor to HC06
  if (Serial.available() > 0)
  {
    hc06.write(Serial.read());
  }
}