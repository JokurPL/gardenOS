#include "Helper.h"

bool Helper::isManualIrrigation()
{
    int mode = EEPROM.read(MANUAL_IRRIGATION_EEPROM);

    if (mode == 1)
    {
        return true;
    }
    return false;
}

bool Helper::startCyclicIrrigation(DS3231 Clock)
{
    // int second = Clock.getSecond();
    int minute = Clock.getMinute();
    int hour = Clock.getHour(h12, PM);
    int day = Clock.getDoW();

    int monday = EEPROM.read(CYCLIC_MONDAY_EEPROM);
    int tuesdey = EEPROM.read(CYCLIC_TUESDEY_EEPROM);
    int wednesday = EEPROM.read(CYCLIC_WEDNESDAY_EEPROM);
    int thursday = EEPROM.read(CYCLIC_THURSDAY_EEPROM);
    int friday = EEPROM.read(CYCLIC_FRIDAY_EEPROM);
    int saturday = EEPROM.read(CYCLIC_SATURDAY_EEPROM);
    int sunday = EEPROM.read(CYCLIC_SUNDAY_EEPROM);

    int startHour = EEPROM.read(CYCLIC_START_HOUR_EEPROM);
    int startMinute = EEPROM.read(CYCLIC_START_MINUTE_EEPROM);

    int stopHour = EEPROM.read(CYCLIC_STOP_HOUR_EEPROM);
    int stopMinute = EEPROM.read(CYCLIC_STOP_MINUTE_EEPROM);

    switch (day)
    {
    case 1:
        if (monday == 1 && hour >= startHour && minute >= startMinute && hour <= stopHour && minute < stopMinute)
        {
            return true;
        }
        break;

    case 2:
        if (tuesdey == 1 && hour >= startHour && minute >= startMinute && hour <= stopHour && minute < stopMinute)
        {
            return true;
        }
        break;

    case 3:
        if (wednesday == 1 && hour >= startHour && minute >= startMinute && hour <= stopHour && minute < stopMinute)
        {
            return true;
        }
        break;

    case 4:
        if (thursday == 1 && hour >= startHour && minute >= startMinute && hour <= stopHour && minute < stopMinute)
        {
            return true;
        }
        break;

    case 5:
        if (friday == 1 && hour >= startHour && minute >= startMinute && hour <= stopHour && minute < stopMinute)
        {
            return true;
        }
        break;

    case 6:
        if (saturday == 1 && hour >= startHour && minute >= startMinute && hour <= stopHour && minute < stopMinute)
        {
            return true;
        }
        break;

    case 7:
        if (sunday == 1 && hour == startHour && minute == startMinute && hour <= stopHour && minute < stopMinute)
        {
            return true;
        }
        break;

    default:
        return false;
        break;
    }
    return false;
}

bool Helper::stopCyclicIrrigation(DS3231 Clock)
{
    // int second = Clock.getSecond();
    int minute = Clock.getMinute();
    int hour = Clock.getHour(h12, PM);
    int day = Clock.getDoW();

    int monday = EEPROM.read(CYCLIC_MONDAY_EEPROM);
    int tuesdey = EEPROM.read(CYCLIC_TUESDEY_EEPROM);
    int wednesday = EEPROM.read(CYCLIC_WEDNESDAY_EEPROM);
    int thursday = EEPROM.read(CYCLIC_THURSDAY_EEPROM);
    int friday = EEPROM.read(CYCLIC_FRIDAY_EEPROM);
    int saturday = EEPROM.read(CYCLIC_SATURDAY_EEPROM);
    int sunday = EEPROM.read(CYCLIC_SUNDAY_EEPROM);

    int stopHour = EEPROM.read(CYCLIC_STOP_HOUR_EEPROM);
    int stopMinute = EEPROM.read(CYCLIC_STOP_MINUTE_EEPROM);

    switch (day)
    {
    case 1:
        if (monday == 1 && hour == stopHour && minute == stopMinute)
        {
            return true;
        }
        break;

    case 2:
        if (tuesdey == 1 && hour == stopHour && minute == stopMinute)
        {
            return true;
        }
        break;

    case 3:
        if (wednesday == 1 && hour == stopHour && minute == stopMinute)
        {
            return true;
        }
        break;

    case 4:
        if (thursday == 1 && hour == stopHour && minute == stopMinute)
        {
            return true;
        }
        break;

    case 5:
        if (friday == 1 && hour == stopHour && minute == stopMinute)
        {
            return true;
        }
        break;

    case 6:
        if (saturday == 1 && hour == stopHour && minute == stopMinute)
        {
            return true;
        }
        break;

    case 7:
        if (sunday == 1 && hour == stopHour && minute == stopMinute)
        {
            return true;
        }
        break;

    default:
        return false;
        break;
    }
    return false;
}

