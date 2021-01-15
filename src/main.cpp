#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DS3231.h>
#include <Wire.h>
#include <string.h>
#include <EEPROM.h>
#include <Timers.h>
#include <Helper.h>

// Time variables
DS3231 Clock;
Timer sendMoistureTimer;
Helper functions;

int relayState = 0;
int lastRelayState = 0;

int moistureSensorsAmonut = 0;

float averageMoisture;
float lastAverageMoisture;

String dataFromPhone;

SoftwareSerial hc06(2, 3); // 2 - Rx, 3 - Tx | Arduino Rx -> HC Tx # Arduino Tx -> HC Rx by divider

void setup()
{
  sendMoistureTimer.begin(100);
  functions.stopIrrigation();

  Wire.begin();
  Serial.begin(9600);
  hc06.begin(9600);

  functions.initAnalogs(moistureSensorsAmonut);
  pinMode(RELAY, OUTPUT); // simple RELAY
}

void loop()
{

  relayState = digitalRead(RELAY);
  averageMoisture = functions.toAverage(moistureSensorsAmonut);

  if (relayState != lastRelayState)
  {
    functions.sendRelayInfo(hc06, relayState);
    lastRelayState = relayState;
  }

  if (averageMoisture != lastAverageMoisture)
  {
    if (sendMoistureTimer.available())
    {
      functions.sendAverageInfo(hc06, averageMoisture);
      lastAverageMoisture = averageMoisture;
      sendMoistureTimer.begin(100);
    }
  }

  if (hc06.available() > 0)
  {
    dataFromPhone = hc06.readString();
    Serial.print(dataFromPhone);
    if (dataFromPhone[0] == 'R' && dataFromPhone[1] == 'T' && dataFromPhone[2] == 'C')
    {
      hc06.print(dataFromPhone);

      int *timeFromBT;
      timeFromBT = functions.readDataFromBT(dataFromPhone);

      int timeData[7];

      for (int i = 1; i < 8; i++)
      {
        timeData[i] = *(timeFromBT + i);
      }

      functions.setTime(Clock, timeData[1], timeData[2], timeData[3], timeData[4], timeData[5], timeData[6], timeData[7]);
      functions.readTime(Clock, hc06);
    }
    else if (dataFromPhone[0] == 'A' && dataFromPhone[1] == 'T' && dataFromPhone[2] == 'S')
    {
      int *timeFromBT;
      timeFromBT = functions.readDataFromBT(dataFromPhone);

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
      functions.stopIrrigation();

      int *settingsFromBT;
      settingsFromBT = functions.readDataFromBT(dataFromPhone);

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
      timeFromBT = functions.readDataFromBT(dataFromPhone);

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
      dataFromBT = functions.readDataFromBT(dataFromPhone);

      int data[2];
      for (int i = 1; i < 3; i++)
      {
        data[i] = *(dataFromBT + i);
      }

      EEPROM.write(MANUAL_IRRIGATION_EEPROM, data[1]);
    }
    else if (dataFromPhone[0] == 'G' && dataFromPhone[1] == 'E' && dataFromPhone[2] == 'T')
    {
      functions.sendInformation(Clock, hc06, averageMoisture);
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

  //Write from Serial Monitor to HC06
  if (Serial.available() > 0)
  {
    hc06.write(Serial.read());
  }

  if (functions.toAverage(moistureSensorsAmonut) < functions.minMoisture() && functions.mode() == 1)
  {
    functions.startIrrigation();
  }
  else if (functions.toAverage(moistureSensorsAmonut) >= functions.minMoisture() && functions.mode() == 1)
  {
    functions.stopIrrigation();
  }
  else if (functions.isFirstAlarm(Clock) && functions.mode() == 2)
  {
    functions.startIrrigation();
  }
  else if (functions.isStopFirstAlarm(Clock) && functions.mode() == 2)
  {
    functions.stopIrrigation();
  }
  else if (functions.startCyclicIrrigation(Clock) && functions.mode() == 3)
  {
    functions.startIrrigation();
  }
  else if (functions.stopCyclicIrrigation(Clock) && functions.mode() == 3)
  {
    functions.stopIrrigation();
  }
  else if (functions.isManualIrrigation() && functions.mode() == 4)
  {
    functions.startIrrigation();
  }
  else if (!functions.isManualIrrigation() && functions.mode() == 4)
  {
    functions.stopIrrigation();
  }
}