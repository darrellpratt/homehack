#include <RCSwitch.h>
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/////////////////////////////
//VARS
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 3;        

//initialize variables for sensor pins
int moistPin1 = 2;
int moistPin2 = 3;
int moistPin3 = 4;


//initialize variables to store readings from sensors
int moistVal1 = 0;
int moistVal2 = 0;
int moistVal3 = 0;


//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 5000;  

char ch;
char motion;
float temp;
float temp2;

int photocellPin = 1;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider

boolean lockLow = true;
boolean takeLowTime;  

int pirPin = 2;    //the digital pin connected to the PIR sensor's output

SoftwareSerial mySerial(11, 12);


#define ONE_WIRE_BUS 4

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);



RCSwitch mySwitch = RCSwitch();
String content = "";
String content2 = "";

String pulseLength = "";
String code = "";
String inString = "";
char character;
char character2;

char* buf;

void setup() {
  mySwitch.enableTransmit(10);  // Using Pin #10
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(pirPin, INPUT);
  digitalWrite(pirPin, LOW);
  motion = '0';
    // Start up the library
  sensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement


  //give the sensor some time to calibrate
    for(int i = 0; i < calibrationTime; i++){
      delay(1000);
      }
    delay(50);

}
void rfsend(String code){
        if(code == "1off"){
          mySwitch.setPulseLength(185);
          mySwitch.send(1332540, 24);
        }
        if (code == "1on"){
          mySwitch.setPulseLength(185);
          mySwitch.send(1332531, 24);

        }
        if (code == "2on"){
          mySwitch.setPulseLength(185);
          mySwitch.send(1332675, 24);
        }
        if (code == "2off"){
          mySwitch.setPulseLength(185);
          mySwitch.send(1332684, 24);
        }
        if (code == "3on"){
          mySwitch.setPulseLength(185);
          mySwitch.send(1332995, 24);
        }
        if (code == "3off"){
          mySwitch.setPulseLength(185);
          mySwitch.send(1333004, 24);
        }
        
        if (code == "greenon"){
          mySwitch.setPulseLength(465);
          mySwitch.send(1425740, 24);

        }
        if (code == "greenoff"){
          mySwitch.setPulseLength(465);
          mySwitch.send(1425728, 24);
        }
  
}
void loop() {
   
  mySerial.listen();
  sensors.requestTemperatures(); // Send the command to get temperatures
  while (mySerial.available() > 0) { 
      character = mySerial.read();
      content.concat(character);
      inString = content;
      inString = content.substring(0, content.length() - 2);
    }
     //ch = mySerial.read();
     if ((content != "")||(content != "1")||(content != "2")||(content != "3")) {
        rfsend(content);
      }
     
     

     if ( content == "3") { 
       temp = sensors.getTempFByIndex(0);
       photocellReading = analogRead(photocellPin);
       mySerial.print("node=3 motion=");
       mySerial.print(motion);
       mySerial.print(';');
       
       if(digitalRead(pirPin) == HIGH){
         if(lockLow){  
           //makes sure we wait for a transition to LOW before any further output is made:
           lockLow = false;            
           motion='1';
           delay(50);
           }         
           takeLowTime = true;
         }
  
       if(digitalRead(pirPin) == LOW){       
  
         if(takeLowTime){
          lowIn = millis();          //save the time of the transition from high to LOW
          takeLowTime = false;       //make sure this is only done at the start of a LOW phase
          }
         //if the sensor is low for more than the given pause, 
         //we assume that no more motion is going to happen
         if(!lockLow && millis() - lowIn > pause){  
             //makes sure this block of code is only executed again after 
             //a new motion sequence has been detected
             lockLow = true; 
             motion = '0';             
            // Serial.print((millis() - pause)/1000);
             //Serial.println(" sec");
             delay(50);
             }
         }
       temp = sensors.getTempFByIndex(0);
       temp2 = sensors.getTempFByIndex(1);
       //read values from the sensors
       moistVal1 = analogRead(moistPin1);
       moistVal2 = analogRead(moistPin2);
       moistVal3 = analogRead(moistPin3);

       

       mySerial.print("temp="); // if '1' is received, then send back analog read A0
       mySerial.print(temp);
       mySerial.print(";");
       mySerial.print("temp2="); // if '1' is received, then send back analog read A0
       mySerial.print(temp2);
       mySerial.print(";");
       mySerial.print("light=");
       //photocellReading = analogRead(photocellPin);
       //photocellReading = analogRead(photocellPin);
       mySerial.print(photocellReading);
       mySerial.print(";");
       mySerial.print("plant1=");
       //moistVal1 = analogRead(moistPin1);
       //moistVal1 = analogRead(moistPin1);
       mySerial.print(moistVal1);
       mySerial.print(";");
       mySerial.print("plant2=");
       //moistVal2 = analogRead(moistPin2);
       //moistVal2 = analogRead(moistPin2);
       mySerial.print(moistVal2);
       mySerial.print(";");
       mySerial.print("plant3=");
       mySerial.print(moistVal3);
       mySerial.print(";");
       
       

       } 
       delay(1);
       content ="";

  
  
  if (Serial.available()){
    delay(100);
    content = "";
    while (Serial.available() > 0) { 
      character = Serial.read();
      content.concat(character);
      if (content != "") {
        rfsend(content);
      }
      
    }
  }
   

  }

