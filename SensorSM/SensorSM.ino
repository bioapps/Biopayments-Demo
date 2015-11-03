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


// STATES FOR STATE MACHINE
#define START 'S'
#define WRITE_NFC 'W'
#define READ_NFC  'R'
#define DELETE_NFC 'D'
#define BITCOINS 'B'
#define HELP 'H'

byte states[] = {
  START, WRITE_NFC, READ_NFC, DELETE_NFC, HELP, BITCOINS
};


int state = START;
int defaultState = START;
char stateMenu = 's';                           // Char Menu Option to print
// serial data
String inputString = "                         ";  // a string to hold incoming data
String parameters = "                         ";
String dataString = "                         ";
boolean stringComplete = false;  // whether the string is complete





void setup(void) {
  Serial.begin(115200);

  nfc.begin();

  // input string needs space
  inputString.reserve(30);
  inputString = "";
  parameters = "";


  Serial.print("#R");
}



// Functions


// check if it is a valid state for the state machine
boolean isState(int s) {
  boolean r = false;
  for (int i = 0; i < sizeof(states); i++) {
    r = (states[i] == s);
    if (r) return r;
  }
  return r;
}


// serial event
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}


void Readnfc()
{
  Serial.println("\nScan a NFC tag\n");

  if (nfc.tagPresent())
  {
    NfcTag tag = nfc.read();
    Serial.println(tag.getTagType());
    Serial.print("UID: "); Serial.println(tag.getUidString());

    if (tag.hasNdefMessage()) // every tag won't have a message
    {

      NdefMessage message = tag.getNdefMessage();
      Serial.print("\nThis NFC Tag contains an NDEF Message with ");
      Serial.print(message.getRecordCount());
      Serial.print(" NDEF Record");
      if (message.getRecordCount() != 1) {
        Serial.print("s");
      }
      Serial.println(".");

      // cycle through the records, printing some info from each
      int recordCount = message.getRecordCount();
      for (int i = 0; i < recordCount; i++)
      {
        Serial.print("\nNDEF Record "); Serial.println(i + 1);
        NdefRecord record = message.getRecord(i);
        // NdefRecord record = message[i]; // alternate syntax

        Serial.print("  TNF: "); Serial.println(record.getTnf());
        Serial.print("  Type: "); Serial.println(record.getType()); // will be "" for TNF_EMPTY

        // The TNF and Type should be used to determine how your application processes the payload
        // There's no generic processing for the payload, it's returned as a byte[]
        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);

        // Print the Hex and Printable Characters
        Serial.print("  Payload (HEX): ");
        PrintHexChar(payload, payloadLength);

        // Force the data into a String (might work depending on the content)
        // Real code should use smarter processing
        String payloadAsString = "";
        for (int c = 0; c < payloadLength; c++) {
          payloadAsString += (char)payload[c];
        }
        Serial.print("  Payload (as String): ");
        Serial.println(payloadAsString);

        // id is probably blank and will return ""
        String uid = record.getId();
        if (uid != "") {
          Serial.print("  ID: "); Serial.println(uid);
        }
      }
    }
  }
  delay(3000);
}


void ReadBtc()
{

  String TypeMod = "";

  if (nfc.tagPresent())
  {
    NfcTag tag = nfc.read();
    //Serial.println(tag.getTagType());
    //Serial.print("UID: "); Serial.println(tag.getUidString());

    if (tag.hasNdefMessage()) // every tag won't have a message
    {

      NdefMessage message = tag.getNdefMessage();
      //Serial.print("\nThis NFC Tag contains an NDEF Message with ");
      //Serial.print(message.getRecordCount());
      //Serial.print(" NDEF Record");
      /*
      if (message.getRecordCount() != 1) {
        Serial.print("s");
      }
      */
      Serial.println("#P");

      // cycle through the records, printing some info from each
      int recordCount = message.getRecordCount();
      for (int i = 0; i < recordCount; i++)
      {
        //Serial.print("\nNDEF Record "); Serial.println(i + 1);
        NdefRecord record = message.getRecord(i);
        // NdefRecord record = message[i]; // alternate syntax

        //Serial.print("  TNF: "); Serial.println(record.getTnf());
        //Serial.print("  Type: "); Serial.println(record.getType()); // will be "" for TNF_EMPTY

        // The TNF and Type should be used to determine how your application processes the payload
        // There's no generic processing for the payload, it's returned as a byte[]
        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);

        // Print the Hex and Printable Characters
        //Serial.print("  Payload (HEX): ");
        //PrintHexChar(payload, payloadLength);

        // Force the data into a String (might work depending on the content)
        // Real code should use smarter processing
        String payloadAsString = "";
        for (int c = 0; c < payloadLength; c++) {
          payloadAsString += (char)payload[c];
        }

        //TypeMod = payloadAsString.substring(0, payloadAsString.indexOf(':'));

        //TypeMod = payloadAsString.substring(0, payloadAsString.indexOf(':'));
        payloadAsString = payloadAsString.substring(1);
        //Serial.print(TypeMod);
        //if(TypeMod == "BT")
        Serial.println(payloadAsString);

        // id is probably blank and will return ""
        String uid = record.getId();
        if (uid != "") {
          Serial.print("  ID: "); Serial.println(uid);
        }
      }
    }
  }
  delay(3000);
}






void Writenfc() {


  Serial.println("\nPlace a formatted chip on the reader.");
  if (nfc.tagPresent()) {

    Serial.println("#P");
    NdefMessage message = NdefMessage();
    message.addUriRecord("BTC:USERNAME,PASSWORD");
    //message.addUriRecord("PN:840326-XXXX");
    //message.addUriRecord("EMAIL:YOUREMAILS");
    //message.addUriRecord("http://arduino.cc");
    //message.addTextRecord("Goodbye, Arduino!");
    boolean success = nfc.write(message);
    if (success) {
      Serial.println("#S");
    } else {
      Serial.println("#F1");
    }
  }
  delay(3000);

}



void Formatnfc()
{

boolean success;

  Serial.println("#P");
  if (nfc.tagPresent()) {
    boolean success = nfc.erase();
  }

  if (success) {
    Serial.println("#S");
  } else {
    Serial.println("#F2");
  }


}

void loop(void) {


  // get serial data
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);
    // it there is anything to do with it, do it!
    if (isState(inputString[0])) {
      state = inputString[0];
      parameters = inputString.substring(2);
    }
    // clear the string:

    inputString = "";
    stringComplete = false;
  }


  // state machine
  switch (state) {
    case START:

      defaultState = START;
      break;
    case HELP:
      Serial.println(F("Help info commands:\nSTART   'S'\nREAD    'R'\nWRITE   'W'\nBITCOINS  'B'\nHELP    'H'"));
      state = defaultState;
      stateMenu = 'H';

      break;

    case WRITE_NFC:
      //Serial.println("\nWaiting for an nfc ...");
      Writenfc();
      state = defaultState;
      stateMenu = 'W';
      Serial.println("#R");
      break;

    case READ_NFC:

      //Serial.println("\nWaiting for an nfc ...");
      Readnfc();
      state = defaultState;
      stateMenu = 'R';
      Serial.println("#R");
      break;

    case BITCOINS:


      //Serial.println("\nWaiting for an nfc ...");
      ReadBtc();
      state = defaultState;
      stateMenu = 'B';
      Serial.println("#R");
      break;

    case DELETE_NFC:



      break;


  }
}



