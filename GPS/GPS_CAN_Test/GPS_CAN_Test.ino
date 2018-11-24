#include <mcp_can.h>
#include <SPI.h>

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;
unsigned int received = 0;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin


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
  unsigned char data[56];
  int j =8;
  do{
  if (CAN_MSGAVAIL == CAN.checkReceive())           // check if data coming
  
  
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
    
  
  }while(buf[0] != 16 && buf[1] != 69);

  for (int i=0; i< 8 ;i++){
    data[i] = buf[i];
  }

  for (int i=0; i< 6;i++){
    while(CAN_MSGAVAIL != CAN.checkReceive()){}
    CAN.readMsgBuf(&len,buf);
    for(int i =0; i< 8;i++,j++){
      data[j] = buf[i];
    }
  }

  long int Zaccel = (data[28]<<8) | data[29];
  Zaccel = (Zaccel<<8)| data[30];
  Zaccel = (Zaccel<<8)| data[31];
  double Zaccels = Zaccel *(0.200/65536)/125/8.1 ;

  long int Xaccel = (data[36]<<8) | data[37];
  Xaccel = (Xaccel<<8)| data[38];
  Xaccel = (Xaccel<<8)| data[39];
  double Xaccels = Xaccel *(0.200/65536)/125 /8.1;
  Serial.print(Xaccels);
  Serial.print("\t");
  Serial.print(Zaccels);
  Serial.println();
}
