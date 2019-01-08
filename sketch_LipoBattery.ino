#include <RunningStat.h>


/*-------Values set by User------------------------------*/
double aRef = 3.3; //Either 3.3 volts or 5.0 volts. rx[] and ry[] must change accordingly.
const int stages = 4 + 1; // this number controls the conversions, upversions, etc. Always add 1 for the zero stage.
int reps = 25;  //Number of samples to average to get the a2d measurement.
int dlay = 10;  //Number of milliseconds to delay when measuring a new pin.
int a2dRez = 1023; // Assumes 10 bit A2D. If A2D resolution is different, put the appropriate constant in this variable. a2dRez = 2^bits-1
// all 3.3v resistors should be measured on multimeter and exact values should be used in these variables.( shown values are random variables within 5%).
int rx[7] = {0, 509, 279, 99, 360, 360, 680};
int ry[7] = {0, 333,  70, 14,  39,  29,  47};
//Thresholds
double Drained     = 3.0; // whatever level the battery specs say to never pass
double DisCharged  = 3.20; // The level set by the operation as needing charge.
double Charged     = 4.19; // The level set by the operation as meeting charge requirements.
double OverCharged = 4.80; // Whatever level the battery specs say to never exceed. Clamped A2D counts indicates a wiring problem or defective zener. Do not put off inspection.
//Testing items to be commented out in loop() when actual measurements are made. For production use, cnts[] will be replaced by analogRead values in getA2D function.
int cnts[] =   { 0, 512, 512, 484, 509, 486, 505};
/*--------------End of User Values ---------------------*/

double upvert[7] ; // array will be used to convert the voltage divider voltage to the stage voltage. Elements are the inversion of the voltage divider fractions.
double count2Volts = aRef / a2dRez; // factor to multiply a2dCount to get the volts measured.
// first element of following arrays is always 0. remaining array is populated from indices: 1,2, ...,6
int pin[] = {0, A0, A1, A2, A3, A4, A5};
int a2dCnt[7];
double stdDev[7];
double snr[7];
double vdv[7];
double sv[7];
double scv[7];
double pcnt[7];

void computeUpverts( int sz) {
  for (int i = 1; i < sz; i++) {
    upvert[i] = (double)(ry[i] + rx[i]) / ry[i];
    //   Serial.println("\n" + String(i) + " : " + String(upvert[i]));
  }
}
/**
 * fakeA2D function is used to test all but the actual reading of the A2D values. If it is called from loop(), the values for mean, stdDev, snr are bogus.
 */
void fakeA2D(int pins) {
  for (int i=1; i<pins; i++) {
    a2dCnt[i]=cnts[i];
    stdDev[i] = .5;
    snr[i] = a2dCnt[i] / stdDev[i];
    vdv[i] = a2dCnt[i] * count2Volts;
  }
}
/**
   Function getA2D( dlay, reps, pins) will take reps A2D readings for a pin, compute the mean, standardDeviation and SNR for those samples (e.g. reps= 25).
   getA2D(...) also computes the vdv voltages from the A2D mean. This is the voltage across the load resistor in the Voltage Divider.
   If SNR decreases over time, a search for the extra noise should be conducted.
*/
void getA2D( int dlay, int reps, int pins) {

  RunningStat rs;
  int first[7];
  for (int i = 1; i < pins; i++) {
    rs.Clear();   // Start new pin with no samples in rs.
    first[i] = analogRead(pin[i]);  //waste a sample to let A2D approximate the value.

    for (int j = 0; j < reps; j++) {  //collect reps samples for this pin.
      rs.Push(analogRead(pin[i]));
      delay(2);                       // it might be possible to comment out this delay.
    }
    a2dCnt[i] = rs.Mean();
    stdDev[i] = rs.StandardDeviation();
    snr[i] = a2dCnt[i] / stdDev[i];
    vdv[i] = a2dCnt[i] * count2Volts;
    delay(dlay); //delay before starting a new pin.
  }
}


/**
   Function stageVoltages(...) computes and populates the stage voltages, sv[] for stages 1 thru 6
   using the voltage divider voltages * the inversion of the voltage divider fraction (the upvert array).
*/
void stageVoltages( double vdv[], double sv[], int sz) {
  for (int i = 1; i < sz; i++) {
    sv[i] = upvert[i] * vdv[i];
  }
}

/**
   Function stageCellVoltages(...) computes and populates the stage cell voltages array, scv[].
   The voltage for the stage[i] cell is found by subtracting the voltage at stage[i-1] from the voltage at stage[i]
   Percentage is the scv[i]/Charged. Value of Charged is set in config at top.  
*/
void stageCellVoltages(double sv[], double scv[], int sz) {
  for (int i = 1; i < sz; i++) {
    scv[i] = sv[i] - sv[i - 1];
    pcnt[i] = scv[i] / Charged * 100;
  }
}
/**
   function showStatus() displays a row for each stage: stg, count, vdv, sv, scv, status
*/
void showStatus(int stgs) {
  Serial.println("\n----------------------------\n");

  for ( int i = 1; i < stgs ; i++) {
    String msg = "";
    double v = scv[i];
    if ( v <= Drained ) {
      msg = msg +  " Drained: ";
    }
    else if (v > Drained && v <= DisCharged) {
      msg = msg +  " DisCharged: ";
    }
    else if (v > DisCharged && v < Charged  ) {
      msg = msg +  " Charging: ";
    }
    else if (v >= Charged && v < OverCharged  ) {
      msg = msg +  " Charged: ";
    }
    else if (v >= OverCharged ) {
      msg = msg +  " OverCharged: ";
    }
    //Serial.println("\n stage, counts, stdDev, SNR, measured, stage, stageCell, status" ); //print header for status outputs.
    Serial.println( "     " + String(i) + ", " + String(a2dCnt[i]) + ",  " + String(stdDev[i]) + ",  " + String(snr[i]) + ",  " + String(vdv[i]) + ", " + String(sv[i]) + ", " + String(scv[i]) + ", " + msg + " " + pcnt[i] + "%" );
    Serial.println();
  }
  Serial.println("\n----------------------------\n");

}

void showResistors(int stgs) {
  Serial.print("Resistors for aRef: " + String(aRef));
  for (int i = 1; i < stgs; i++) {  // 0 stage has no resistors so start with 1
    Serial.print(" X: " + String(rx[i]) + " Y: " + String(ry[i]));
  }
}

void setup() {
  Serial.begin(9600);
  for (int i = 1; i < stages; i++) {
    pinMode(pin[i], INPUT);
  }
  computeUpverts(stages);
  showResistors(stages);  //uncomment this if you want to see the resistor values in the monitor at startup.
  Serial.println("\n stage, counts, stdDev, SNR, measured, stage, stageCell, status" ); //print header for status outputs.
}

void loop() {
  //fakeA2D(7);  //To test all but the actual A2D measurements, uncomment this line and comment out the next line.
  getA2D(10, 25, stages); //delay 10 ms, reps=25, pins=stages note: pin[0] is 0; pin[1]=A0.
  stageVoltages(vdv, sv, stages);
  stageCellVoltages(sv, scv, stages);
  showStatus(stages);
}
