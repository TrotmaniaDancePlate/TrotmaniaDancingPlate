// 12.06.2019 - 
// Read and send values from ITR-9904

// declarations for HW setup
const int PIN_LED = 2;
const int PIN_SENSOR = A7;

// declarations for SW setup
//   allocate memory for a two byte array
byte* retval = malloc(sizeof(byte)*2);
byte sync_counter = 0;

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

  Serial.write(tobyte(value, retval),2);
  Serial.flush();

  sync_counter++;
  if (sync_counter == 128) {
    Serial.write(tobyte(0xFCFC, retval),2);
    Serial.flush();
    sync_counter = 0;
  }
  
  delay(10);
}


byte* tobyte(unsigned int input, byte* retval) {
  retval[0] = (byte) (input >> 8);
  retval[1] = (byte) (input);
  
  return retval;
}
