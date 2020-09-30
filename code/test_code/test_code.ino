#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>

SoftwareSerial SIM900(9, 10);
TinyGPS gps;  //Creates a new instance of the TinyGPS object


PulseSensorPlayground pulseSensor;

const int PulseWire = 0;
const int LED13 = 13;
int Threshold = 550;
float val;
int a = 0;
int b = 0;
float latitude =11.499198;
float longitude=77.276666;


void setup()
{
  Serial.begin(9600);
  SIM900.begin(9600);
  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED13);
  pulseSensor.setThreshold(Threshold);
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");
  }

}


void sendSMS()
{  
    SIM900.print("AT+CMGF=1\r"); 
    delay(400);
    SIM900.println("AT + CMGS = \"+919842513373\"");// recipient's mobile number with country code
    delay(300);
    SIM900.println("SUSHMITHA"); 
    SIM900.print("Temperature = ");SIM900.print(val);
    SIM900.print("    BPM = ");SIM900.println(b);
    SIM900.print("Latitude = "); SIM900.print(latitude,6);
    SIM900.print("    Longitude = ");SIM900.print(longitude,6);
    delay(200);
    SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26
    delay(200);
    SIM900.println();
}


void loop()
{

  val = analogRead(1);
  val = ( val * 500) / 1024;
  Serial.print("TEMPRATURE = ");
  Serial.print(val);
  Serial.println("*C");
  a = val;

  int myBPM = pulseSensor.getBeatsPerMinute();
  if (pulseSensor.sawStartOfBeat()) {
    Serial.print("BPM: ");
    Serial.println(myBPM);
    b = myBPM;
  }
  if(a>100 || b>85 ||(a<90 || b<60) )
  {
     sendSMS();
  }
 
 
  delay(1000);
  Serial.println();
}
