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
volatile bool ToP3 = false;

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
void Draw_Robot(byte posit, bool clawState);
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

//desenha o robo no lcd na posição determinada e com a garra aberta(true) ou fechada(false)
void Draw_Robot(byte posit, bool clawState){
    LCD.setCursor(posit, 1);
    LCD.print("|__|");
    LCD.setCursor((posit+1),2);
    LCD.print("|");
    LCD.setCursor((posit+1),3);
    if(clawState){LCD.print("^ ");} 
    else{LCD.print("<>");}
}
//Tela que mostra a disposição inicial do sistema
void initial_screen(){
  headline();
  
  //define qual a posição que o carro começa aleatoriamente
  byte InitialPosition = random(1,4);

  //converte a posição inicial na posição do cursor no lcd para o carro
  switch (InitialPosition){
    case 1:
      InitialPosition = 1;
      break;
    case 2:
      InitialPosition = 8;
    break;
    case 3:
      InitialPosition = 15;
    break;
    default:
      break;
  }
  
  //desenha o carro na posição inicial
  lastPosition = InitialPosition;
  Draw_Robot(lastPosition, true);
}

void Go_to_P1(byte lastPosit){
  digitalWrite(LedG,LOW); //indica que recebeu o comando

  //fechamento da garra
  digitalWrite(LedR,HIGH);
  Draw_Robot(lastPosit,false);
  delay(500);
  digitalWrite(LedR,LOW);

  //movimento do carro
  digitalWrite(LedB,HIGH);
  lastPosit = 1;
  for(byte i=(lastPosition-1);i>=lastPosit;i--){
    LCD.clear();
    headline();
    Draw_Robot(i,false);
    delay(300);
  }
  delay(500);
  digitalWrite(LedB,LOW);

  //abertura da garra
  digitalWrite(LedR,HIGH);
  Draw_Robot(lastPosit,true);
  delay(300);
  digitalWrite(LedR,LOW);
  digitalWrite(LedG,HIGH);
  lastPosition = lastPosit;
  ToP1 = false;
}

void Go_to_P2(byte lastPosit){
  digitalWrite(LedG,LOW); //indica que recebeu o comando

  //fechamento da garra
  digitalWrite(LedR,HIGH);
  Draw_Robot(lastPosit,false);
  delay(500);
  digitalWrite(LedR,LOW);

  //movimento do carro
  digitalWrite(LedB,HIGH);
  lastPosit = 8;
  if(lastPosition>lastPosit){
    for(byte i=(lastPosition-1);i>=lastPosit;i--){
    LCD.clear();
    headline();
    Draw_Robot(i,false);
    delay(300);
    }
  }
  else if(lastPosition<lastPosit){
    for(byte i=(lastPosition+1);i<=lastPosit;i++){
    LCD.clear();
    headline();
    Draw_Robot(i,false);
    delay(300);
    }
  }
  delay(500);
  digitalWrite(LedB,LOW);

  //abertura da garra
  digitalWrite(LedR,HIGH);
  Draw_Robot(lastPosit,true);
  delay(300);
  digitalWrite(LedR,LOW);
  digitalWrite(LedG,HIGH);
  lastPosition = lastPosit;
  ToP2 = false;
}

void Go_to_P3(byte lastPosit){ 
  digitalWrite(LedG,LOW); //indica que recebeu o comando

  //fechamento da garra
  digitalWrite(LedR,HIGH);
  Draw_Robot(lastPosit,false);
  delay(500);
  digitalWrite(LedR,LOW);

  //movimento do carro
  digitalWrite(LedB,HIGH);
  lastPosit = 15;
  for(byte i=(lastPosition+1);i<=lastPosit;i++){
    LCD.clear();
    headline();
    Draw_Robot(i,false);
    delay(300);
  }
  delay(500);
  digitalWrite(LedB,LOW);

  //abertura da garra
  digitalWrite(LedR,HIGH);
  Draw_Robot(lastPosit,true);
  delay(300);
  digitalWrite(LedR,LOW);
  digitalWrite(LedG,HIGH);
  lastPosition = lastPosit;
  ToP3 = false;
}


/*

#Separar as funções em .h ou descrever com // as funções e trechos do codigo

# Não fazer nada se mandar ir pra posição que já está.

*/