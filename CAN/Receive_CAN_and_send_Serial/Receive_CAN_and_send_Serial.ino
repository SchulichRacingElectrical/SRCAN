#include <mcp_can.h>
#include <SPI.h>

const int SPI_CS_PIN = 10;
unsigned int received = 0;

MCP_CAN CAN(SPI_CS_PIN);

int CombineBytes (int high, int low) {
  int num = high * 256 + low;
  if (num > 32767) {
    num = num - 65536;
  }
  return num;
}

void setup() {
  Serial.begin(1000000);

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
  String masterstring;

  if (CAN_MSGAVAIL == CAN.checkReceive())           // check if data coming
  {
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
    int rpm = CombineBytes(buf[0],buf[1]);
    masterstring = "{rpm:" + (String)rpm + "}";
    Serial.print(masterstring);
  }
}
