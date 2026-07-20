#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define MQ135_PIN 34

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_BME280 bme;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Initialize BME280
  if (!bme.begin(0x76)) {
    if (!bme.begin(0x77)) {
      Serial.println("No BME280 found. Check wiring!");
      while (1);
    }
  }
  Serial.println("BME280 ready!");

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("No OLED found. Check wiring!");
    while (1);
  }
  Serial.println("OLED ready!");

  // MQ135 is analog, no initialization needed
  pinMode(MQ135_PIN, INPUT);
  Serial.println("MQ135 ready!");

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("System Starting...");
  display.display();
  delay(2000);
}

void loop() {
  // BME280 readings
  float tempF = (bme.readTemperature() * 9.0 / 5.0) + 32.0;
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;

  // MQ135 reading (raw analog value 0-4095)
  int airQuality = analogRead(MQ135_PIN);

  // Determine air quality label
  String airLabel;
  if (airQuality < 1000) {
    airLabel = "Good";
  } else if (airQuality < 2000) {
    airLabel = "Moderate";
  } else if (airQuality < 3000) {
    airLabel = "Poor";
  } else {
    airLabel = "Bad";
  }

  // Print to Serial Monitor
  Serial.println("--------------------");
  Serial.print("Temp:      "); Serial.print(tempF); Serial.println(" F");
  Serial.print("Humidity:  "); Serial.print(humidity); Serial.println(" %");
  Serial.print("Pressure:  "); Serial.print(pressure); Serial.println(" hPa");
  Serial.print("Air Quality: "); Serial.print(airQuality); Serial.print(" raw ("); Serial.print(airLabel); Serial.println(")");

  // Display on OLED
  display.clearDisplay();

  // Title
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("ENV MONITOR");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

  // Temperature
  display.setCursor(0, 13);
  display.print("Temp:  ");
  display.print(tempF, 1);
  display.println(" F");

  // Humidity
  display.setCursor(0, 23);
  display.print("Humid: ");
  display.print(humidity, 1);
  display.println(" %");

  // Pressure
  display.setCursor(0, 33);
  display.print("Press: ");
  display.print(pressure, 1);
  display.println(" hPa");

  // Air Quality
  display.setCursor(0, 43);
  display.print("Air:   ");
  display.print(airQuality);
  display.print(" ");
  display.println(airLabel);

  display.display();

  delay(2000);
}