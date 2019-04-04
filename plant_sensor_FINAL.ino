/*
Arduino code used in plant sensor. Uses a few user defined functions to simplify code. In short, it collects 
thresholds from user through serial input, then produces output by comparing user thresholds to readings from 
capacative soil sensor and thermometer. Output is data sent to another arduino using digitalWrite high and low 
signals. Also controls a relay connected to CAP_LOW_PIN. Can be connected to LEDs instead of other arduino.
Relies on Adafruit seesaw library, not included with arduino IDE.
*/
#include"Adafruit_seesaw.h"

Adafruit_seesaw ss;                                         //needed for soil sensor.
const int MIN=0,MAX=1;                                      //used when indexing threshold arrays.
const int TEMP_LOW_PIN=5,TEMP_HIGH_PIN=6;                   //assigns pins to be used later.
const int CAP_HIGH_PIN=7,CAP_LOW_PIN=8;                     //no reason why specific numbers were picked.
const int RELAY_CONTROL=9;                                  //pin used to control relay.
float tempThreshold[2],capThreshold[2];                     //creates threshold arrays.
String minMax[2]={"Minimum","Maximum"};                     //array of strings used for min max prompt.
bool cap_read_disable;                                      //used to disable or enable capacative reading
int counter=0;                                              //used to disable or enable capacative reading
                                                            
void setup(){
  ss.begin(0x36);                                           //connects soil sensor.

  pinMode(TEMP_LOW_PIN,OUTPUT);                             //enables these pins to be output.
  pinMode(TEMP_HIGH_PIN,OUTPUT);                            //will be used to send signals to mega.
  pinMode(CAP_LOW_PIN,OUTPUT);
  pinMode(CAP_HIGH_PIN,OUTPUT);
  pinMode(RELAY_CONTROL,OUTPUT);
  
  getTempThreshold();                                       //this function is defined below.

  getCapThreshold();                                        //this function is also defined below.
  
  Serial.begin(9600);                                       //needs restarted after user functions.
}

void loop(){
  uint16_t capReading = ss.touchRead(0);                    //these two lines get the temperature
  float tempF=(((ss.getTemp())*(9.0/5.0))+27);              //and cap reading from the sensor.

  tempCheck(tempF);                                         //temp check function defined below

  capCheck(capReading);

  printData(tempF,capReading);

  delay(1000);                                              //waits a second to restart loop
}

void getTempThreshold(){                                    //function used in setup.
  for(int i=0;i!=2;i++){                                    //this for loop populates the temp
    Serial.begin(9600);                                     //threshold array, with min and max already
    Serial.print("What is your ");                          //being defined. Restarts serial every time so
    Serial.print(minMax[i]);                                //serial.available function works correctly.
    Serial.println(" temperature?");                        //prompts user then parses float from serial
    while(!Serial.available()){}                            //input and places it in appropriate index
    tempThreshold[i]=Serial.parseFloat();                   //in array.
    Serial.end();
  }
}

void getCapThreshold(){                                     //function used in setup.
  for(int i=0;i!=2;i++){                                    //same process as getTempThreshold, but with
    Serial.begin(9600);                                     //cap values instead of temp values.
    Serial.print("what is your ");
    Serial.print(minMax[i]);
    Serial.println(" cap reading?");
    while(!Serial.available()){}
    capThreshold[i]=Serial.parseFloat();
    Serial.end();
  }
}

void tempCheck(float tempF){                                //function used to check temperature in main.
  if(tempF>tempThreshold[MAX]){                             //if branch executes if temp is over max.
    digitalWrite(TEMP_HIGH_PIN,HIGH);                       //sets temp high pin to high.
    digitalWrite(TEMP_LOW_PIN,LOW);
  }
  else if(tempF<tempThreshold[MIN]){                        //if branch executes if temp is below min.
    digitalWrite(TEMP_HIGH_PIN,LOW);                        //sets temp low pin to high.
    digitalWrite(TEMP_LOW_PIN,HIGH);
  }
  else{                                                     //if branch executes if in range.
    digitalWrite(TEMP_HIGH_PIN,LOW);                        //sets both pins to low.
    digitalWrite(TEMP_LOW_PIN,LOW);
  }
}

void capCheck(uint16_t capReading){                        //function used to check cap reading in main.


  if(cap_read_disable){
    counter++;
    if(counter==2){
      digitalWrite(RELAY_CONTROL,LOW);
      digitalWrite(CAP_LOW_PIN,LOW);
    }
    if(counter==200){
      cap_read_disable=false;
      counter=0;
    }
    return;
  }
  if(capReading>capThreshold[MAX]){                        //if branch executes if cap is over max.
    digitalWrite(CAP_HIGH_PIN,HIGH);                       //sets cap high pin to high.
    digitalWrite(CAP_LOW_PIN,LOW);                         //used to detect overwatering.
    digitalWrite(RELAY_CONTROL,LOW);
    cap_read_disable=false;
  }
  else if(capReading<capThreshold[MIN]){                   //if branch executes if cap is below min.
    digitalWrite(CAP_HIGH_PIN,LOW);                        //sets cap low pin to high.
    digitalWrite(CAP_LOW_PIN,HIGH);   
    digitalWrite(RELAY_CONTROL,HIGH);                      //this will activate relay in circuit.
    cap_read_disable=true;
  }
  else{                                                    //if branch executes if in range.
    digitalWrite(CAP_HIGH_PIN,LOW);                        //sets both pins to low.
    digitalWrite(CAP_LOW_PIN,LOW);
    digitalWrite(RELAY_CONTROL,LOW);
    cap_read_disable=false;
  }
}
void printData(float tempF,uint16_t capReading){
  Serial.println(tempF);
  Serial.println(capReading);
}
