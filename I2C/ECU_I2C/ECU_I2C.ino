#include <mcp_can.h>
#include <SPI.h>
#include <Wire.h>

const int SPI_CS_PIN = 10;
unsigned int received = 0;

MCP_CAN CAN(SPI_CS_PIN);

void GroupAndSend( String names[], int values[], int sizeofValues)
{
  String masterstring = "{";
  for (int i = 0; i < sizeofValues ; i++) {
    masterstring += names[i] + ":";
    masterstring += String(values[i]);
    if ( i != sizeofValues - 1) {
      masterstring += ",";
    }
  }
  masterstring += "}";
  char* masterstringc = masterstring.c_str();
  
  Wire.beginTransmission(8);
  Wire.write(masterstringc);
  //for (int i =0; i < masterstring.length(); i++){
  //  Wire.write(masterstringc[i]);
  //}

  Wire.endTransmission(); 
}


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
  //Serial.println();                               idk why this line is here
}


void setup() {

  Serial.begin(115200);
  Wire.begin();
  
  while (CAN_OK != CAN.begin(CAN_1000KBPS))              // init can bus : baudrate = 500k
  {
    Serial.println("CAN BUS Shield init fail");
    Serial.println(" Init CAN BUS Shield again");
    delay(100);
  }

  Serial.println("CAN BUS Shield init ok!");
}


void loop() {

  unsigned char len = 0;
  unsigned char buf[8];

  if (CAN_MSGAVAIL == CAN.checkReceive()) {

    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

    unsigned long canId = CAN.getCanId();

    if (canId == 0x0CFFF048) {

      String names[] = {"rpm", "tps", "ftime", "igangl"}; //"rpm", "tps", " fueltime", "ignitionangle"

      int values[4];
      fillValueArray(buf, values, 2);
      GroupAndSend (names, values, 2);
      fillValueArray(buf+4,values,2);
      GroupAndSend(names+2,values,2);
    }

    if (canId == 0xCFFF148) {

      String names[] = {"baro", "map", "lambda"};

      int values[3];
      fillValueArray(buf, values, 3);

      GroupAndSend(names, values, 3);
    }

    if (canId == 0xCFFF248) {

      String names[] = {"a1", "a2", "a3", "a4"}; //analog 1-4

      int values[4];
      fillValueArray(buf, values, 2);
      GroupAndSend(names, values, 2);
      fillValueArray(buf+4,values,2);
      GroupAndSend(names+2,values,2);
    }

    if (canId == 0xCFFF348 ) {

      String names[] = {"a5", "a6", "a7", "a8"}; //analog 5-8

      int values[4];
      fillValueArray(buf, values, 2);
      GroupAndSend(names, values, 2);
      fillValueArray(buf+4,values,2);
      GroupAndSend(names+2,values,2);

    }

    if (canId == 0xCFFF448 ) {

      String names[] = {"f1", "f2", "f3", "f4"};  //frequency 1-4

      int values[4];
      fillValueArray(buf, values, 2);
      GroupAndSend(names, values, 2);
      fillValueArray(buf+4,values,2);
      GroupAndSend(names+2,values,2);
      

    }

    if (canId == 0xCFFF548 ) {

      String names[] = {"bat", "atmp", "ctmp"}; //"battery", "airtemp", "coolanttemp"

      int values[3];
      fillValueArray(buf, values, 3);

      GroupAndSend (names, values, 3);
    }

    if (canId == 0xCFFF648 ) {
      String names[] = {"t1", "t2"}; //thermistor 1-2

      int values[2];
      fillValueArray(buf, values, 2);

      GroupAndSend(names,values,2);

    }
  }
}