int Helper::mode()
{
    int irrigationMode = EEPROM.read(IRRIGATIOM_MODE_EEPROM);

    if (irrigationMode == 1)
    {
        return 1;
    }
    else if (irrigationMode == 2)
    {
        return 2;
    }
    else if (irrigationMode == 3)
    {
        return 3;
    }
    else if (irrigationMode == 4)
    {
        return 4;
    }

    return 1;
}

bool Helper::isFirstAlarm(DS3231 Clock)
{
    int secondEEPROM, minuteEEPROM, hourEEPROM, dayEEPROM, monthEEPROM, yearEEPROM, second, minute, hour, day, month, year, stopMinuteEEPROM, stopHourEEPROM;

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

    stopMinuteEEPROM = EEPROM.read(STOP_ALARM_MINUTE_EEPROM);
    stopHourEEPROM = EEPROM.read(STOP_ALARM_HOUR_EEPROM);

    //second == secondEEPROM &&

    if (minute >= minuteEEPROM && hour >= hourEEPROM && day == dayEEPROM && month == monthEEPROM && year == yearEEPROM && hour <= stopHourEEPROM && minute < stopMinuteEEPROM)
    {
        return true;
    }
    return false;
}

bool Helper::isStopFirstAlarm(DS3231 Clock)
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

void Helper::readTime(DS3231 Clock, SoftwareSerial hc06)
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

void Helper::initAnalogs(int moistureSensorsAmonut)
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

float Helper::toAverage(int moistureSensorsAmonut)
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

void Helper::setTime(DS3231 Clock, int second, int minute, int hour, int dayOfWeek, int day, int month, int year)
{
    Clock.setSecond(second);
    Clock.setMinute(minute);
    Clock.setHour(hour);
    Clock.setDoW(dayOfWeek);
    Clock.setDate(day);
    Clock.setMonth(month);
    Clock.setYear(year);
}

