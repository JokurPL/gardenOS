#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DS3231.h>
#include <Wire.h>
#include <string.h>
#include <EEPROM.h> // ???

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

// ####### ALARM CONSTANTS #######

#define ALARM_SECOND_EEPROM 0
#define ALARM_MINUTE_EEPROM 1
#define ALARM_HOUR_EEPROM 2
#define ALARM_DAY_EEPROM 3
#define ALARM_MONTH_EEPROM 4
#define ALARM_YEAR_EEPROM 5

#define STOP_ALARM_SECOND_EEPROM 6
#define STOP_ALARM_MINUTE_EEPROM 7
#define STOP_ALARM_HOUR_EEPROM 8
#define STOP_ALARM_DAY_EEPROM 9
#define STOP_ALARM_MONTH_EEPROM 10
#define STOP_ALARM_YEAR_EEPROM 11

// ####### SETTINGS CONSTANTS #######

#define SENSORS_AMOUNT_EEPROM 12
#define IRRIGATIOM_MODE_EEPROM 13

int moistureSensorsAmonut = 0;
float averageMoisture;

char incomingValue;
char outcomingValue;

String dataFromPhone;

SoftwareSerial hc06(2, 3); // 2 - Rx, 3 - Tx | Arduino Rx -> HC Tx # Arduino Tx -> HC Rx by divider

// ################################ FUNCTIONS ################################

bool isAutomode() {
  int irrigationMode = EEPROM.read(IRRIGATIOM_MODE_EEPROM);

  if (irrigationMode == 1)
  {
    return true;
  }
  return false;
}

bool isFirstAlarm()
{
  int secondEEPROM, minuteEEPROM, hourEEPROM, dayEEPROM, monthEEPROM, yearEEPROM, second, minute, hour, day, month, year;

  second = Clock.getSecond();
  minute = Clock.getMinute();
  hour = Clock.getHour(h12, PM);
  day = Clock.getDate();
  month = Clock.getMonth(Century);
  year = Clock.getYear();

  secondEEPROM = EEPROM.read(ALARM_SECOND_EEPROM);
  minuteEEPROM = EEPROM.read(ALARM_MINUTE_EEPROM);
  hourEEPROM = EEPROM.read(ALARM_HOUR_EEPROM);
  dayEEPROM = EEPROM.read(ALARM_DAY_EEPROM);
  monthEEPROM = EEPROM.read(ALARM_MONTH_EEPROM);
  yearEEPROM = EEPROM.read(ALARM_YEAR_EEPROM);

  //second == secondEEPROM &&

  if (minute == minuteEEPROM && hour == hourEEPROM && day == dayEEPROM && month == monthEEPROM && year == yearEEPROM)
  {
    return true;
  }
  return false;
}

bool isStopFirstAlarm()
{
  int secondEEPROM, minuteEEPROM, hourEEPROM, dayEEPROM, monthEEPROM, yearEEPROM, second, minute, hour, day, month, year;

  second = Clock.getSecond();
  minute = Clock.getMinute();
  hour = Clock.getHour(h12, PM);
  day = Clock.getDate();
  month = Clock.getMonth(Century);
  year = Clock.getYear();

  secondEEPROM = EEPROM.read(STOP_ALARM_SECOND_EEPROM);
  minuteEEPROM = EEPROM.read(STOP_ALARM_MINUTE_EEPROM);
  hourEEPROM = EEPROM.read(STOP_ALARM_HOUR_EEPROM);
  dayEEPROM = EEPROM.read(STOP_ALARM_DAY_EEPROM);
  monthEEPROM = EEPROM.read(STOP_ALARM_MONTH_EEPROM);
  yearEEPROM = EEPROM.read(STOP_ALARM_YEAR_EEPROM);

  //second == secondEEPROM &&

  if (minute == minuteEEPROM && hour == hourEEPROM && day == dayEEPROM && month == monthEEPROM && year == yearEEPROM)
  {
    return true;
  }
  return false;
}

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

