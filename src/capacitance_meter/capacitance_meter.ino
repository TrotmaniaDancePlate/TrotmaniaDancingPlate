#define CHARGE 2
#define DISCHARGE 3
#define SENSOR A0

void setup() {
  pinMode(CHARGE, INPUT);
  digitalWrite(CHARGE, LOW);
  Serial.begin(9600);
}

void loop() {
    //Record the start time with millis()
    long start = millis();
    //Set charge pin to OUTPUT and make it HIGH
    pinMode(CHARGE, OUTPUT);
    digitalWrite(CHARGE, HIGH);
    //Check the voltage repeatedly in a loop until it gets to 63.2% of total voltage.
    while(analogRead(SENSOR) < 648){}

    //Report the value with serial.print
    Serial.println(millis()-start);

    
    digitalWrite(CHARGE, LOW);
    pinMode(DISCHARGE, OUTPUT);
    digitalWrite(DISCHARGE, LOW); 
    while(analogRead(SENSOR) > 0){}

    pinMode(DISCHARGE, INPUT);
}
