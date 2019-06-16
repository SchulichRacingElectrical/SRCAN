#include <SPI.h>
#include <Wire.h>
#include <mcp_can.h>
#include <Adafruit_NeoPixel.h>
#define PIN 6
#define DRIVERWANTSBLOCK 1
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRBW + NEO_KHZ800); 

//GLOBALS
const int SPI_CS_PIN = 10;
unsigned int received = 0;
int rpm = 1;
int engineTemp = 1;
int flag = 10;
bool engineTooHot = false;
//GLOBALS

MCP_CAN CAN(SPI_CS_PIN);

//void GroupAndSend( String names[], int values[], int sizeofValues)
//{
//  String masterstring = "{";
//  for (int i = 0; i < sizeofValues ; i++) {
//    masterstring += names[i] + ":";
//    masterstring += String(values[i]);
//    if ( i != sizeofValues - 1) {
//      masterstring += ",";
//    }
//  }
//  masterstring += "}";
//  const char* masterstringc = masterstring.c_str();
//
//  Serial.println(masterstringc);
//  
//  //Wire.beginTransmission(8);
//  //Wire.write(masterstringc);
//  //Wire.endTransmission(); 
//}


int CombineBytes (int high, int low) {
  int num = high * 256 + low;
  if (num > 32767) {
    num = num - 65536;
  }
  return num;
}

void fillValueArray (unsigned char buf[], int values[], int numberofValues) {
  for (int i = 0, j = 0; i < numberofValues; i++, j += 2) {
    values[i] = CombineBytes(buf[j + 1], buf[j]);
  }
}

//LED CODE
void blockIncrementLED() { //Just for making the blue appear as a block
  for (int i = 10; i < 16; i++)
    strip.setPixelColor(i, 0, 0, 255);
}

void LED (int rpm) {//Actually determines the number of LEDS on based on the RPM value
  int numberoflights = ((int)(rpm - 10800) * 0.0064);        
  if (numberoflights > strip.numPixels()) 
    numberoflights = 0;
  for (int i = 0; i < strip.numPixels(); i++) {
     if (i < numberoflights && i < 6)
        strip.setPixelColor(i, 0, 255, 0);
     else if (i < numberoflights && i < 10)
        strip.setPixelColor(i, 255, 0, 0);
     else if (i < numberoflights && i < 16 && DRIVERWANTSBLOCK) {
        blockIncrementLED();
        break;
     } 
     else if (i < numberoflights && i < 16)
       strip.setPixelColor(i, 0, 0, 255);
     else
       strip.setPixelColor(i, 0);
    }
    strip.show();
    flag = true;
}

void flashWarning(){
  delay(100);
  for (int i = 0; i < strip.numPixels(); i++) {
     strip.setPixelColor(i, 255, 0, 0);
  }
  strip.show();
  delay(100);
  for (int i = 0; i < strip.numPixels(); i++) {
     strip.setPixelColor(i, 0);
  }
  strip.show();
}
//LED CODE

void setup() {
  Serial.begin(115200);
  Wire.begin();
  while (CAN_OK != CAN.begin(CAN_1000KBPS))              // init can bus : baudrate = 500k
    delay(100);
  strip.begin();
  strip.show();
  strip.setBrightness(5);
}

void loop() {
  unsigned char len = 0;
  unsigned char buf[8];
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(&len, buf); 
    unsigned long canId = CAN.getCanId();
    if(canId == 0x0CFFF048){   
      int value[1];
      fillValueArray(buf, value, 1);
      rpm = value[0];
      flag = (rpm >= 10000)?1:0;
    }
    if (canId == 0xCFFF548) {
      int values[3];
      fillValueArray(buf, values, 3);
      engineTemp = (int)(values[2]/10);
      if(engineTemp > 110) //Proper threshold is 110.
        engineTooHot = true;
    }
  }
  if(!engineTooHot)
    LED(rpm);
  else
    flashWarning();
    //Code for the display
    //Wire.beginTransmission(8);
    //Wire.write(data);
    //Wire.endTransmission(); 
}
  //0x0CFF_F048 first two bytes are RPM
  
//  if (CAN_MSGAVAIL == CAN.checkReceive()) {
//
//    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
//    
//
//    unsigned long canId = CAN.getCanId();
//
//    Serial.println("-----------------------------");
//    Serial.print("Get data from ID: ");
//    Serial.println(canId, HEX);
//    String names[] = {"rpm", "tps", "ftime", "igangl"};
//
//    int values[4];
//    fillValueArray(buf, values, 4);
//    GroupAndSend (names, values, 4);
//    //fillValueArray(buf+4,values,4);
//    Serial.println();
//
//    if (canId == 0x0CFFF048) {
//
//      String names[] = {"rpm", "tps", "ftime", "igangl"}; //"rpm", "tps", " fueltime", "ignitionangle"
//
//      int values[4];
//      fillValueArray(buf, values, 2);
//      GroupAndSend (names, values, 2);
//      fillValueArray(buf+4,values,2);
//      GroupAndSend(names+2,values,2);
//    }
//
//    if (canId == 0xCFFF148) {
//
//      String names[] = {"baro", "map", "lambda"};
//
//      int values[3];
//      fillValueArray(buf, values, 3);
//
//      GroupAndSend(names, values, 3);
//    }
//
//    if (canId == 0xCFFF248) {
//
//      String names[] = {"a1", "a2", "a3", "a4"}; //analog 1-4
//
//      int values[4];
//      fillValueArray(buf, values, 2);
//      GroupAndSend(names, values, 2);
//      fillValueArray(buf+4,values,2);
//      GroupAndSend(names+2,values,2);
//    }
//
//    if (canId == 0xCFFF348 ) {
//
//      String names[] = {"a5", "a6", "a7", "a8"}; //analog 5-8
//
//      int values[4];
//      fillValueArray(buf, values, 2);
//      GroupAndSend(names, values, 2);
//      fillValueArray(buf+4,values,2);
//      GroupAndSend(names+2,values,2);
//
//    }
//
//    if (canId == 0xCFFF448 ) {
//
//      String names[] = {"f1", "f2", "f3", "f4"};  //frequency 1-4
//
//      int values[4];
//      fillValueArray(buf, values, 2);
//      GroupAndSend(names, values, 2);
//      fillValueArray(buf+4,values,2);
//      GroupAndSend(names+2,values,2);
//      
//
//    }
//
//    if (canId == 0xCFFF548 ) {
//
//      String names[] = {"bat", "atmp", "ctmp"}; //"battery", "airtemp", "coolanttemp"
//
//      int values[3];
//      fillValueArray(buf, values, 3);
//
//      GroupAndSend (names, values, 3);
//    }
//
//    if (canId == 0xCFFF648 ) {
//      String names[] = {"t1", "t2"}; //thermistor 1-2
//
//      int values[2];
//      fillValueArray(buf, values, 2);
//
//      GroupAndSend(names,values,2);
//
//    }
//  }
//}
