#include "Functions_robot_car.h"

LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 20, 4);

//tela de inicialização
void start_screen(){
    LCD.init();
    LCD.backlight();
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
  
  //desenha a garra aberta (< >) e fechada (<>)
  void Draw_Claw(bool clawState){
    if(clawState){LCD.print("< >");} 
    else{LCD.print(" <>");}
  }

  //desenha o robo no lcd na posição determinada e com a garra aberta(true) ou fechada(false)
  void Draw_Robot(byte posit, bool clawState){
      LCD.setCursor(posit, 1);
      LCD.print("|__|");
      LCD.setCursor((posit+1),2);
      LCD.print("|");
      LCD.setCursor((posit),3);
      Draw_Claw(clawState);
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
    if(lastPosit==1){
      lastPosition = lastPosit;
      ToP1 = false;
    }
    else{  
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
      lastPosition = lastPosit;
      ToP1 = false;
      digitalWrite(LedG,HIGH);
    }
  }
  
  void Go_to_P2(byte lastPosit){
    if(lastPosit==8){
      lastPosition = lastPosit;
      ToP2 = false;
    }
    else{
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
      lastPosition = lastPosit;
      ToP2 = false;
      digitalWrite(LedG,HIGH);
    }
  }
  
  void Go_to_P3(byte lastPosit){ 
    if(lastPosit==15){
      lastPosition = lastPosit;
      ToP3 = false;
    }
    else{
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
      lastPosition = lastPosit;
      ToP3 = false;
      digitalWrite(LedG,HIGH);
    }
  }
  