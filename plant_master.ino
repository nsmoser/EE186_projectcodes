const int WIRES=4;                                           //specifies wires per sender arduino
const int DEVICE_MAX=8;
const int ARRAY_MAX=32;                                      //specifies max array elements
const int START_PIN=22;                                      //specifies first pin
int wireArray[ARRAY_MAX],pinArray[ARRAY_MAX];                //creates array to store wire data and pin location
int wireKeeper[DEVICE_MAX][WIRES];                           //keeps track of which arduino is connected to which wires
bool loopExit=false;                                         //controls while loops
int arrayIndexes,devices,lastPin,i,j;                        //variables needed in main

void setup() {
  int *arrayPointer=&arrayIndexes;
  
  while(!loopExit){                                          //waits for user exit flag to be true
    devices=arraySizeCheck(arrayPointer);                    //gets number of arduinos connected
    Serial.begin(9600);                                      //restarts serial
  }
  wireIdentification(*arrayPointer,devices);

}

void loop() {
  for(i=0;i!=devices;i++){                                   //assigns all elements of pinArray to wireKeeper
    for(j=0;j!=WIRES;j++){                                   //stored in 2d array in order to make device identification easier
      Serial.println(wireKeeper[i][j]);                 //example: device 2 wires stored in row[1]column[0-3]
    }                                                        //essentially sorts pinArray depending on number of devices                                              //counter ensures right value is pulled from pinArray,not repeating
  }
  delay(1000);
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

int wireIdentification(int arrayPointer, int devices){
  int counter=0;
  for(i=0;i!=arrayPointer;i++){                          
    wireArray[i]=0;                                          //initializes wireArray
    pinArray[i]=i+START_PIN;                                 //assigns values to pinArray
    }           
 
  for(i=0;i!=arrayIndexes;i++){
    pinMode(pinArray[i],INPUT_PULLUP);                       //initializes defined pins in pinArray
  }
  for(i=0;i!=devices;i++){                                   //assigns all elements of pinArray to wireKeeper
    for(j=0;j!=WIRES;j++){                                   //stored in 2d array in order to make device identification easier
      wireKeeper[i][j]=pinArray[j+counter];                  //example: device 2 wires stored in row[1]column[0-3]
    }                                                        //essentially sorts pinArray depending on number of devices
    counter+=4;                                              //counter ensures right value is pulled from pinArray,not repeating
  }
}