int *readDataFromBT(String dataFromPhone)
{
  static int dataArray[7];

  char data[dataFromPhone.length()];
  for (size_t i = 0; i < sizeof(data); i++)
  {
    data[i] = dataFromPhone[i];
  }

  int iterator = 0;
  char *token = strtok(data, ",");

  while (token != NULL)
  {
    if (iterator > 0)
    {
      dataArray[iterator] = atoi(token);
    }
    token = strtok(NULL, ",");
    iterator++;
  }

  return dataArray;
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

  if (hc06.available() > 0)
  {
    // incomingValue = hc06.read();
    // Serial.write(incomingValue);
    dataFromPhone = hc06.readString();
    Serial.print(dataFromPhone);
    if (dataFromPhone[0] == 'R' && dataFromPhone[1] == 'T' && dataFromPhone[2] == 'C')
    {
      hc06.print(dataFromPhone);

      int *timeFromBT;
      timeFromBT = readDataFromBT(dataFromPhone);

      int timeData[7];

      for (int i = 1; i < 8; i++)
      {
        timeData[i] = *(timeFromBT + i);
      }

      setTime(timeData[1], timeData[2], timeData[3], timeData[4], timeData[5], timeData[6], timeData[7]);
      readTime();
    }
    else if (dataFromPhone[0] == 'A' && dataFromPhone[1] == 'T' && dataFromPhone[2] == 'S')
    {
      int *timeFromBT;
      timeFromBT = readDataFromBT(dataFromPhone);

      int timeData[7];
      for (int i = 1; i < 8; i++)
      {
        timeData[i] = *(timeFromBT + i);
      }

      EEPROM.write(ALARM_SECOND_EEPROM, timeData[1]);
      delay(3.3);
      EEPROM.write(ALARM_MINUTE_EEPROM, timeData[2]);
      delay(3.3);
      EEPROM.write(ALARM_HOUR_EEPROM, timeData[3]);
      delay(3.3);
      EEPROM.write(ALARM_DAY_EEPROM, timeData[5]);
      delay(3.3);
      EEPROM.write(ALARM_MONTH_EEPROM, timeData[6]);
      delay(3.3);
      EEPROM.write(ALARM_YEAR_EEPROM, timeData[7]);
    }
    else if (dataFromPhone[0] == 'A' && dataFromPhone[1] == 'T' && dataFromPhone[2] == 'F')
    {
      int *timeFromBT;
      timeFromBT = readDataFromBT(dataFromPhone);

      int timeData[7];
      for (int i = 1; i < 8; i++)
      {
        timeData[i] = *(timeFromBT + i);
      }

      EEPROM.write(STOP_ALARM_SECOND_EEPROM, timeData[1]);
      delay(3.3);
      EEPROM.write(STOP_ALARM_MINUTE_EEPROM, timeData[2]);
      delay(3.3);
      EEPROM.write(STOP_ALARM_HOUR_EEPROM, timeData[3]);
      delay(3.3);
      EEPROM.write(STOP_ALARM_DAY_EEPROM, timeData[5]);
      delay(3.3);
      EEPROM.write(STOP_ALARM_MONTH_EEPROM, timeData[6]);
      delay(3.3);
      EEPROM.write(STOP_ALARM_YEAR_EEPROM, timeData[7]);
    }
    else if (dataFromPhone[0] == 'S' && dataFromPhone[1] == 'E' && dataFromPhone[2] == 'T')
    {
      int *settingsFromBT;
      settingsFromBT = readDataFromBT(dataFromPhone);

      int timeData[7];
      for (int i = 1; i < 8; i++)
      {
        timeData[i] = *(settingsFromBT + i);
      }

      EEPROM.write(SENSORS_AMOUNT_EEPROM, timeData[1]);
      delay(3.3);
      EEPROM.write(IRRIGATIOM_MODE_EEPROM, timeData[2]);

      Serial.print(" AM: ");
      Serial.print(EEPROM.read(SENSORS_AMOUNT_EEPROM));
      Serial.print(" MOD: ");
      Serial.print(EEPROM.read(IRRIGATIOM_MODE_EEPROM));
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

  if (isFirstAlarm() && isAutomode())
  {
    // TO DO: irrigate()
    digitalWrite(LED, HIGH);
  }
  if (isStopFirstAlarm() && isAutomode())
  {
    // TO DO: stopIrrigate()
    digitalWrite(LED, LOW);
  }
  if (!isAutomode())
  {
    // TO DO: stopIrrigate()
    digitalWrite(LED, LOW);
  }
  
}