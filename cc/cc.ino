#include <stdlib.h>
//#include <avr/io.h>
//#include <util/delay.h>

#include "U8g2lib.h"

//U8GLIB_SSD1306_128X32 u8g2(U8G_I2C_OPT_NONE);    // I2C / TWI
U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0);



#define VPin 9
#define analogPin A1
int value;

const float A = 3.9083E-3;
const float B = -5.7750E-7;
// const float R1 = 1000;
// const float R0 = 1000;
const float R3 = 1000;
const float R4 = 1.0 / (1.0/1000.0 + 1.0/10000.0);
const float v = 1024 * 13;
const float k = R4 / (R3 + R4);
float p, q, s, T, RmR0;
char str[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int8_t i;
uint8_t pos = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("Hello world!");

    u8g2.begin();
    u8g2.setColorIndex(1);         // pixel on

    // analogReference(INTERNAL); // use internal 1.1V reference
    analogReference(EXTERNAL); // use external reference
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(VPin, OUTPUT);
    digitalWrite(VPin, LOW);

    for (i = -10; i < 60; i += 10) {
        Serial.println(24 + (int)((float(i) / 60) * 120));
    }
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

    dtostrf(T, 5, 1, str);
    // 0 1 2 3 4 5 6 7 8
    // - 2 3 . 4 ° C 0
    str[5] = 176;
    str[6] = 'C';
    Serial.println(str);

    u8g2.firstPage();
    do {

        #ifdef WITHRULER
            u8g2.setFont(u8g2_font_logisoso18_tf);  // height 28
            //u8g2.setFont(u8g2_font_logisoso16_tf);  // height 23
            u8g2.drawStr(pos, 21, str);
            // y = 21 .. 31 is usable
            // temperature range -10 ... +50 is mapped to 0 ... 120

            // draw markers 4:-10, 24:0, 44:10, 64:20, 84:30, 104:40, 124:50
            for (i = 0; i < 7; i++) {
                u8g2.drawVLine(4 + i * 20, 24, 6);
            }
            // draw zero mark
            u8g2.drawHLine(4 + 20 - 1, 30, 3);
            u8g2.drawHLine(4 + 20 - 2, 31, 5);

            // draw temperature
            i = 24 + (int)(120.0 * T / 60.0);
            u8g2.drawHLine(4, 26, i);
            u8g2.drawHLine(4, 27, i);
        #else
            u8g2.setFont(u8g2_font_logisoso28_tf);
            u8g2.drawStr(pos, 31, str);
        #endif
    } while( u8g2.nextPage() );

    pos = pos > 19 ? 0 : pos + 1;
}

//"/home/achimk/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/bin/avrdude" "-C/home/achimk/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/etc/avrdude.conf" -v -V -patmega328p -carduino "-P/dev/ttyUSB0" -b57600 -D "-Uflash:w:/tmp/arduino-sketch-336C2F0C210C42A89507639BC7B31230/test.ino.hex:i"
