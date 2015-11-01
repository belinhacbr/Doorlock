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

unsigned long previousMillis = 0;
const long interval = 10000;

bool keyPressed(char customKey);
void feedActivityLED(unsigned long currentMillis);
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
  
  unsigned long currentMillis = millis();
  feedActivityLED(currentMillis);
  
  char customKey = customKeypad.getKey();
  if(keyPressed(customKey)){
      previousMillis = currentMillis;
      feedActivityLED(currentMillis);
      
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

void feedActivityLED(unsigned long currentMillis){
  //To save power i only set HIGH on red led in case of recent activity
  bool keyPressedRecently = currentMillis - previousMillis <= interval;
  if(keyPressedRecently){
    digitalWrite(redLed, HIGH);
  }else{
    digitalWrite(redLed, LOW);
  }
}


void unlockProc(){
    digitalWrite(greenLed, HIGH);
    digitalWrite(redLed, LOW);
    digitalWrite(motor, HIGH);  //UNLOCK
    Serial.println("open");
    delay(1500);   //wait 1,5 seconds
    digitalWrite(greenLed, LOW);
    digitalWrite(motor, LOW);    //LOCK
    Serial.println("closed");

}
