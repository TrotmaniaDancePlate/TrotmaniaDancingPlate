#include <fc_serial_lib.h>

FcSerial serialLib = FcSerial(10);

// 12.06.2019 - 
// Read and send values from ITR-9904

// declarations for HW setup
const int PIN_LED = 2;
const int PIN_SENSOR = A7;

////int avg = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
}

void loop() {

  // WARNING! IR-LED may be damaged by HIGH > 0.5ms every 10ms.
  digitalWrite(PIN_LED, HIGH);
  delayMicroseconds(100);
  int value = analogRead(PIN_SENSOR);
  digitalWrite(PIN_LED, LOW);

  ////avg = (avg*9+value)/10;
  ////Serial.println(avg);

  serialLib.send(value);
  
  delay(10);
}
