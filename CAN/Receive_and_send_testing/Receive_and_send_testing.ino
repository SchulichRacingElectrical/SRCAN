/* SUMMARY OF TESTING:
   - both arduinos were able to send and receive
   - arduino not made by Arduino Company was not sending "t" but was receiving it so second arduino is most likely processing data faster
     or second arduino is not seeing that there is a message available
*/

#include <mcp_can.h>
#include <SPI.h>

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;
unsigned int received = 0;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

void setup()
{
  Serial.begin(115200);

  while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
  {
    Serial.println("CAN BUS Shield init fail");
    Serial.println(" Init CAN BUS Shield again");
    delay(100);
  }
  Serial.println("CAN BUS Shield init ok!");

}
unsigned char msgreceivedmsg[] = {"a"};
unsigned char nomsgreceived[] = {"t"};


void loop() {
  unsigned char len = 0;
  unsigned char buf[8];

  if (CAN_MSGAVAIL == CAN.checkReceive())           // check if data coming
  {
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

    unsigned long canId = CAN.getCanId();

    Serial.println("-----------------------------");
    Serial.print("get data from ID: 0x");
    Serial.println(canId);

    for (int i = 0; i < len; i++) // print the data
    {
      if ( buf[i] != '\0') {
        Serial.print((char)buf[i]);
        Serial.print("\t");
      }
    }
    Serial.println();

    CAN.sendMsgBuf(41, 0, 8, msgreceivedmsg);
  }
  else
  {
    CAN.sendMsgBuf(71, 0, 8, nomsgreceived);
    Serial.print("sent");

  }

}
