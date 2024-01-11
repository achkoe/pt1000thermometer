//#include <stdint.h>
//#include <avr/io.h>
//#include <util/delay.h>

#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>



#define VPin 9
#define analogPin A1
int value;
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

const float A = 3.9083E-3;
const float B = -5.7750E-7;
// const float R1 = 1000;
// const float R0 = 1000;
const float R3 = 1000;
const float R4 = 1.0 / (1.0/1000.0 + 1.0/10000.0);
const float v = 1024 * 13;
const float k = R4 / (R3 + R4);
float p, q, s, T, RmR0;

void setup() {
    Serial.begin(9600);
    Serial.println("Hello world!");
    lcd.begin(16, 2);
    lcd.setBacklight(7);
    lcd.print("Hello, world!");
    // analogReference(INTERNAL); // use internal 1.1V reference
    analogReference(EXTERNAL); // use external reference
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(VPin, OUTPUT);
    digitalWrite(VPin, LOW);
}

void loop() {
    digitalWrite(LED_BUILTIN, LOW);
    delay(2000);           // waits 100 milliseconds
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(VPin, HIGH);
    delay(10);
    value = analogRead(analogPin);
    digitalWrite(VPin, LOW);
    Serial.println(value);

    /*
    Vmeas = value * Vref / 1024;
    Va = Vmeas / v + Vref * R4 / (R3 + R4);
    Rm = R1 * Va / (Vref - Va);
    */
    RmR0 = (value + v * k) / (v * (1 - k) - value);

    p = A / B;
    q = (1 - RmR0) / B;
    s = sqrt((p / 2) * (p / 2) - q);
    T = -p / 2 - s;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.setCursor(0, 0);
    lcd.print(T);
    lcd.setCursor(0, 1);
    lcd.setCursor(0, 1);
    lcd.print(value);
}

//"/home/achimk/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/bin/avrdude" "-C/home/achimk/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/etc/avrdude.conf" -v -V -patmega328p -carduino "-P/dev/ttyUSB0" -b57600 -D "-Uflash:w:/tmp/arduino-sketch-336C2F0C210C42A89507639BC7B31230/test.ino.hex:i"
