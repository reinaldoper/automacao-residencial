#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"

/**
  Connection
  Arduino    VoiceRecognitionModule
   2   ------->     TX  52 Arduino Mega
   3   ------->     RX  53 Arduino Mega
*/

//VR myVR(2, 3);   // 2:RX 3:TX

VR myVR(52, 53);   // 52:RX 53:TX

uint8_t records[7]; // save record
uint8_t buf[64];

int led = 3;

# define ligaInterna  (0)
# define ligaExterna     (1)
# define ligaAr    (2)
# define ligaPortao (3)



/**
  @brief   Print signature, if the character is invisible,
           print hexible value instead.
  @param   buf     --> command length
           len     --> number of parameters
*/
void printSignature(uint8_t *buf, int len)
{
  int i;
  for (i = 0; i < len; i++) {
    if (buf[i] > 0x19 && buf[i] < 0x7F) {
      Serial.write(buf[i]);
    }
    else {
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}

/**
  @brief   Print signature, if the character is invisible,
           print hexible value instead.
  @param   buf  -->  VR module return value when voice is recognized.
             buf[0]  -->  Group mode(FF: None Group, 0x8n: User, 0x0n:System
             buf[1]  -->  number of record which is recognized.
             buf[2]  -->  Recognizer index(position) value of the recognized record.
             buf[3]  -->  Signature length
             buf[4]~buf[n] --> Signature
*/
void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if (buf[0] == 0xFF) {
    Serial.print("NONE");
  }
  else if (buf[0] & 0x80) {
    Serial.print("UG ");
    Serial.print(buf[0] & (~0x80), DEC);
  }
  else {
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if (buf[3] > 0) {
    printSignature(buf + 4, buf[3]);
  }
  else {
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}

void setup()
{
  /** initialize */
  myVR.begin(9600);

  Serial.begin(115200);
  Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
  pinMode(led, OUTPUT);
  if (myVR.clear() == 0) {
    Serial.println("Recognizer cleared.");
  } else {
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while (1);
  }

  if (myVR.load((uint8_t)ligaInterna) >= 0) {
    Serial.println("ligaInterna loaded");
  }

  if (myVR.load((uint8_t)ligaExterna) >= 0) {
    Serial.println("ligaExterna loaded");
  }

  if (myVR.load((uint8_t)ligaAr) >= 0) {
    Serial.println("ligaAr loaded");
  }

  if (myVR.load((uint8_t)ligaPortao) >= 0) {
    Serial.println("ligaPortao loaded");
  }
  Serial1.begin(9600);
}

void loop()
{
  int ret;
  ret = myVR.recognize(buf, 50);
  if (ret > 0) {
    switch (buf[1]) {
      case ligaInterna:
        Serial1.write('A');
        break;
        
      case ligaExterna: 
        Serial1.write('B');
        break;

      case ligaAr: 
        Serial1.write('C');
        break;

      case ligaPortao: 
        Serial1.write('D');
        break;
        
      default:
        Serial1.println("Record function undefined");
        break;
    }
    /** voice recognized */
    printVR(buf);
  }
}
