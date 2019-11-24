#define CHARGE 2
#define DISCHARGE 3
#define SENSOR A0

// declarations for SW setup
//   allocate memory for a two byte array
byte* retval = malloc(sizeof(byte)*2);
byte sync_counter = 0;

void setup() {
  pinMode(CHARGE, INPUT);
  digitalWrite(CHARGE, LOW);
  Serial.begin(9600);
}

void loop() {
    //Record the start time with millis()
    long start = micros();
    //Set charge pin to OUTPUT and make it HIGH
    pinMode(CHARGE, OUTPUT);
    digitalWrite(CHARGE, HIGH);
    //Check the voltage repeatedly in a loop until it gets to 63.2% of total voltage.
    while(analogRead(SENSOR) < 648){}

    //Report the value with serial.print
    //writeValue(millis()-start);

    Serial.println(micros()-start);
    digitalWrite(CHARGE, LOW);
    pinMode(DISCHARGE, OUTPUT);
    digitalWrite(DISCHARGE, LOW); 
    while(analogRead(SENSOR) > 0){}

    pinMode(DISCHARGE, INPUT);
}

void writeValue(int value) {
  Serial.write(tobyte(value, retval),2);
  Serial.flush();

  sync_counter++;
  if (sync_counter == 128) {
    Serial.write(tobyte(0xFCFC, retval),2);
    Serial.flush();
    sync_counter = 0;
  }
}

byte* tobyte(unsigned int input, byte* retval) {
  retval[0] = (byte) (input >> 8);
  retval[1] = (byte) (input);
  
  return retval;
}
