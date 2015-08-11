#include <SPI.h>

// MCP3201 ADC chip test circuit

// connections:
// MCP3201 Pin 5 (chip select) - Arduino Mega 2560 Pin 53 
// chip select - active is low. Pin LOW : device is selected
// Pin HIGH: device is off
// MCP3201 Pin 6 (data out) - Arduino Mega 2560 Pin 50
// MCP3201 Pin 7 (clock) - Arduino Mega 2560 Pin 52

#define ADC_SELECT_PIN 53 // MCP3201 Pin 5

void setup() {

  pinMode (ADC_SELECT_PIN, OUTPUT);
  digitalWrite(ADC_SELECT_PIN, HIGH);

  SPI.begin();
  
  pinMode(31, OUTPUT); // Gate set to low
  digitalWrite(31, LOW);// Gate set to low
  
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  
  delay(1000);

  int adc = read_adc();
  Serial.print(adc, BIN);
  Serial.print(' ');
  Serial.println(adc);
}


inline unsigned int read_adc(){
  byte byte0, byte1;
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE1));
  digitalWrite(ADC_SELECT_PIN, LOW); // Select MCP3201
  byte1 = SPI.transfer(0);
  byte0 = SPI.transfer(0);
  SPI.endTransaction();
  digitalWrite(ADC_SELECT_PIN, HIGH);
  
  return (((byte1 << 8) + byte0) >> 1) & 0xFFF;
}

