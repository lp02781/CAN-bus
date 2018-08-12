#include <SPI.h>
#include "mcp_can.h"
#include <SoftwareSerial.h>

const int SPI_CS_PIN = 9;
unsigned char len = 0;
unsigned char buff[8];
unsigned long canId;

unsigned int can_id = 0x72;
MCP_CAN CAN(SPI_CS_PIN);  
unsigned char stmp[8] = {0, 1, 2, 3, 4, 5, 6, 7}; 

int rx_pin = 6;
int tx_pin = 7;
SoftwareSerial mySerial(rx_pin, tx_pin);

int time_now;
int number = 0;

int potensio_input = A1;
int potensio;  

int led1 = 3;
int led2 = 4;
int led3 = 5;

void setup(){
    Serial.begin(9600);
    while (CAN_OK != CAN.begin(CAN_500KBPS)){
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");
    mySerial.begin(38400);
    mySerial.println("bluetooth ready");
    pinMode(potensio_input, INPUT);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
}

void loop(){
    time_now = millis();
    can_recv();
    serial_recv();
    Serial.println();
    Serial.println();
    
    potensio = analogRead(potensio_input);
    can_send();
    serial_send();

    bluetooth_serial();
    Serial.println();
    Serial.println();
    Serial.println();
    number++;
    delay(1500);
}

void receiver_action(){
  if(buff[2] > 29 ){
    digitalWrite(led1,HIGH);
  }
  else{
    digitalWrite(led1,LOW);
  }

  if(buff[4] > 7 ){
    digitalWrite(led2,HIGH);
  }
  else{
    digitalWrite(led2,LOW);
  }

  if(buff[6] > 7 ){
    digitalWrite(led3,HIGH);
  }
  else{
    digitalWrite(led3,LOW);
  }
}

void can_recv(){
 if(CAN_MSGAVAIL == CAN.checkReceive()){
        CAN.readMsgBuf(&len, buff);    
        canId = CAN.getCanId();
  }
}

void serial_send(){
  Serial.println("number\t time\t potensio\t null\t null\t null\t null\t null\t"); 
  Serial.print(stmp[0]); 
  Serial.print("\t");
  Serial.print(stmp[1]);
  Serial.print("\t");
  Serial.print(stmp[2]); 
  Serial.print("\t");
  Serial.print(stmp[3]); 
  Serial.print("\t");
  Serial.print(stmp[4]);
  Serial.print("\t");  
  Serial.print(stmp[5]); 
  Serial.print("\t");
  Serial.print(stmp[6]); 
  Serial.print("\t");
  Serial.print(stmp[7]);
}

void can_send(){
  stmp[0] = number;
  stmp[1] = time_now;
  stmp[2] = potensio;
  CAN.sendMsgBuf(can_id, 0, 8, stmp);
}

void serial_recv(){
  Serial.println("-----------------------------");
  Serial.print("get data from ID: 0x");
  Serial.println(canId, HEX);
        
  Serial.println("number\t time\t temp\t hum\t inches\t cm\t light\t suhu\t"); 
  for(int i = 0; i<len; i++){
    Serial.print(buff[i]);
    Serial.print("\t");
    receiver_action();
  }
}

void bluetooth_serial(){
  mySerial.println("-----------------------------");
  mySerial.print("get data from ID: 0x");
  mySerial.println(canId, HEX);
        
  mySerial.println("number\t time\t temp\t hum\t inches\t cm\t light\t suhu\t"); 
  for(int i = 0; i<len; i++){
    mySerial.print(buff[i]);
    mySerial.print("\t");
    receiver_action();
  }

  mySerial.println("number\t time\t potensio\t null\t null\t null\t null\t null\t"); 
  mySerial.print(stmp[0]); 
  mySerial.print("\t");
  mySerial.print(stmp[1]);
  mySerial.print("\t");
  mySerial.print(stmp[2]); 
  mySerial.print("\t");
  mySerial.print(stmp[3]); 
  mySerial.print("\t");
  mySerial.print(stmp[4]);
  mySerial.print("\t");  
  mySerial.print(stmp[5]); 
  mySerial.print("\t");
  mySerial.print(stmp[6]); 
  mySerial.print("\t");
  mySerial.print(stmp[7]);
}

