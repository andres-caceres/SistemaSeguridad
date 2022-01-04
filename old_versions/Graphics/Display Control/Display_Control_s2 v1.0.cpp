#define NOKIA_CLK_PIN	14
#define NOKIA_DIN_PIN	15
#define NOKIA_DC_PIN	16
#define NOKIA_CE_PIN	17
#define NOKIA_RST_PIN	18
#define NOKIA_LED_PIN	19

#include <LCD5110_Graph.h>

LCD5110 myGLCD(NOKIA_CLK_PIN,NOKIA_DIN_PIN,NOKIA_DC_PIN,NOKIA_RST_PIN,NOKIA_CE_PIN);

extern uint8_t SmallFont[];
extern uint8_t ACGIndustries[];
extern unsigned char TinyFont[];

void setup()
{
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  
  pinMode(NOKIA_LED_PIN,OUTPUT); //Backlight
  digitalWrite(NOKIA_LED_PIN,LOW); //Backlight off
  
}

void Display_Control_s2 ()
{
	//intro
  myGLCD.clrScr();
  myGLCD.drawBitmap(0, 0, ACGIndustries, 84, 48);
  myGLCD.update();

  delay(1500);
  
  myGLCD.clrScr();
  myGLCD.print("Sistema de ", CENTER, 0);
  myGLCD.print("Seguridad", CENTER, 8);
  myGLCD.print("v2.6", CENTER, 20);
  myGLCD.drawRect(28, 18, 56, 28);
  for (int i=0; i<6; i++)
  {
    myGLCD.drawLine(57, 18+(i*2), 83-(i*3), 18+(i*2));
    myGLCD.drawLine((i*3), 28-(i*2), 28, 28-(i*2));
  }
  myGLCD.setFont(TinyFont);
  myGLCD.print("(C)2020 por", CENTER, 36);
  myGLCD.print("Andres Caceres", CENTER, 42);
  myGLCD.update();
  
  delay(5000);
  
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myGLCD.print("Runtime (ms):", CENTER, 32);
  myGLCD.printNumI(millis(), CENTER, 40);
  myGLCD.update();
  
  delay(5000);

}

void loop()
{
	Display_Control_s2();
}