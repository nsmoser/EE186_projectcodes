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
  pinMode(BUTTON_PIN,INPUT);
  Serial.begin(9600);
  int *arrayPointer=&arrayIndexes;
  
  while(!loopExit){                                          //waits for user exit flag to be true
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

void deviceSwitch(){
   if(buttonPress()){
    deviceNum++;
    delay(500);
  }
  if(deviceNum==devices){
    deviceNum=0;
  }
}
