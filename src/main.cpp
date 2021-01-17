#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DS3231.h>
#include <Wire.h>
#include <string.h>
#include <EEPROM.h>
#include <Timers.h>
#include <Helper.h>

DS3231 Clock;
Timer sendMoistureTimer;
Helper helper;

int relayState = 0;
int lastRelayState = 0;

float averageMoisture;
float lastAverageMoisture;

int moistureSensorsAmonut = 0;

String dataFromPhone;

SoftwareSerial hc06(2, 3); // 2 - Rx, 3 - Tx | Arduino Rx -> HC Tx # Arduino Tx -> HC Rx by divider

/* * * * * KEYPAD * * * * */
#define KEY_1 4
#define KEY_2 5
#define KEY_3 6
#define KEY_4 7

void setup()
{
  sendMoistureTimer.begin(100);
  helper.stopIrrigation();

  Wire.begin();
  Serial.begin(9600);
  hc06.begin(9600);

  /* * * * * KEYPAD INIT * * * * */
  pinMode(KEY_1, INPUT_PULLUP);
  pinMode(KEY_2, INPUT_PULLUP);
  pinMode(KEY_3, INPUT_PULLUP);
  pinMode(KEY_4, INPUT_PULLUP);

  helper.initAnalogs(moistureSensorsAmonut); // EEPROM.read(SENSORS_AMOUNT_EEPROM)
  pinMode(RELAY, OUTPUT);
}

