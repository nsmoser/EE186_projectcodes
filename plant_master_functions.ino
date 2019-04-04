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
  analogWrite(6,100);
  lcd.begin(20,4);                                           //starts lcd screen
}

void lcdStatus(){                                            //prints device number and status line
  lcd.home();                                                //moved for readability
  lcd.print("  Device ");
  lcd.print(deviceNum+1);
  lcd.print(" Status");
  lcd.setCursor(0,1);
  lcd.print("--------------------");
}

void tempCheck(){                                           //function that displays temp info
  if(digitalRead(wireKeeper[deviceNum][tempHigh])==digitalRead(wireKeeper[deviceNum][tempLow])){
    lcd.setCursor(0,2);
    lcd.print("Temp: In Range");
  }
  if(digitalRead(wireKeeper[deviceNum][tempHigh])>digitalRead(wireKeeper[deviceNum][tempLow])){
    lcd.setCursor(0,2);
    lcd.print("Temp: Too High");
  }
  if(digitalRead(wireKeeper[deviceNum][tempHigh])<digitalRead(wireKeeper[deviceNum][tempLow])){
    lcd.setCursor(0,2);
    lcd.print("Temp: Too Low ");
  }
}

void capCheck(){                                            //function that displays moisture info
  if(digitalRead(wireKeeper[deviceNum][capHigh])==digitalRead(wireKeeper[deviceNum][capLow])){
    lcd.setCursor(0,3);
    lcd.print("Moisture: In Range");
  }
  if(digitalRead(wireKeeper[deviceNum][capHigh])>digitalRead(wireKeeper[deviceNum][capLow])){
    lcd.setCursor(0,3);
    lcd.print("Moisture: Too High");
  }
  if(digitalRead(wireKeeper[deviceNum][capHigh])<digitalRead(wireKeeper[deviceNum][capLow])){
    lcd.setCursor(0,3);
    lcd.print("Moisture: Too Low ");
  }
}
