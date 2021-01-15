#ifndef Helper_h
#define Helper_h

#include <Arduino.h>
#include <EEPROM.h>
#include <DS3231.h>
#include <SoftwareSerial.h>
#include <string.h>

bool Century = false;
bool h12;
bool PM;
bool ADy, A12h, Apm;

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
    /* data */
public:
    bool isManualIrrigation(int address);
    bool startCyclicIrrigation(DS3231 Clock, int mondayAddress, int tuesdeyAddress, int wednesdayAddress, int thursdayAddress, int fridayAddress, int saturdayAddress, int sundayAddress, int startHourAddress, int startMinuteAddress, int stopHourAddress, int stopMinuteAddress);
    bool stopCyclicIrrigation(DS3231 Clock, int mondayAddress, int tuesdeyAddress, int wednesdayAddress, int thursdayAddress, int fridayAddress, int saturdayAddress, int sundayAddress, int stopHourAddress, int stopMinuteAddress);
    bool isFirstAlarm(DS3231 Clock);
    bool isStopFirstAlarm(DS3231 Clock);

    int mode();
    int *readDataFromBT(String dataFromPhone);
    int minMoisture();

    float toAverage(int moistureSensorsAmonut);

    void readTime(DS3231 Clock, SoftwareSerial hc06);
    void initAnalogs(int moistureSensorsAmonut);
    void setTime(DS3231 Clock, int second, int minute, int hour, int dayOfWeek, int day, int month, int year);

    void sendAverageInfo(SoftwareSerial hc06, float moisture);
    void sendRelayInfo(SoftwareSerial hc06, int state);
    void sendInformation(DS3231 Clock, SoftwareSerial hc06, float averageMoisture);

    void startIrrigation();
    void stopIrrigation();
};

#endif