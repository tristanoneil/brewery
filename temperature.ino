/*
 *
 * Temperature handles temperature related activies
 * like reading and displaying the current temperature.
 *
 */

/*
 * Displays a given temperature on the LCD screen
 */
void displayTemp(float temp) {
  // Declare which fonts we will be using
  extern uint8_t BigFont[];

  // Declare character buffer for float to string conversion.
  char buffer[10];

  // Convert temperature to a string representation
  // so it can be displayed on the LCD screen.
  String strTemp = dtostrf(temp, 5, 2, buffer);

  LCD.setFont(BigFont);
  LCD.fillScr(VGA_TEAL);
  LCD.setBackColor(VGA_TEAL);
  LCD.setColor(255, 255, 255);

  LCD.print(strTemp, CENTER, 80);
  LCD.print("Degrees F", CENTER, 100);
}

/*
 * Returns the temperature from one DS18S20 in DEG F
 */
float getTemp() {
  byte data[12];
  byte addr[8];

  // No more sensors on chain, reset search
  if (!ds.search(addr)) {
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

  // Start conversion, with parasite power on at the end
  ds.write(0x44, 1);

  byte present = ds.reset();
  ds.select(addr);

  // Read Scratchpad
  ds.write(0xBE);

  // we need 9 bytes
  for (int i = 0; i < 9; i++) {
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  // Using two's compliment
  float tempRead = ((MSB << 8) | LSB);

  float TemperatureSum = (((tempRead / 16) * 9) / 5) + 32;

  return TemperatureSum;
}
