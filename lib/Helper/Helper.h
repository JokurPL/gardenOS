#ifndef Helper_h
#define Helper_h

#include <Arduino.h>
#include <EEPROM.h>
#include <DS3231.h>
#include <SoftwareSerial.h>
#include <string.h>

#define RELAY 10
#define firstAnalogPin 54

// ####### ALARM(PLANNED IRRIGATION) CONSTANTS #######

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
#define MIN_MOISTURE_EEPROM 14

// ####### CYCLIC IRRIGATION CONSTANTS #######
#define CYCLIC_MONDAY_EEPROM 15
#define CYCLIC_TUESDEY_EEPROM 16
#define CYCLIC_WEDNESDAY_EEPROM 17
#define CYCLIC_THURSDAY_EEPROM 18
#define CYCLIC_FRIDAY_EEPROM 19
#define CYCLIC_SATURDAY_EEPROM 20
#define CYCLIC_SUNDAY_EEPROM 21
#define CYCLIC_START_HOUR_EEPROM 22
#define CYCLIC_START_MINUTE_EEPROM 23
#define CYCLIC_STOP_HOUR_EEPROM 24
#define CYCLIC_STOP_MINUTE_EEPROM 25

// ####### MANUAL IRRIGATION CONSTANTS #######
#define MANUAL_IRRIGATION_EEPROM 26

class Helper
{
private:
    bool Century = false;
    bool h12;
    bool PM;
    bool ADy, A12h, Apm;
public:
    bool isManualIrrigation();
    bool startCyclicIrrigation(DS3231);
    bool stopCyclicIrrigation(DS3231);
    bool isFirstAlarm(DS3231);
    bool isStopFirstAlarm(DS3231);

    int mode();
    int *readDataFromBT(String);
    int minMoisture();

    float toAverage(int);

    void readTime(DS3231, SoftwareSerial);
    void initAnalogs(int);
    void setTime(DS3231, int, int, int, int, int, int, int);

    void sendAverageInfo(SoftwareSerial, float);
    void sendRelayInfo(SoftwareSerial, int);
    void sendInformation(DS3231, SoftwareSerial, float);

    void startIrrigation();
    void stopIrrigation();
};

#endif