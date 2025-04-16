#include <Arduino.h>
//#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 20, 4);

#define LedR 25
#define LedG 33
#define LedB 32

#define ButtonP1 27
#define ButtonP2 14
#define ButtonP3 12

#define DebounceTime 100*1000 //us

volatile unsigned long timestamp_last_interrupt_p1 = 0;
volatile unsigned long timestamp_last_interrupt_p2 = 0;
volatile unsigned long timestamp_last_interrupt_p3 = 0;

volatile bool ToP1 = false;
volatile bool ToP2 = false;
volatile bool Top3 = false;

void IRAM_ATTR button_p1();
void IRAM_ATTR button_p2();
void IRAM_ATTR button_p3();

//Variaveis globais
byte lastPosition;

//Assinatura de funções
void Go_to_P1(byte lastPosit);
void Go_to_P2(byte lastPosit);
void Go_to_P3(byte lastPosit);
void start_screen();
void headline();
void Robot(byte posit, bool clawState);
void initial_screen();

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

  LCD.init();
  LCD.backlight();
  start_screen();
  initial_screen();
}

void loop() {
  if(ToP1){
    Go_to_P1(lastPosition);
  }
  else if(ToP2){
    Go_to_P2(lastPosition);
  }
  else if(Top3){
    Go_to_P3(lastPosition);
  }
  else{

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
    Top3 = true;
    timestamp_last_interrupt_p3 = micros();
  }
}

//tela de inicialização
void start_screen(){
  LCD.setCursor(4, 0);
  LCD.print("Genius tech!");
  LCD.setCursor(2, 3);
  LCD.print("Inicializando...");
  delay(2000);
  LCD.clear();
}

void headline(){
  //cabeçalho
  LCD.setCursor(1, 0);
  LCD.print("_P1_");
  LCD.setCursor(8, 0);
  LCD.print("_P2_");
  LCD.setCursor(15, 0);
  LCD.print("_P3_");
}

void Robot(byte posit, bool clawState){
  switch (posit){
    case 1:
      LCD.setCursor(1, 1);
      LCD.print("|__|");
      LCD.setCursor(2,2);
      LCD.print("|");
      LCD.setCursor(2,3);
      if(clawState) LCD.print("^");
      else LCD.print("<>");
      break;
    case 2:
      LCD.setCursor(8, 1);
      LCD.print("|__|");
      LCD.setCursor(9,2);
      LCD.print("|");
      LCD.setCursor(9,3);
      LCD.print("^");
      if(clawState) LCD.print("^");
      else LCD.print("<>");
      break;
    case 3:
      LCD.setCursor(15, 1);
      LCD.print("|__|");
      LCD.setCursor(16,2);
      LCD.print("|");
      LCD.setCursor(16,3);
      LCD.print("^");
      if(clawState) LCD.print("^");
      else LCD.print("<>");
      break;
    default:
      LCD.setCursor(2, 1);
      LCD.print("Erro no gerador!");
      break;
    }
}
//Tela que mostra a disposição inicial do sistema
void initial_screen(){
  headline();
  
  byte InitialPosition = random(1,4);
  Serial.println(InitialPosition);

  Robot(InitialPosition, true);

  lastPosition = InitialPosition;
}

void Go_to_P1(byte lastPosit){
  LCD.clear();
  headline();
  Robot(lastPosit,false);
  lastPosition = 1;
  ToP1 = false;
}

void Go_to_P2(byte lastPosit){
  LCD.clear();
  headline();
  Robot(lastPosit,false);
  lastPosition = 2;
  ToP2 = false;
}

void Go_to_P3(byte lastPosit){
  LCD.clear();
  headline();
  Robot(lastPosit,false);
  lastPosition = 3;
  Top3 = false;
}


//separar as funções em .h, observar o comportamento aleatorio do robo quando os botões estão sendo pressionados
//duplicação da garra em alguns momentos
//adicionar animações do carrinho andando de uma posição para a outra