void loop()
{
  relayState = digitalRead(RELAY);
  averageMoisture = helper.toAverage(moistureSensorsAmonut);

  /* * * * * Update relay state on change * * * * */
  if (relayState != lastRelayState)
  {
    helper.sendRelayInfo(hc06, relayState);
    lastRelayState = relayState;
  }

  /* * * * * Update average moisture value on change * * * * */
  if (averageMoisture != lastAverageMoisture)
  {
    if (sendMoistureTimer.available())
    {
      helper.sendAverageInfo(hc06, averageMoisture);
      lastAverageMoisture = averageMoisture;
      sendMoistureTimer.begin(100);
    }
  }

  /* * * * * Data from phone * * * * */
  if (hc06.available() > 0)
  {
    dataFromPhone = hc06.readString();
    Serial.print(dataFromPhone);
    if (dataFromPhone[0] == 'R' && dataFromPhone[1] == 'T' && dataFromPhone[2] == 'C')
    {
      hc06.print(dataFromPhone);

      int *timeFromBT;
      timeFromBT = helper.readDataFromBT(dataFromPhone);

      int timeData[7];

      for (int i = 1; i < 8; i++)
      {
        timeData[i] = *(timeFromBT + i);
      }

      helper.setTime(Clock, timeData[1], timeData[2], timeData[3], timeData[4], timeData[5], timeData[6], timeData[7]);
      helper.readTime(Clock, hc06);
    }
    else if (dataFromPhone[0] == 'A' && dataFromPhone[1] == 'T' && dataFromPhone[2] == 'S')
    {
      int *timeFromBT;
      timeFromBT = helper.readDataFromBT(dataFromPhone);

      int timeData[14];
      for (int i = 1; i < 15; i++)
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
      delay(3.3);
      EEPROM.write(STOP_ALARM_SECOND_EEPROM, timeData[8]);
      delay(3.3);
      EEPROM.write(STOP_ALARM_MINUTE_EEPROM, timeData[9]);
      delay(3.3);
      EEPROM.write(STOP_ALARM_HOUR_EEPROM, timeData[10]);
      delay(3.3);
      EEPROM.write(STOP_ALARM_DAY_EEPROM, timeData[12]);
      delay(3.3);
      EEPROM.write(STOP_ALARM_MONTH_EEPROM, timeData[13]);
      delay(3.3);
      EEPROM.write(STOP_ALARM_YEAR_EEPROM, timeData[14]);
    }
    else if (dataFromPhone[0] == 'S' && dataFromPhone[1] == 'E' && dataFromPhone[2] == 'T')
    {
      helper.stopIrrigation();

      int *settingsFromBT;
      settingsFromBT = helper.readDataFromBT(dataFromPhone);

      int timeData[4];
      for (int i = 1; i < 5; i++)
      {
        timeData[i] = *(settingsFromBT + i);
      }

      if (timeData[3] < 0 || timeData[3] > 100)
      {
        timeData[3] = 0;
      }

      EEPROM.write(SENSORS_AMOUNT_EEPROM, timeData[1]);
      delay(3.3);
      EEPROM.write(IRRIGATIOM_MODE_EEPROM, timeData[2]);
      delay(3.3);
      EEPROM.write(MIN_MOISTURE_EEPROM, timeData[3]);
      // delay(3.3);

      // moistureSensorsAmonut = EEPROM.read(SENSORS_AMOUNT_EEPROM);
    }
    else if (dataFromPhone[0] == 'S' && dataFromPhone[1] == 'C' && dataFromPhone[2] == 'I')
    {
      int *timeFromBT;
      timeFromBT = helper.readDataFromBT(dataFromPhone);

      int timeData[14];
      for (int i = 1; i < 12; i++)
      {
        timeData[i] = *(timeFromBT + i);
      }

      EEPROM.write(CYCLIC_MONDAY_EEPROM, timeData[1]);
      delay(3.3);
      EEPROM.write(CYCLIC_TUESDEY_EEPROM, timeData[2]);
      delay(3.3);
      EEPROM.write(CYCLIC_WEDNESDAY_EEPROM, timeData[3]);
      delay(3.3);
      EEPROM.write(CYCLIC_THURSDAY_EEPROM, timeData[4]);
      delay(3.3);
      EEPROM.write(CYCLIC_FRIDAY_EEPROM, timeData[5]);
      delay(3.3);
      EEPROM.write(CYCLIC_SATURDAY_EEPROM, timeData[6]);
      delay(3.3);
      EEPROM.write(CYCLIC_SUNDAY_EEPROM, timeData[7]);
      delay(3.3);
      EEPROM.write(CYCLIC_START_HOUR_EEPROM, timeData[8]);
      delay(3.3);
      EEPROM.write(CYCLIC_START_MINUTE_EEPROM, timeData[9]);
      delay(3.3);
      EEPROM.write(CYCLIC_STOP_HOUR_EEPROM, timeData[10]);
      delay(3.3);
      EEPROM.write(CYCLIC_STOP_MINUTE_EEPROM, timeData[11]);
    }
    else if (dataFromPhone[0] == 'M' && dataFromPhone[1] == 'A' && dataFromPhone[2] == 'N')
    {

      int *dataFromBT;
      dataFromBT = helper.readDataFromBT(dataFromPhone);

      int data[2];
      for (int i = 1; i < 3; i++)
      {
        data[i] = *(dataFromBT + i);
      }

      EEPROM.write(MANUAL_IRRIGATION_EEPROM, data[1]);
    }
    else if (dataFromPhone[0] == 'G' && dataFromPhone[1] == 'E' && dataFromPhone[2] == 'T')
    {
      helper.sendInformation(Clock, hc06, averageMoisture);
    }
    else if (dataFromPhone[0] == 'p')
    {
      if (digitalRead(RELAY) == HIGH)
      {
        digitalWrite(RELAY, LOW);
      }
      else
      {
        digitalWrite(RELAY, HIGH);
      }
    }
  }

  /* * * * * Write from Serial Monitor to HC06 * * * * */
  if (Serial.available() > 0)
  {
    hc06.write(Serial.read());
  }

  /* * * * * Executive section * * * * */
  if (helper.toAverage(moistureSensorsAmonut) < helper.minMoisture() && helper.mode() == 1)
  {
    helper.startIrrigation();
  }
  else if (helper.toAverage(moistureSensorsAmonut) >= helper.minMoisture() && helper.mode() == 1)
  {
    helper.stopIrrigation();
  }
  else if (helper.isFirstAlarm(Clock) && helper.mode() == 2)
  {
    helper.startIrrigation();
  }
  else if (helper.isStopFirstAlarm(Clock) && helper.mode() == 2)
  {
    helper.stopIrrigation();
  }
  else if (helper.startCyclicIrrigation(Clock) && helper.mode() == 3)
  {
    helper.startIrrigation();
  }
  else if (helper.stopCyclicIrrigation(Clock) && helper.mode() == 3)
  {
    helper.stopIrrigation();
  }
  else if (helper.isManualIrrigation() && helper.mode() == 4)
  {
    helper.startIrrigation();
  }
  else if (!helper.isManualIrrigation() && helper.mode() == 4)
  {
    helper.stopIrrigation();
  }
}