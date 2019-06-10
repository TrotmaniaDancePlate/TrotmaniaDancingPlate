const int PIN_LED = 2;
const int PIN_SENSOR = A7;

int avg = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  digitalWrite(PIN_LED, HIGH);
  delayMicroseconds(100);
  int value = analogRead(PIN_SENSOR);
  digitalWrite(PIN_LED, LOW);

  avg = (avg*9+value)/10;
  Serial.println(avg);
  delay(10);
}
