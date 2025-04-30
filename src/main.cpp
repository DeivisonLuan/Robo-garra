#include <Arduino.h>
#include "Functions_robot_car.h"

volatile unsigned long timestamp_last_interrupt_p1 = 0;
volatile unsigned long timestamp_last_interrupt_p2 = 0;
volatile unsigned long timestamp_last_interrupt_p3 = 0;

volatile bool ToP1 = false;
volatile bool ToP2 = false;
volatile bool ToP3 = false;

void IRAM_ATTR button_p1();
void IRAM_ATTR button_p2();
void IRAM_ATTR button_p3();

//Variaveis globais
byte lastPosition;

void setup() {
  pinMode(LedR, OUTPUT);
  pinMode(LedG, OUTPUT);
  pinMode(LedB, OUTPUT);

  pinMode(ButtonP1, INPUT_PULLUP);
  pinMode(ButtonP2, INPUT_PULLUP);
  pinMode(ButtonP3, INPUT_PULLUP);

  attachInterrupt(ButtonP1, button_p1, FALLING);
  attachInterrupt(ButtonP2, button_p2, FALLING);
  attachInterrupt(ButtonP3, button_p3, FALLING);
  
  randomSeed(analogRead(26));

  Serial.begin(115200);

  start_screen();
  initial_screen();
  digitalWrite(LedG,HIGH);
}

void loop() {
  if(ToP1 && !ToP2 && !ToP3){
    Go_to_P1(lastPosition);
    ToP2 = false;
    ToP3 = false;
  }
  else if(!ToP1 && ToP2 && !ToP3){
    Go_to_P2(lastPosition);
    ToP1 = false;
    ToP3 = false;
  }
  else if(!ToP1 && !ToP2 && ToP3){
    Go_to_P3(lastPosition);
    ToP1 = false;
    ToP2 = false;
  }
  else{
    ToP1 = false;
    ToP2 = false;
    ToP3 = false;
  }
  Serial.println(lastPosition);
  delay(2000);
}

void IRAM_ATTR button_p1(){
  if (micros() - timestamp_last_interrupt_p1 > DebounceTime) {
    ToP1 = true;
    timestamp_last_interrupt_p1 = micros();
  }
}
void IRAM_ATTR button_p2(){
  if (micros() - timestamp_last_interrupt_p2 > DebounceTime) {
    ToP2 = true;
    timestamp_last_interrupt_p2 = micros();
  }
}
void IRAM_ATTR button_p3(){
  if (micros() - timestamp_last_interrupt_p3 > DebounceTime) {
    ToP3 = true;
    timestamp_last_interrupt_p3 = micros();
  }
}