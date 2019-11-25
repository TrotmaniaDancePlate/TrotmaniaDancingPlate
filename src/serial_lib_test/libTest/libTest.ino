#include <fc_serial_lib.h>

FcSerial serialLib = FcSerial(10);

void setup() {
    Serial.begin(9600);
}

void loop() {
  serialLib.send(random(0, 1024));
  delay(100);
}
