#include <mcp_can.h>
#include <SPI.h>

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
  Serial.print(masterstring);
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
  Serial.println();
}


void setup() {

  Serial.begin(115200);

  while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
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

      String names[] = {"rpm", "tps", " fueltime", "ignitionangle"};

      int values[4];
      fillValueArray(buf, values, 4);

      GroupAndSend (names, values, 4);
    }

    if (canId == 0x148) {

      String names[] = {"baro", "map", "lambda"};

      int values[3];
      fillValueArray(buf, values, 3);

      GroupAndSend(names, values, 3);
    }

    if (canId == 0x248) {

      String names[] = {"analog1", "analog2", "analog3", "analog4"};

      int values[4];
      fillValueArray(buf, values, 4);

      GroupAndSend(names, values, 4);
    }

    if (canId == 0x348 ) {

      String names[] = {"analog5", "analog6", "analog7", "analog8"};

      int values[4];
      fillValueArray(buf, values, 4);

      GroupAndSend(names, values, 4);

    }

    if (canId == 0x448 ) {

      String names[] = {"frequency1", "frequency2", "frequency3", "frequency4"};

      int values[4];
      fillValueArray(buf, values, 4);

      GroupAndSend(names, values, 4);

    }

    if (canId == 0x548 ) {

      String names[] = {"battery", "airtemp", "coolanttemp"};

      int values[3];
      fillValueArray(buf, values, 3);

      GroupAndSend (names, values, 3);
    }

    if (canId == 0x648 ) {
      String names[] = {"thermistor1", "thermistor2"};

      int values[2];
      fillValueArray(buf, values, 2);

    }
  }
}
