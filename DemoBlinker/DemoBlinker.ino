
#if 0
#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_SPI pn532spi(SPI, 10);
NfcAdapter nfc = NfcAdapter(pn532spi);
#else

#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
#endif



void setup() {


  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Serial.println("NDEF Reader");
  nfc.begin();
  Serial.println("\nScan a NFC tag\n");
}

void loop() {
  // put your main code here, to run repeatedly:

  
  if (nfc.tagPresent())
  {
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);
    //NfcTag tag = nfc.read();
    //tag.print();
  }

  digitalWrite(13, LOW);   // turn the LED on (HIGH is the voltage level)
}
