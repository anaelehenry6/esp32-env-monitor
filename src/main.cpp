#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

Adafruit_BME280 bme;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Starting BME280 test...");

  if (!bme.begin(0x76)) {
    Serial.println("Could not find BME280 sensor at 0x76, trying 0x77...");
    if (!bme.begin(0x77)) {
      Serial.println("No BME280 found. Check your wiring!");
      while (1); // stop here until fixed
    }
  }

  Serial.println("BME280 found! Reading data...");
}

void loop() {
  float tempC = bme.readTemperature();
  float tempF = (tempC * 9.0 / 5.0) + 32.0;
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F; // convert to hPa

  Serial.println("--------------------");
  Serial.print("Temperature: ");
  Serial.print(tempF);
  Serial.println(" °F");

  Serial.print("Humidity:    ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Pressure:    ");
  Serial.print(pressure);
  Serial.println(" hPa");

  delay(2000); // wait 2 seconds between readings
}