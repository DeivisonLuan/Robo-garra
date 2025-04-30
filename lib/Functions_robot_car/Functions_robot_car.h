#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define LedR 25
#define LedG 33
#define LedB 32

#define ButtonP1 27
#define ButtonP2 14
#define ButtonP3 12

#define DebounceTime 100*1000 //us

extern volatile bool ToP1;
extern volatile bool ToP2;
extern volatile bool ToP3;

extern byte lastPosition;

//Assinatura de funções
void Go_to_P1(byte lastPosit);
void Go_to_P2(byte lastPosit);
void Go_to_P3(byte lastPosit);
void start_screen();
void headline();
void Draw_Claw(bool clawState);
void Draw_Robot(byte posit, bool clawState);
void initial_screen();