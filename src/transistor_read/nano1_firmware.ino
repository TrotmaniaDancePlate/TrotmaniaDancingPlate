// 12.06.2019 - .. ; reused 04.01.2020 - 05.01.2020
// Read and send values from ITR-9904 / ITR-8307

/*
 * Important information regarding target HW and setup!
 * 
 * Switching timing of PIN_LED is selected regarding circuit
 *   "REV: 1.0 ; Date: 2019-06-10 ; Drawn By: chase_0693"
 * 
 * Timer0 Interrupt config conflicts with Arduino delay()
 *   so Arduino micros() is used instead, now!
 * 
 */

// Definitions for HW setup
#define PIN_LED 2 // port pin to switch transistor for IR-LED
#define PIN_SENSOR A7 // port pin to read photo transistor
#define RECOVER_MU 9800 // recover time of IR-LED in micros

#define PIN_STATE0 4
#define PIN_STATE1 5

// declarations for SW setup
//   allocate memory for a two byte array
byte* retval = (byte*)malloc(sizeof(byte)*2);

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_STATE0, OUTPUT);
  pinMode(PIN_STATE1, OUTPUT);
}

void loop() {
  unsigned long micros_prev = 0;    // to store time stamp
  byte sync_counter = 0;

  // - switch IR-LED on, read sensor, switch IR-LED off
  // WARNING! IR-LED may be damaged by HIGH > 0.5ms every 10ms.
  digitalWrite(PIN_LED, HIGH);
  delayMicroseconds(100);
  int value = analogRead(PIN_SENSOR);
  digitalWrite(PIN_LED, LOW);
  micros_prev = micros(); // store time stamp of LED switch off
  // WARNING! The previous lines should only be changed with care!
  
  // - send sensor data
  Serial.write(tobyte(value, retval),2);
  Serial.flush();

  // - send synchronization pattern
  sync_counter++;
  if (sync_counter == 128) {
    Serial.write(tobyte(0xFCFC, retval),2);
    Serial.flush();
    sync_counter = 0;
  }

  switch (dance_event(value))
  {
    case 0: // FC / idle level
      digitalWrite(PIN_STATE0, HIGH);
      digitalWrite(PIN_STATE1, HIGH);
      break;
    case 1: // FD / enter level
      digitalWrite(PIN_STATE0, LOW);
      digitalWrite(PIN_STATE1, LOW);
      break;
    case 2: // FE / activation detected
      digitalWrite(PIN_STATE0, HIGH);
      digitalWrite(PIN_STATE1, LOW);
      break;
    case 3: // FF / jump detected
      digitalWrite(PIN_STATE0, LOW);
      digitalWrite(PIN_STATE1, HIGH);
      break;
    default: // Error state
      break;
  }

  // The rest of this main loop should ensure that the IR-LED has
  //  enough time to recover independent from program load
  delay(4); // this line is probably useless (just for reassurance)

  // WARNING! Function delay_to_recover has to be called at the
  //  end of the main loop to ensure proper IR-LED switch timing!
  if (false == delay_to_recover(micros_prev)){
    digitalWrite(LED_BUILTIN, HIGH);
  }
}


byte* tobyte(unsigned int input, byte* retval) {
  retval[0] = (byte) (input >> 8);
  retval[1] = (byte) (input);
  
  return retval;
}

/* Function delay_to_recover
 *   - Tested on: 05.01.2020
 *   - "71 minutes test" with oscilloscope: OK
 */
boolean delay_to_recover(unsigned long L_micros_prev)
{
  unsigned long micros_val = 0; // to hold time stamp for several operations
  #define MICROS_DELTA L_micros_prev  // to stint one 32bit variable
  
  micros_val = micros();
  // Ignore time stamp if overflow of micros() occured
  if ( micros_val < L_micros_prev )
  {
    delayMicroseconds(RECOVER_MU); // Wait for IR-LED to recover
    //digitalWrite(PIN_STATE1, HIGH); // DEBUG: Only for 71min-Test
  }
  else
  {
    MICROS_DELTA = micros_val - L_micros_prev;
    if ( MICROS_DELTA > RECOVER_MU ){
      // Send error signal if timing is outran
      return false;
    }
    else{ // only if (RECOVER_MU - MICROS_DELTA) is positive
      delayMicroseconds( RECOVER_MU - MICROS_DELTA );
    }
  }
  return true;
}

byte dance_event(unsigned int value)
{
  byte state = 0;
  
  return state;
}
