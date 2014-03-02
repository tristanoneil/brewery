#include <UTFT.h>
#include <OneWire.h>
#include <PID_v1.h>

#define heatingElementPin 52
#define pumpPin 50

// Initialize TFT LCD Display
UTFT LCD(ITDB32S, 38, 39, 40, 41);

// Initialize DS18S20 temperature probe.
OneWire ds(11);

// Define variables to connect to the PID controller
double targetTemp, currentTemp, Output;

// Define a PID controller
PID pidController(&currentTemp, &Output, &targetTemp, 900, 0, 0, DIRECT);

int WindowSize = 5000;
unsigned long windowStartTime;

// Pump should be off by default
boolean pumpOn = false;

void setup()
{
  LCD.InitLCD();

  // Initialize USB at 9600 baud
  Serial.begin(9600);

  // Initialize heatingElementPin for writing
  pinMode(heatingElementPin, OUTPUT);

  // Initialize pumpPin for writing
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, HIGH);

  // Set window start time to now.
  windowStartTime = millis();

  // Set target temperature
  targetTemp = getTemp();

  // tell the PID to range between 0 and the full window size
  pidController.SetOutputLimits(0, WindowSize);

  // enable the PID controller
  pidController.SetMode(AUTOMATIC);
}

void loop()
{
  String content = "";
  char character;
  boolean newTemp = false;

  currentTemp = getTemp();
  pidController.Compute();

  if(millis() - windowStartTime>WindowSize) {
    windowStartTime += WindowSize;
  }

  if(Output < (millis() - windowStartTime)) {
    digitalWrite(heatingElementPin, LOW);
  }
  else {
    digitalWrite(heatingElementPin, HIGH);
  }

  while(Serial.available()) {
    character = Serial.read();

    if(character == 'p') {
      pumpOn = !pumpOn;

      if(pumpOn) {
        digitalWrite(pumpPin, LOW);
        Serial.println("Pump On");
      }
      else {
        digitalWrite(pumpPin, HIGH);
        Serial.println("Pump Off");
      }
    }

    if(character == 't') {
      newTemp = true;
    }
    else {
      content.concat(character);
    }
  }

  if (content != "") {
    if(newTemp) {
      targetTemp = atof(content.c_str());
      Serial.println(content);
      newTemp = false;
    }
  }

  displayTemps(currentTemp, targetTemp);
}
