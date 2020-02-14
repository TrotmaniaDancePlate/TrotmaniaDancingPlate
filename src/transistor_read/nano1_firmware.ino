// 12.06.2019 - .. ; reused 04.01.2020 - 07.01.2020
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

// Definitions for SW
//#define DE_INIT 0
#define ENMIC *(1000*1000) // prepare value to avoid float operations
#define DEMIC /(1000*1000) // recover working value

// declarations for SW setup
//   allocate memory for a two byte array
byte* retval = (byte*)malloc(sizeof(byte)*2);

typedef enum de_states{de_init, de_idle, de_touch, de_enter, de_press, de_jumpup, de_impact} de_state;
typedef enum de_lpfmodes{de_STOP, de_ACTIVE, de_BOOST} de_lpfmode;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_STATE0, OUTPUT);
  pinMode(PIN_STATE1, OUTPUT);
}

void loop() {
  unsigned long micros_prev = 0;    // to store time stamp
  static byte sync_counter = 0;

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
    case de_idle:
      digitalWrite(PIN_STATE0, HIGH);
      digitalWrite(PIN_STATE1, HIGH);
      break;
    case de_enter:
      digitalWrite(PIN_STATE0, LOW);
      digitalWrite(PIN_STATE1, LOW);
      break;
    case de_touch:
      digitalWrite(PIN_STATE0, HIGH);
      digitalWrite(PIN_STATE1, LOW);
      break;
    case de_press:
      digitalWrite(PIN_STATE0, LOW);
      digitalWrite(PIN_STATE1, HIGH);
      Serial.write(tobyte(0x0020, retval),2); Serial.flush();  // just for DEBUG!
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


byte* tobyte(int input, byte* retval) {
  retval[0] = (byte) (input >> 8);
  retval[1] = (byte) (input);
  
  return retval;
}

/* Function delay_to_recover
 *   - Tested on: 07.01.2020
 *   - "71 minutes test" with oscilloscope: OK
 */
boolean delay_to_recover(unsigned long L_micros_prev)
{
  #define MICROS_DELTA L_micros_prev  // to stint one 32bit variable

  // Overflow of micros is considered here but does not need to
  //  be handled due to the magical effect of two's complement  ;)
  MICROS_DELTA = micros() - L_micros_prev;
  if ( MICROS_DELTA > RECOVER_MU ){
    // Send error signal if timing is outran
    return false;
  }
  else {
    delayMicroseconds( RECOVER_MU - MICROS_DELTA );
    return true;
  }
}

de_state dance_event(int raw_value)
{
  #define NEW_lpf_value_micro lpf_value_prev_micro  // to stint one 32bit variable
  #define ENTER_THRESHOLD 15 // ground level has to be exceeded by this value to detect touch
  #define GROUND_LEVEL 183   // preliminary value for ground level (200)
  #define TOUCH_TICKS 100    // Number of samples to wait before reject touch and detect enter
  const int lpf_param_p_micro = 5000; // corresponding to 0.005 (ENMIC)
  
  static de_state state = de_init;
  static int raw_value_prev;
  static int lpf_value_prev_micro;
  static byte touch_ticks_counter = 0;
  
  int delta_value;
  int lpf_value;
  //de_tpmode mode = de_STOP;
  //int lpf_dyn_param_p_micro;
  
  // state effect for init and low-pass filter
  switch (state)
  {
    case de_init:
      // Initialize previous values with current values
      raw_value_prev = raw_value;
      lpf_value_prev_micro = ((int)raw_value ENMIC);

      // Leave initialization state
      state = de_idle;
      break;
    
    case de_enter:
      // Normal low-pass filter operation
      NEW_lpf_value_micro = lpf_value_prev_micro + lpf_param_p_micro *
                        (((int)raw_value ENMIC) - lpf_value_prev_micro);
      lpf_value = (int)(NEW_lpf_value_micro DEMIC);
      lpf_value_prev_micro = NEW_lpf_value_micro;
      break;
      
    case de_touch:
      // Faster low-pass filter operation (boost with x10)
      NEW_lpf_value_micro = lpf_value_prev_micro + lpf_param_p_micro * 10 *
                        (((int)raw_value ENMIC) - lpf_value_prev_micro);
      lpf_value = (int)(NEW_lpf_value_micro DEMIC);
      lpf_value_prev_micro = NEW_lpf_value_micro;
      break;
      
    default:
      // freeze low-pass filter in all other states (idle and jump)
      lpf_value = (int)(lpf_value_prev_micro DEMIC);
      break;
  }

  delta_value = raw_value - raw_value_prev;

  // State effect for event detection and state changes
  switch (state)
  {
    case de_idle:
      if (raw_value > (GROUND_LEVEL + ENTER_THRESHOLD+4)){
        touch_ticks_counter = 0;
        state = de_touch;
      }
      break;
    case de_enter:
      if (raw_value < (GROUND_LEVEL + ENTER_THRESHOLD-4)){
        state = de_idle;
      }
      break;
    case de_touch:
      touch_ticks_counter++;
      if (raw_value < (GROUND_LEVEL + ENTER_THRESHOLD-4)){
        state = de_press;
      }
      else {
        if (touch_ticks_counter >= TOUCH_TICKS) {
          state = de_enter;
        }
      }
      break;
    
    case de_press:
      state = de_idle;
      break;
    default:
      break;
  }

  // Store raw_value for next delta_value calculation
  raw_value_prev = raw_value;
  return state;
}
