

/*Testing the HM-10 BLE Serial Device. */

#include <AltSoftSerial.h>
//#include <SoftSerial.h>

AltSoftSerial bluetoothSerial;

boolean NL = true;
const String cmd;
const char* open="1421";
bool gateIsOpen=true;
void setup() {

Serial.begin(9600);

while (!Serial) ;
Serial.println("-------Starting BLE ---------");

bluetoothSerial.begin(9600);

}

void loop() {

char Serialdata;


if (Serial.available()) {

Serialdata = Serial.read();

//bluetoothSerial.print(Serialdata);  // causes double letters in phone ble

if (Serialdata!=10 & Serialdata!=13 & Serialdata!='x' ){

bluetoothSerial.write(Serialdata);

}
if ( Serialdata == 'x'){
  //Serial.println(String(cmd));
  //Serial.println("cmd " + String(cmd));
  Serial.println("Open the Gate!");

}

//if (NL) { Serial.print("\r\n>"); NL = false; }
if (NL) { Serial.print("\n>"); NL = false; }

Serial.write(Serialdata);

if (Serialdata==10) { NL = true; }
}


cmd="";
if (bluetoothSerial.available()) {
cmd = bluetoothSerial.readStringUntil('.');   
Serial.print(cmd);
if(cmd == open){
  if (gateIsOpen){
    Serial.println(" Open the Gate!");
    delay(10000);
    Serial.println("Closing the Gate");
  }else {
    Serial.println(" Close the Gate!");
    gateIsOpen = !gateIsOpen;
  }
}
}
}















