#include <SPI.h>

// MCP4911 DAC chip test circuit

//connections:
// MCP4911 Pin 2 (chip select) - Arduino Mega 2560 Pin 49
// chip select - active is low. Pin LOW : device is selected
// Pin HIGH: device is off
// MCP4911 Pin 4 (data in) - Arduino Mega 2560 Pin 51
// MCP4911 Pin 3 (clock) - Arduino Mega 2560 Pin 52

#define DAC_SELECT_PIN 49 // MCP4911 Pin 2

void setup() {

  pinMode (DAC_SELECT_PIN, OUTPUT);
  digitalWrite(DAC_SELECT_PIN, HIGH);
  
  SPI.begin();  
  
  pinMode(31, OUTPUT); // Disable gate electrode
  digitalWrite(31, HIGH);
  
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  for (int i=0; i<1024; i++) {
    set_dac(i, true);
    delay(1);
  }
  
  
  set_dac(0, true);
  delay(100);

}

inline void set_dac(unsigned int voltage, bool enable){
  // add the control bits to the voltage input
  
  // two don't care bits at the end
  voltage = voltage << 2;
  
  // add shutdown bit (|SHDN), 1 means output is enabled
  voltage = voltage + (1 << 12); // bit position 12: 10 bit + 2 bits don't care
  // add gain bit (|GA), 1 means gain is 1x (0 means gain is 2x)
  voltage = voltage + (1 << 13);
  // add buffer bit (BUF), 1 = buffered, 0 = unbuffered
  voltage = voltage + (1 << 14);
  // add first bit = 0 (always), do nothing
  // voltage = voltage + (0 << 15); // line included for completeness
  
  
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));
  digitalWrite(DAC_SELECT_PIN, LOW); // Select MCP4911
  SPI.transfer((voltage >> 8) & 0xFF);
  SPI.transfer(voltage & 0xFF);
  SPI.endTransaction();
  digitalWrite(DAC_SELECT_PIN, HIGH); // Deselect MCP4911
  
}

