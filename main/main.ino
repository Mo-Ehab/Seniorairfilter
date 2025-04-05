#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include "DHT20.h"



float temp;
float humidity;
int CO2;
int VOC;
int CO;
int PM;

DHT20 DHT;

#define PIN 6
#define NUMPIXELS 4

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);



void setup() {

  DHT20();
  Wire.begin(17, 18);
  DHT.begin();
  Serial.begin(9600);
  pinMode(14, OUTPUT);
  analogWriteFrequency(16, 30000);
  analogWrite(16, 16);
  pinMode(5, OUTPUT);
}


void loop() {


  GetTempHumidity();
  Serial.printf("Temprature and humidity: %.2f°C , %.2f%% \n", temp, humidity);

  GetPMConcentration();
  Serial.printf("PM concentration: %d µg/m3 \n", PM);

  GetCO2Concentration();
  Serial.printf("CO2 concentration: %d ppm \n", CO2);

  GetVOCConcentration();
  Serial.printf("VOC concentration: %d \n", VOC);

  GetCOConcentration();
  Serial.printf("CO concentration: %d \n", CO);

  if (VOC > 60 || CO > 60 || PM > 1000 || CO2 > 1000) {
    RGBcolor(128, 0, 0);
    analogWrite(16, 256);
    //tone(5, 85);
  } else {
    RGBcolor(0, 128, 0);
    analogWrite(16, 16);
    tone(5, 0);
  }

  delay(1000);
}


void GetTempHumidity() {

  DHT.read();
  temp = DHT.getTemperature();
  humidity = DHT.getHumidity();

  return;
}


void GetPMConcentration() {

  Wire.beginTransmission(77);
  Wire.write(0x24);
  Wire.endTransmission(false);
  Wire.requestFrom(77, 2);
  int a = Wire.read();
  int b = Wire.read();
  PM = a * 256 + b;

  return;
}

void GetCO2Concentration() {

  Wire.beginTransmission(0x32);
  Wire.write(0x64);
  Wire.write(0x03);
  Wire.endTransmission();
  Wire.beginTransmission(0x32);
  Wire.requestFrom(50, 3);
  int a = Wire.read();
  int b = Wire.read();
  CO2 = a * 256 + b;

  return;
}

void GetVOCConcentration() {

  VOC = analogRead(7);

  return;
}

void GetCOConcentration() {

  CO = analogRead(8);

  return;
}

void RGBcolor(int r, int g, int b) {

  pixels.clear();
  for (int i = 0; i < NUMPIXELS; i++) {

    pixels.setPixelColor(i, pixels.Color(r, g, b));
    pixels.show();
  }
}