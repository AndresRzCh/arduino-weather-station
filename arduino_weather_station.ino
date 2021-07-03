
// Import Packages
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"

// Custom Variables
const byte BACKLIGHTPIN=3; // Digital pin which is wired to D10 in LCD Screen
int TIME_DELAY = 2000; // Time of each iteration in ms
int SAVE_ITERATIONS = 150; // Iterations between saves
int LIGHT_STEP = 20; // Light step (Light from 0 to 240)
int INITIAL_LIGHT = 80; // Initial light setup
int MSG_TIME = 1000; // Time for showing a message in LCD screen in ms

// Variable Definitions
char dateString[11];
char timeString[18];
float temperature;
float humidity;
float pressure;
int count=0;
int thisMinute;
int thisHour;
int thisMonth;
int thisYear;
int brightness=INITIAL_LIGHT;
int save_iterations=SAVE_ITERATIONS-1;
DateTime now;

// Initialize Packages
Adafruit_BME280 bme;
LiquidCrystal lcd(8,9,4,5,6,7);
File file;
RTC_DS1307 rtc;

// Initial Setup
void setup(void) {

  Serial.begin(9600);

  analogWrite(3, brightness);
  lcd.begin(16, 2);
  lcd.print("Starting...");
  Serial.println("Starting...");

  rtc.begin();
  if (!rtc.isrunning())
  {
    Serial.println("RTC No Battery");
    lcd.clear();
    lcd.print("RTC No Battery");
    while (1);
  }

  if (!bme.begin(0x76)) {
    Serial.println("Check BME Wiring");
    lcd.clear();
    lcd.print("Check BME Wiring");
    while (1);
  }
}

// Loop main function
void loop() {

  int x = analogRead (0);

  if (x < 60) { //Right Button
    next();
  }
  else if (x < 200) { //Up Button
    increase_light();
  }
  else if (x < 400) { //Down Button
    decrease_light();
  }
  else if (x < 600) { //Left Button
    previous();
  }
  else if (x < 800) { //Select Button
    save();
  }

  delay(TIME_DELAY);

  getData();
  printinfo();

  if (count >= save_iterations) {
    save();
  }
  else {
    count++;
  }
}

void  getData()
{
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure();
  pressure = pressure/100.0F;
  now = rtc.now();
  thisMinute = now.minute();
  thisHour = now.hour();
  thisMonth = now.month();
  thisYear = now.year();
}

void save()
{
  count = 0;

  if (!SD.begin(10)) {
    Serial.println("No SD Card!");
    lcd.clear();
    lcd.print("No SD Card!");
  }
  else {

    snprintf(dateString, sizeof(dateString), "%4d%02d.TXT", thisYear, thisMonth);
    file = SD.open(dateString, FILE_WRITE);
    Serial.print("Write");
    Serial.print(dateString);
    lcd.clear();
    lcd.print("Write");
    lcd.print(dateString);

    if (file) {
      file.print(now.unixtime());
      file.print(";");
      file.print(temperature);
      file.print(";");
      file.print(humidity);
      file.print(";");
      file.println(pressure);
      file.close();
      Serial.println("Done.");
      lcd.setCursor(0,1);
      lcd.print("Done.");
    }
    else {
      Serial.print("ERROR!");
      lcd.setCursor(0,1);
      lcd.print("ERROR!");
    }
  }
  delay(MSG_TIME);
}

void next() {
  int r = (save_iterations - count) * (TIME_DELAY / 1000);
  int minutes = r / 60;
  int seconds = r % 60;
  Serial.print("Next Save: ");
  Serial.print(minutes);
  Serial.print(" min ");
  Serial.print(seconds);
  Serial.println("s");
  lcd.clear();
  lcd.print("Next Save: ");
  lcd.setCursor(0,1);
  lcd.print(minutes);
  lcd.print(" min ");
  lcd.print(seconds);
  lcd.print("s");
}

void previous() {
  int r = (count) * (TIME_DELAY / 1000);
  int minutes = r / 60;
  int seconds = r % 60;
  Serial.print("Last Save: ");
  Serial.print(minutes);
  Serial.print(" min ");
  Serial.print(seconds);
  Serial.println("s");
  lcd.clear();
  lcd.print("Last Save: ");
  lcd.setCursor(0,1);
  lcd.print(minutes);
  lcd.print(" min ");
  lcd.print(seconds);
  lcd.print("s");
}

void printinfo() {

  Serial.print("T: ");
  Serial.print(temperature);
  Serial.print("º | H: ");
  Serial.print(humidity);
  Serial.print("% | P: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  lcd.clear();
  snprintf(timeString, sizeof(timeString), "%d:%02d", thisHour, thisMinute);
  lcd.print(timeString);

  if (pressure < 1000) {
    lcd.print(" ");
  }
  lcd.print(" ");
  String pressureString = String(pressure, 2);
  lcd.print(pressureString);
  lcd.print("hPa");
  lcd.setCursor(0,1);
  String temperatureString = String(temperature, 2);
  lcd.print(temperatureString);
  lcd.print((char)223);
  lcd.print("C ");

  if (temperature > -10) {
    lcd.print(" ");
  }

  if (humidity < 100) {
    lcd.print(" ");
  }

  String humidityString = String(humidity, 2);
  lcd.print(humidityString);
  lcd.print("%");
}

void increase_light() {
  brightness = min(240, brightness + LIGHT_STEP);
  analogWrite(3, brightness);
  lcd.clear();
  lcd.print("Increase Light");
  lcd.setCursor(0,1);
  lcd.print(brightness);
  lcd.print("/240");
}

void decrease_light() {
  brightness = max(0, brightness - LIGHT_STEP);
  analogWrite(3, brightness);
  lcd.clear();
  lcd.print("Decrease Light");
  lcd.setCursor(0,1);
  lcd.print(brightness);
  lcd.print("/240");
}
