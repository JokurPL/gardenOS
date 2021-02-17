#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DS3231.h>
#include <Wire.h>
#include <string.h>
#include <EEPROM.h>
#include <Timers.h>
#include <Helper.h>
#include <Adafruit_SSD1306.h>
// #include <Oled.h>

DS3231 Clock;
Timer sendMoistureTimer;
Helper helper;
// Oled oledHelper;

int relayState = 0;
int lastRelayState = 0;

float averageMoisture;
float lastAverageMoisture;

int moistureSensorsAmonut = 0;

String dataFromPhone;

SoftwareSerial hc06(50, 51); // 2 - Rx, 3 - Tx | Arduino Rx -> HC Tx # Arduino Tx -> HC Rx by divider
                             // 22 - Rx(for MEGA), 23 - Tx(for MEGA)
/* * * * * KEYPAD * * * * * */
#define KEY_1 4
#define KEY_2 5
#define KEY_3 6
#define KEY_4 7

/* * * * * OLED * * * * * */
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const unsigned char logo[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc6, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x40, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0c, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00,
    0x00, 0x01, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00,
    0x00, 0x01, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x7f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x7f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x3f, 0x07, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x3c, 0x1e, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0xcc, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x39, 0xcc, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0xcc, 0x7f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x39, 0xce, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0xcf, 0x87, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x39, 0xcf, 0xe3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0xcf, 0xf3, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x19, 0xcf, 0xe6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x1c, 0x06, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x1e, 0x3e, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfe, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x1f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfe, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x1f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfe, 0x00, 0x00, 0x00};

void intro()
{
  
  display.clearDisplay();
  display.display();

  int LOGO_HEIGHT = 60;
  int LOGO_WIDTH = 60;

  display.clearDisplay();
  display.drawBitmap(
      (display.width() - LOGO_WIDTH) / 2,
      (display.height() - LOGO_HEIGHT) / 2,
      logo, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(2000);

  display.clearDisplay();
  display.display();
}

void displayMode() 
{
    switch (helper.mode())
  {
  case 1:
    display.setTextSize(1.5);
    display.setTextColor(WHITE, BLACK);
    display.setCursor(0, 36);
    display.print("Tryb: automatyczne podlewanie");
    display.display();
    break;

  case 2:
    display.setTextSize(1.5);
    display.setTextColor(WHITE, BLACK);
    display.setCursor(0, 36);
    display.print("Tryb: planowane podlewanie");
    display.display();
    break;

  case 3:
    display.setTextSize(1.5);
    display.setTextColor(WHITE, BLACK);
    display.setCursor(0, 36);
    display.print("Tryb: cykliczne podlewanie");
    display.display();
    break;

  case 4:
    display.setTextSize(1.5);
    display.setTextColor(WHITE, BLACK);
    display.setCursor(0, 36);
    display.print("Tryb: reczne podlewanie");
    display.display();
    break;

  default:
    break;
  }
}

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

  /* * * * * * OLED * * * * * */
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  intro();
  displayMode();

  lastRelayState = relayState;
  if (relayState == HIGH)
  {
    display.setTextSize(1.5);
    display.setTextColor(WHITE, BLACK);
    display.setCursor(0, 12);
    display.println("Stan: podlewanie");
    display.display();
  }
  else
  {
    display.setTextSize(1.5);
    display.setTextColor(WHITE, BLACK);
    display.setCursor(0, 12);
    display.println("Stan: spoczynek");
    display.display();
  }

  averageMoisture = helper.toAverage(moistureSensorsAmonut);
  display.setTextSize(1.5);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 24);
  display.print("Wilgotnosc: " + String(averageMoisture) + "%");
  display.display();
}

void loop()
{
  displayMode();

  relayState = digitalRead(RELAY);
  averageMoisture = helper.toAverage(moistureSensorsAmonut);
  // averageMoisture =  rand() % 100;

  /* * * * * Update relay state on change * * * * */
  if (relayState != lastRelayState)
  {
    helper.sendRelayInfo(hc06, relayState);
    lastRelayState = relayState;
    if (relayState == HIGH)
    {
      display.setTextSize(1.5);
      display.setTextColor(WHITE, BLACK);
      display.setCursor(0, 12);
      display.println("Stan: podlewanie");
      display.display();
      delay(100);
    }
    else
    {
      display.setTextSize(1.5);
      display.setTextColor(WHITE, BLACK);
      display.setCursor(0, 12);
      display.println("Stan: spoczynek ");
      display.display();
      delay(100);
    }
  }

  /* * * * * Update average moisture value on change * * * * */
  if (averageMoisture != lastAverageMoisture)
  {
    display.setTextSize(1.5);
    display.setTextColor(WHITE, BLACK);
    display.setCursor(0, 24);
    display.println("Wilgotnosc: " + String(averageMoisture) + "%");
    display.display();
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