/*
esp_stepper_limit_switch_sketch_aug21.ino
Testing the reflective ir limit switch. 
When white band is under the ir detector, voltage will rise from 0.03 v to 3.0 V
This will trigger the ISR's listening on pin 12 and pin 14.
*/

// Define connections and variables

#define EXPAND_LIMIT_PIN  12
#define CONTRACT_LIMIT_PIN  14


// Variables
int expandLimit = 1;
int contractLimit= 0;
volatile int limit = -1;  // -1 no limit; 0 contract limit; 1 expandLimit
unsigned long tim;
unsigned long start;

String whchLimit[2] = { "Contract", "Expand" };


// Interrupt Handlers

void expandLimitHit() {
  limit = expandLimit;
  tim = micros() - start;
}

void contractLimitHit() {
  limit = contractLimit;
  tim = micros() - start;
}


void setup() {
  pinMode(EXPAND_LIMIT_PIN, INPUT_PULLDOWN); 
  pinMode(CONTRACT_LIMIT_PIN, INPUT_PULLDOWN);
  Serial.begin(115200);
  Serial.println("=====Starting Limit Switch Test Sketch==== time in micros: ");

  // Attach interrupt pins to handlers
  attachInterrupt(digitalPinToInterrupt(EXPAND_LIMIT_PIN), expandLimitHit, RISING);
  attachInterrupt(digitalPinToInterrupt(CONTRACT_LIMIT_PIN), contractLimitHit, RISING);
  // digitalWrite(EXPAND_LIMIT_PIN, LOW);  // waiting for ISR to set it high.
  // digitalWrite(CONTRACT_LIMIT, LOW); // Both are INPUT pins, I cannot set it
  start = micros();
  tim=start;
  Serial.println(tim);
}

void loop() {
  /*
  Serial.print(micros() - start);
  Serial.print( "  " ) ;
  Serial.println(analogRead(EXPAND_LIMIT_PIN));
  */
  if (limit > -1) {
    Serial.print(whchLimit[limit]);
    Serial.print(" Limit was hit at time secs: ");
    Serial.println(tim/1e6);
    delay(1500);
    limit = -1;  // reset for next limit.
  }
}