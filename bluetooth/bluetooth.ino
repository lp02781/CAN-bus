
#include <SoftwareSerial.h>

int rx_pin = 6;
int tx_pin = 7;
SoftwareSerial mySerial(rx_pin, tx_pin);

void setup() {
  Serial.begin(9600);
  Serial.println("Goodnight moon!");

  mySerial.begin(38400);
  mySerial.println("Hello, world?");
}

void loop() { 
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}
