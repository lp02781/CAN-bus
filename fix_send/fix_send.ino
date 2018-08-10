#include<dht.h>  
#include <mcp_can.h>
#include <SPI.h>

int trigPin = A0;   
int echoPin = A1;   
long duration, cm, inches;
  
dht DHT; 
int dht_dpin = 7;
int temp, hum;

int ldr = A2;
int light;

int lm35 = A3;
int suhu;

unsigned char len = 0;
unsigned char buff[8];
unsigned long canId;

unsigned int can_id = 0x70;
int SPI_CS_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN);  
unsigned char stmp[8] = {0, 1, 2, 3, 4, 5, 6, 7}; 

int time_now;
int number = 0;

int led1 = 5;
int led2 = 6;

void setup() 
{ 
 Serial.begin(9600); 
 while (CAN_OK != CAN.begin(CAN_500KBPS)){
    Serial.println("CAN BUS Shield init fail");
    Serial.println(" Init CAN BUS Shield again");
    delay(100);
 }
 Serial.println("CAN BUS Shield init ok!");
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(ldr, INPUT);
 pinMode(lm35, INPUT);
 pinMode(led1, OUTPUT);
 pinMode(led2, OUTPUT);
 
 delay(1000); 
} 

void loop() 
{ 
  time_now = millis()/1000;
  
  can_recv();
  serial_recv();
  Serial.println();
  Serial.println();
  
  get_dht();
  get_ldr_lm35();  
  ultrasonic_sensor();
  can_send();
  serial_send();
  Serial.println();
  Serial.println();
  number++;
  delay(1500);
}

void can_send(){
  stmp[0] = number;
  stmp[1] = time_now;
  stmp[2] = temp;
  stmp[3] = hum;
  stmp[4] = inches;
  stmp[6] = cm;
  stmp[7] = light;
  stmp[8] = suhu;
  CAN.sendMsgBuf(can_id, 0, 8, stmp);
}

void serial_send(){
  Serial.print("can id : ");
  Serial.println(can_id);
  Serial.println("number\t time\t temp\t hum\t inches\t cm\t light\t suhu\t"); 
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
  Serial.println("\t");
}

void get_dht(){ 
  DHT.read11(dht_dpin);    
  temp=DHT.temperature; 
  hum = DHT.humidity;
}

void get_ldr_lm35(){
  light = analogRead(ldr);
  suhu = analogRead(lm35);
}

void ultrasonic_sensor(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  cm = (duration/2) / 29.1;
  inches = (duration/2) / 74;
}

void can_recv(){
 if(CAN_MSGAVAIL == CAN.checkReceive()){
        CAN.readMsgBuf(&len, buff);    
        canId = CAN.getCanId();
  }
}

void receiver_action(){
  if(buff[2] > 80){
    digitalWrite(led1,HIGH);
    digitalWrite(led2,LOW);
  }
  else if(buff[2] > 140){
    digitalWrite(led1,HIGH);
    digitalWrite(led2,HIGH);
  }
  else{
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
  }
}

void serial_recv(){
  Serial.println("-----------------------------");
  Serial.print("get data from ID: 0x");
  Serial.println(canId, HEX);
        
  Serial.println("number\t time\t potensio\t null\t null\t null\t null\t null\t");
  for(int i = 0; i<len; i++){
    Serial.print(buff[i]);
    Serial.print("\t");
    receiver_action();
  }
}

