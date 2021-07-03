#include "RTClib.h"

RTC_DS1307 rtc;

void setup() {
  rtc.begin();
  rtc.adjust(DateTime(__DATE__, __TIME__));
  Serial.begin(9600);

  if (!rtc.isrunning())
  {
    Serial.println("RTC not running!");
  }
}

void loop() {
  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  delay(3000);
}
