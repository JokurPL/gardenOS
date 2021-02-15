#ifndef Oled_h
#define Oled_h

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
class Oled
{
// private:
public:
    void intro(Adafruit_SSD1306 display);
    void writeModeInfo(String text, Adafruit_SSD1306 display);
    void writeStateInfo(String text, Adafruit_SSD1306 display);
};

#endif