#include "dht.h"
#include <Keypad.h>

const int ROW_NUM = 4; 
const int COLUMN_NUM = 4; 

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pin_rows[ROW_NUM] = {9, 8, 7, 6};   
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2}; 
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

String inputString;
long stime;
long endtime;
bool light = 0;

#define dht_apin A1
const int ldr_pin = A0; 
dht DHT;
 
void setup(){
  Serial.begin(9600);
  pinMode(ldr_pin, INPUT);
  inputString.reserve(6); 
  pinMode(13, OUTPUT);
  Serial.println("-----DETAILS OF THE LIGHT CYCLE-----\n");
  Serial.println("Enter start time of light cycle: ");
  stime = key_to_int();
  Serial.print("\nSaved start time as ");
  Serial.println(stime);
  Serial.println("\nEnter end time of light cycle:");
  endtime = key_to_int();
  Serial.print("\nSaved end time as ");
  Serial.println(endtime);
  Serial.println("\n-----TEMPERATURE,HUMIDITY AND LIGHT READINGS-----");
  Serial.println("*******************************************************");
}
 
void loop(){
    while (1) {
    if (stime < millis()) {
      digitalWrite(13, HIGH);
    }
    if (millis() > endtime) {    
     int ldrStatus = analogRead(ldr_pin);
     DHT.read11(dht_apin);
     Serial.print(DHT.temperature);
     Serial.print(" ");
     Serial.print(DHT.humidity);
     Serial.print(" ");
     Serial.println(ldrStatus);
     if (ldrStatus <= 150) {
      digitalWrite(13, HIGH);
      //Serial.println("LDR is Dark, light is ON\n");
      delay(2500);
     } else if (ldrStatus <= 350) {
      digitalWrite(13, HIGH);
      //Serial.println("LDR is Mid, light is ON\n");
      delay(2500);
     } else if (ldrStatus <= 1000){
      digitalWrite(13, LOW);
      //Serial.println("LDR is light, light is OFF\n");
      delay(2500);
     }
    }        
  } 
}

long key_to_int() {
  long inputInt;
  while (1) {
    char key = keypad.getKey();

    if (key) {
      Serial.print(key);

      if (key >= '0' && key <= '9') {
        inputString += key;
      } else if (key == '#') {
        if (inputString.length() > 0) {
          inputInt = inputString.toInt();
          inputString = "";
          break;

        }
      } else if (key == '*') {
        inputString = "";
      }
    }
  }
  return inputInt;
}
