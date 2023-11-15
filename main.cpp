#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

const int ledPin = 23;
const int numLeds = 16;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLeds, ledPin, NEO_GRB + NEO_KHZ800);

float temperatura = 0.0;
bool isButton1Pressed = false;
bool isButton2Pressed = false;

void button1Routine();
void button2Routine();

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  strip.begin();
  strip.show(); // pixeles apagados

  Wire.begin(); // Inicializa la comunicación I2C

  // Establecer el LED Neopixel en color verde al inicio
  for (int i = 0; i < numLeds; i++) {
    strip.setPixelColor(i, strip.Color(0, 255, 0)); // Color verde
  }
  strip.show();
  delay(2000); // Mantener el LED encendido durante 3 segundos al inicio
  strip.clear();
  strip.show();
  strip.clear();
}

void loop() {
  while (Serial2.available() > 0) {
    char Cel = Serial2.read();

    if (Cel == 'c') {
      // Leer temperatura desde el sensor LM75 a través de I2C
      Wire.beginTransmission(0x48); // Dirección del sensor LM75 (0x48 en este caso)
      Wire.write(0x00); // Registro de lectura de temperatura
      Wire.endTransmission();
      Wire.requestFrom(0x48, 2); // Lea 2 bytes de datos

      if (Wire.available() >= 2) {
        int highByte = Wire.read();
        int lowByte = Wire.read();
        int tempData = (highByte << 8) | lowByte;
        temperatura = tempData / 256.0;
        
        Serial.print("La medición del sensor es: ");
        Serial2.println(temperatura);
        Serial.println(temperatura);
      }
    }

    // Detect button presses
    if (Cel == '1') {
      isButton1Pressed = true;
    } else if (Cel == '2') {
      isButton2Pressed = true;
    }
  }

  // Check for button actions
  if (isButton1Pressed) {
    button1Routine();
    isButton1Pressed = false;
  } else if (isButton2Pressed) {
    button2Routine();
    isButton2Pressed = false;
  }
}

void button1Routine() {
  // Define the routine for button 1 press
  for (int i = 0; i < numLeds; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0)); // Red
    strip.show();
    delay(100);
  }
}

void button2Routine() {
  // Define the routine for button 2 press
  for (int i = 0; i < numLeds; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 255)); // Blue
    strip.show();
    delay(100);
  }
}
