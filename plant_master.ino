#include<LiquidCrystal.h>                                    //library needed to make lcd screen work
const int rs=13,en=12,d4=11,d5=10,d6=9,d7=8;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);                        //specifies which wires are connected to which lcd spots

const int WIRES=4;                                           //specifies wires per sender arduino
const int DEVICE_MAX=8;                                      //specifies maximum amount of devices allowed to master
const int ARRAY_MAX=32;                                      //specifies max array elements
const int START_PIN=22;                                      //specifies first pin
const int BUTTON_PIN=14;                                     //specifies which pin button is connected to
int pinArray[ARRAY_MAX];                                     //creates array to store wire data and pin location
int wireKeeper[DEVICE_MAX][WIRES];                           //keeps track of which arduino is connected to which wires
bool loopExit=false;                                         //controls while loops and button
int arrayIndexes,devices,i,j;                                //variables needed in main


void setup(){
  pinMode(BUTTON_PIN,INPUT);                                 //sets button up to be used as input
  lcdBegin();                                                //function with startup info for lcd
  
  Serial.begin(9600);
  int *arrayPointer=&arrayIndexes;                           //pointer that is handed off to arraySizeCheck
  
  while(!loopExit){                                          //waits for user exit flag to be true
    lcdPrompt();                                             //function that displays prompt on lcd screen
    devices=arraySizeCheck(arrayPointer);                    //gets number of arduinos connected
    Serial.begin(9600);                                      //restarts serial
  }
  wireIdentification(*arrayPointer,devices);

}

int deviceNum=0;                                             //deviceNum declared outside of loop so it is not reset to zero by accident

void loop(){
  for(i=0;i!=WIRES;i++){
    int pinState=digitalRead(wireKeeper[deviceNum][i]);
    Serial.print(wireKeeper[deviceNum][i]);
    Serial.print(": ");
    Serial.println(pinState);
  }
  deviceSwitch();
}

int arraySizeCheck(int *arraySize){                           //user defined function to get input
  Serial.begin(9600);                                         //starts serial
  Serial.println("Please give number of watering arduinos");
  while(!Serial.available()){}                                //waits for input from serial monitor
  devices=Serial.parseInt();                                  //processes input
  *arraySize=devices*WIRES;                                   //assigns value to arrayIndexes with pointer
  if(*arraySize<=ARRAY_MAX){loopExit=true;}                   //if value is smaller than allowed max, exit function
  else{Serial.println("Please give valid input");}            //if not, re-ask for valid input
  Serial.end();                                               //closes serial
  return devices;
}

void wireIdentification(int arrayPointer, int devices){
  int counter=0;
  for(i=0;i!=arrayPointer;i++){                          
    pinArray[i]=i+START_PIN;                                  //assigns values to pinArray
    }           
 
  for(i=0;i!=arrayIndexes;i++){
    pinMode(pinArray[i],INPUT);                               //initializes defined pins in pinArray
  }
  for(i=0;i!=devices;i++){                                    //assigns all elements of pinArray to wireKeeper
    for(j=0;j!=WIRES;j++){                                    //stored in 2d array in order to make device identification easier
      wireKeeper[i][j]=pinArray[j+counter];                   //example: device 2 wires stored in row[1]column[0-3]
    }                                                         //essentially sorts pinArray depending on number of devices
    counter+=4;                                               //counter ensures right value is pulled from pinArray,not repeating
  }
}

int buttonPress(){                                            //function that detects a button press
  int buttonState=digitalRead(BUTTON_PIN); 
  if(buttonState){                                            //waits for button to be pressed
    return 1;
  }
  else{
    return 0;
  }
}

void deviceSwitch(){                                          //function that controls switching between devices
   if(buttonPress()){                                         //calls buttonPress function and waits for a return value
    deviceNum++;                                              //increments device number by 1
    delay(500);                                               //half second delay prevents rapid incrementing by accident
  }
  if(deviceNum==devices){                                     //checks if deviceNum is equal to devices
    deviceNum=0;                                              //resets device number to zero
  }                                                           //to get actual device number, add one to deviceNum, 0 only used due to array indexeMs
}

void lcdPrompt(){                                             //displays on lcd screen on startup
    lcd.home();                                               //function created for readability
    lcd.print("Please follow");
    lcd.setCursor(0,1);
    lcd.print("instructions on");
    lcd.setCursor(0,2);
    lcd.print("serial monitor");
}

void lcdBegin(){                                             //startup info for lcd screen
  pinMode(5,OUTPUT);                                         //sets up constrast and brightness for lcd screen
  pinMode(6,OUTPUT);
  analogWrite(5,50);                                         //lowered voltages achieved by pwm
  analogWrite(6,75);
  lcd.begin(20,4);                                           //starts lcd screen
}
