#include <OneWire.h>
#include <UTFT.h>

// Declare which fonts we will be using
extern uint8_t BigFont[];

// Initialize TFT LCD Display
UTFT LCD(ITDB32S, 38, 39, 40, 41);

// Initialize DS18S20 temperature probe.
OneWire ds(11);

int pinNumber = 44; // 11 is temperature probe, 49 is alarm, 52 is 220 V SSR, 50 is 110 V SSR.

void setup()
{
  LCD.InitLCD();
  LCD.setFont(BigFont);

  Serial.begin(9600);
}

void loop()
{
  char buffer[10];
  float temp = getTemp();
  String strTemp = dtostrf(temp, 5, 2, buffer);

  LCD.fillScr(VGA_TEAL);
  LCD.setBackColor(VGA_TEAL);
  LCD.setColor(255, 255, 255);
  LCD.print(strTemp, CENTER, 80);
  LCD.print("Degrees F", CENTER, 100);

  delay(1000);
}

float getTemp() {
  // Returns the temperature from one DS18S20 in DEG F

  byte data[12];
  byte addr[8];

  if (!ds.search(addr)) {
    // No more sensors on chain, reset search
    ds.reset_search();
    return -1000;
  }

  if (OneWire::crc8( addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return -1000;
  }

  if (addr[0] != 0x10 && addr[0] != 0x28) {
    Serial.print("Device is not recognized");
    return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1); // Start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad

  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); // Using two's compliment
  float TemperatureSum = (((tempRead / 16) * 9) / 5) + 32;

  return TemperatureSum;
}