int *Helper::readDataFromBT(String dataFromPhone)
{
    static int dataArray[14];

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

void Helper::sendAverageInfo(SoftwareSerial hc06, float moisture)
{
    hc06.print("M");
    hc06.print(moisture);
}

void Helper::sendRelayInfo(SoftwareSerial hc06, int state)
{
    hc06.print("R");
    hc06.print(state);
}

void Helper::sendInformation(DS3231 Clock, SoftwareSerial hc06, float averageMoisture)
{
    int mode = EEPROM.read(MANUAL_IRRIGATION_EEPROM);

    int minuteStartPlanned = EEPROM.read(ALARM_MINUTE_EEPROM);
    int hourStartPlanned = EEPROM.read(ALARM_HOUR_EEPROM);
    int dayStartPlanned = EEPROM.read(ALARM_DAY_EEPROM);
    int monthStartPlanned = EEPROM.read(ALARM_MONTH_EEPROM);
    int yearStartPlanned = EEPROM.read(ALARM_YEAR_EEPROM);

    int minuteStopPlanned = EEPROM.read(STOP_ALARM_MINUTE_EEPROM);
    int hourStopPlanned = EEPROM.read(STOP_ALARM_HOUR_EEPROM);
    int dayStopPlanned = EEPROM.read(STOP_ALARM_DAY_EEPROM);
    int monthStopPlanned = EEPROM.read(STOP_ALARM_MONTH_EEPROM);
    int yearStopPlanned = EEPROM.read(STOP_ALARM_YEAR_EEPROM);

    int monday = EEPROM.read(CYCLIC_MONDAY_EEPROM);
    int tuesdey = EEPROM.read(CYCLIC_TUESDEY_EEPROM);
    int wednesday = EEPROM.read(CYCLIC_WEDNESDAY_EEPROM);
    int thursday = EEPROM.read(CYCLIC_THURSDAY_EEPROM);
    int friday = EEPROM.read(CYCLIC_FRIDAY_EEPROM);
    int saturday = EEPROM.read(CYCLIC_SATURDAY_EEPROM);
    int sunday = EEPROM.read(CYCLIC_SUNDAY_EEPROM);

    int hourStartCyclic = EEPROM.read(CYCLIC_START_HOUR_EEPROM);
    int minuteStartCyclic = EEPROM.read(CYCLIC_START_MINUTE_EEPROM);

    int hourStopCyclic = EEPROM.read(CYCLIC_STOP_HOUR_EEPROM);
    int minuteStopCyclic = EEPROM.read(CYCLIC_STOP_MINUTE_EEPROM);

    hc06.print("S");
    hc06.print(mode);

    delay(100);

    hc06.print("I");
    if (dayStartPlanned < 10)
    {
        hc06.print("0");
    }
    hc06.print(dayStartPlanned);
    hc06.print(".");
    if (monthStartPlanned < 10)
    {
        hc06.print("0");
    }
    hc06.print(monthStartPlanned);
    hc06.print(".");
    hc06.print("20");
    hc06.print(yearStartPlanned);
    hc06.print(" o ");
    if (hourStartPlanned < 10)
    {
        hc06.print("0");
    }
    hc06.print(hourStartPlanned);
    hc06.print(":");
    if (minuteStartPlanned < 10)
    {
        hc06.print("0");
    }
    hc06.print(minuteStartPlanned);

    delay(100);

    hc06.print("J");
    if (dayStopPlanned < 10)
    {
        hc06.print("0");
    }
    hc06.print(dayStopPlanned);
    hc06.print(".");
    if (monthStopPlanned < 10)
    {
        hc06.print("0");
    }
    hc06.print(monthStopPlanned);
    hc06.print(".");
    hc06.print("20");
    hc06.print(yearStopPlanned);
    hc06.print(" o ");
    if (hourStopPlanned < 10)
    {
        hc06.print("0");
    }
    hc06.print(hourStopPlanned);
    hc06.print(":");
    if (minuteStopPlanned < 10)
    {
        hc06.print("0");
    }
    hc06.print(minuteStopPlanned);

    delay(100);

    hc06.print("K");
    if (monday == 1)
    {
        hc06.print("Pn. ");
    }
    if (tuesdey == 1)
    {
        hc06.print("Wt. ");
    }
    if (wednesday == 1)
    {
        hc06.print("Śr. ");
    }
    if (thursday == 1)
    {
        hc06.print("Cz. ");
    }
    if (friday == 1)
    {
        hc06.print("Pt. ");
    }
    if (saturday == 1)
    {
        hc06.print("Sb. ");
    }
    if (sunday == 1)
    {
        hc06.print("Nd. ");
    }

    hc06.print("\n");
    hc06.print("od ");

    if (hourStartCyclic < 10)
    {
        hc06.print("0");
    }
    hc06.print(hourStartCyclic);
    hc06.print(":");
    if (minuteStartCyclic < 10)
    {
        hc06.print("0");
    }
    hc06.print(minuteStartCyclic);

    hc06.print(" do ");

    if (hourStopCyclic < 10)
    {
        hc06.print("0");
    }
    hc06.print(hourStopCyclic);
    hc06.print(":");
    if (minuteStopCyclic < 10)
    {
        hc06.print("0");
    }
    hc06.print(minuteStopCyclic);

    delay(100);

    sendRelayInfo(hc06, digitalRead(RELAY));

    delay(100);

    sendAverageInfo(hc06, averageMoisture);
}

int Helper::minMoisture()
{
    int moisture = EEPROM.read(MIN_MOISTURE_EEPROM);

    return moisture;
}

void Helper::startIrrigation()
{
    digitalWrite(RELAY, HIGH);
}

void Helper::stopIrrigation()
{
    digitalWrite(RELAY, LOW);
}