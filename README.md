# Arduino Weather Station

This project is a simple weather station built on Arduino. It can measure and log the temperature, humidity, and atmospheric pressure.

---

## Parts Used
* [Arduino UNO Rev 3](https://store.arduino.cc/arduino-uno-rev3)
* [Data Logger Shield](https://learn.adafruit.com/adafruit-data-logger-shield)
* [LCD Keypad Shield](https://www.dfrobot.com/product-51.html)
* [BME280 Sensor](https://www.az-delivery.de/es/products/gy-bme280)

## Libraries
* RTClib
* Adafruit BME280 Library
* Adafruit Unified Sensor

## Setup
Stack the Arduino UNO, the Data Logger Shield, and the Keypad LCD Shield (read below). Wire the 5V, GND, SDA, SCL connections of the BME280 sensor to the corresponding pins on the Arduino. Format an SD Card (FAT16 or FAT32) and insert it on the Data Logger Shield.

#### Stacking Shields
To get the LCD backlight to work, pin D10 on LCD Shield must be blended so that the pin does not connect to the Data Logger Shield. Then it must be wired to pin D3 on Data Logger Shield, mapping pin D10 on LCD Shield with pin D3 on Data Logger Shield. The problem is that both the LCD Shield and the Data Logger Shield use pin D10 simultaneously, which causes a conflict, and backlight control becomes disabled.
