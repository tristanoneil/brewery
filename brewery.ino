#include <UTFT.h>
#include <OneWire.h>

// Initialize TFT LCD Display
UTFT LCD(ITDB32S, 38, 39, 40, 41);

// Initialize DS18S20 temperature probe.
OneWire ds(11);

int heatingElementPin = 52;
int targetTemp = 100;

void setup()
{
  LCD.InitLCD();

  // Initialize USB at 9600 baud
  Serial.begin(9600);

  // Initialize heatingElementPin for writing
  pinMode(heatingElementPin, OUTPUT);
}

void loop()
{
  float currentTemp = getTemp();
  displayTemp(currentTemp);

  // Turn off/on heating element to adjust to
  // target temperature.
  if(currentTemp < targetTemp) {
    digitalWrite(heatingElementPin, HIGH);
  }
  else {
    digitalWrite(heatingElementPin, LOW);
  }

  delay(1000);
}
