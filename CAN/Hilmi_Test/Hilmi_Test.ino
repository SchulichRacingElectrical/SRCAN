#include <mcp_can.h>
#include <SPI.h>

const int SPI_CS_PIN = 10;
unsigned int received = 0;

MCP_CAN CAN(SPI_CS_PIN);                                


void setup() {
  Serial.begin(1000000);
  while (CAN_OK != CAN.begin(CAN_250KBPS))              // init can bus : baudrate = 500k
  {
    Serial.println("CAN BUS Shield init fail");
    Serial.println(" Init CAN BUS Shield again");
    delay(100);
  }

  Serial.println("CAN BUS Shield init ok!");

}

void loop() {

  String names[] = {"rpm", "tps", " fueltime", "ignitionangle"};
  int values[4];
  for (int i = 0; i < 4; i++){
    values[i] = random(0,200);                                   // can change to change range of numbers for the values
  }
  
  String masterstring = "{";
  
  for (int i = 0; i < 4 ; i++) {
    masterstring = masterstring + names[i] + ":";
    masterstring += String(values[i]);  
    if (i != 3){
    masterstring += ",";
    }
  }
  
  Serial.print(masterstring + "}");

  delay(4);

}
