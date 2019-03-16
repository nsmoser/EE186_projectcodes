#include<LiquidCrystal.h>                                    //library needed to make lcd screen work
const int rs=13,en=12,d4=11,d5=10,d6=9,d7=8;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);                        //specifies which wires are connected to which lcd spots

const int WIRES=4;                                           //specifies wires per sender arduino
const int DEVICE_MAX=8;                                      //specifies maximum amount of devices allowed to master
const int ARRAY_MAX=32;                                      //specifies max array elements
const int START_PIN=22;                                      //specifies first pin
const int BUTTON_PIN=14;                                     //specifies which pin button is connected to
const int tempHigh=0,tempLow=1,capHigh=2,capLow=3;          //array locations of specific pins
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
  lcd.clear();
}

int deviceNum=0;                                             //deviceNum declared outside of loop so it is not reset to zero by accident

void loop(){
  lcdStatus();                                               //status header on lcd screen, takes up first two lines of screen
  tempCheck();                                               //check temp info readings, displays on third line of screen
  capCheck();                                                //check moisture info readings, displays on fourth line of screen
  deviceSwitch();                                            //checks to see if device is being toggled
}
