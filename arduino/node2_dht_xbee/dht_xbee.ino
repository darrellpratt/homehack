#include <SoftwareSerial.h>


#include "DHT.h"
#define DHTTYPE DHT22   // DHT 22  (AM2302)

#define DHTPIN 4     // what pin we're connected to

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor


SoftwareSerial mySerial =  SoftwareSerial(2, 3);
char ch;

int photocellPin = 0;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider
DHT dht(DHTPIN, DHTTYPE);

void setup(void) 
{
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  //Serial.println("Goodnight moon!");
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  dht.begin();
}

void loop(void) 
{
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = (t* 9 +2)/5+32;

  //if (Serial.available()) 
  if (mySerial.available()) 
  {
     ch = mySerial.read();
     //ch = Serial.read();

     if ( ch == '2' ) { 
       mySerial.print("node=2 temp=");
       mySerial.print(f);
       mySerial.print(" humidity=");
       mySerial.print(h);  // if '2' is received, then send back analog read A0
       mySerial.print(" light=");
       photocellReading = analogRead(photocellPin);
       mySerial.print(photocellReading);
       mySerial.print("\n");

 
       //Serial.print(sensors.getTempFByIndex(0)); // if '1' is received, then send back analog read A0
      } 
       delay(1);
  }
  
  if (Serial.available()) 
  //if (mySerial.available()) 
  {
     //ch = mySerial.read();
     ch = Serial.read();

     if ( ch == '2' ) { 
       Serial.print("node=2 temp=");
       Serial.print(f);
       Serial.print(" humidity=");
       Serial.print(h);  // if '2' is received, then send back analog read A0
       Serial.print(" light=");
       photocellReading = analogRead(photocellPin);
       Serial.print(photocellReading);
       } 
       delay(1);
  }
  
}
