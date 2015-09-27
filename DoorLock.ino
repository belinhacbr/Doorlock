#include "Keypad.h"

int motor = 2;

int redLed = 4;
int greenLed = 3;
int buttonLock = 13;

const byte ROWS = 4; 
const byte COLS = 4; 
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {12, 11, 10, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6, 5}; //connect to the column pinouts of the keypad
Keypad customKeypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

int STATEA = 0;
int STATEB = 0;

char passA[4] = {'A','B','C','D'};
char passB[4] = {'1','2','3','4'};



bool keyPressed(char customKey);
void unlockProc();

void setup() {
  // put your setup code here, to run once:
  pinMode(motor, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  
  pinMode(buttonLock, INPUT);
  Serial.begin(9600);
  
}

void loop() {
  digitalWrite(motor, LOW);    //LOCK
  char customKey = customKeypad.getKey();
  digitalWrite(redLed, HIGH);
  if(keyPressed(customKey)){
      digitalWrite(greenLed, HIGH);
      
      Serial.print(">>");
      Serial.println(customKey);   
       
      if(customKey==passA[STATEA]){
        STATEA++;  
        Serial.println(customKey);    
      }else{
        STATEA=0;
      } 

      
      if(digitalRead(buttonLock)== HIGH){
        if(customKey==passB[STATEB]){
           STATEB++;
           Serial.println(customKey);
        }else{
          STATEB=0;
        }

      }else{
        digitalWrite(greenLed, HIGH);
        delay(100); 
        digitalWrite(greenLed, LOW);
        delay(100);   
        digitalWrite(greenLed, HIGH);
        delay(100);
        digitalWrite(greenLed, LOW);
      }
  
      delay(250);
      digitalWrite(greenLed, LOW);
 
      if(STATEA==4){
        STATEA=0;
        STATEB=0; 
        unlockProc();
      }
      
      if(STATEB==4){
        STATEA=0;
        STATEB=0;
        //BUZZ IN
        unlockProc();
      }
      
   }
}

bool keyPressed(char customKey){
  return ((customKey>='0' && customKey<='9') || customKey=='*' || customKey=='#' || (customKey>='A' && customKey<='D'));
}


void unlockProc(){
    digitalWrite(greenLed, HIGH);
    digitalWrite(redLed, LOW);
    digitalWrite(motor, HIGH);  //UNLOCK
    Serial.println("open");
    delay(1000);   //wait 1 second
    digitalWrite(greenLed, LOW);
    delay(9000);   //wait 9 seconds
    digitalWrite(motor, LOW);    //LOCK
    Serial.println("closed");

}
