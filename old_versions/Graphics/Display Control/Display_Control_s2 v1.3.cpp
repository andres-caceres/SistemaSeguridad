#define NOKIA_CLK_PIN  14
#define NOKIA_DIN_PIN 15
#define NOKIA_DC_PIN  16
#define NOKIA_CE_PIN  17
#define NOKIA_RST_PIN 18
#define NOKIA_LED_PIN 19

#include <LCD5110_Graph.h>

LCD5110 myGLCD(NOKIA_CLK_PIN,NOKIA_DIN_PIN,NOKIA_DC_PIN,NOKIA_RST_PIN,NOKIA_CE_PIN);

extern unsigned char TinyFont[];
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

extern uint8_t ACGIndustries[];
extern uint8_t Alert_symbol[];

uint32_t t0_s1=0;
uint8_t DEFCON =2;
uint8_t gates =1;
uint8_t lockdown =1;

uint8_t Display_State =0;

void Display_Control_s2()
{

  switch(Display_State)
  {
  case 0: //Startup intro
    {
    
    myGLCD.clrScr();
    myGLCD.drawBitmap(0, 0, ACGIndustries, 84, 48);
    myGLCD.update();

    delay(2000);
	
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

    delay(2000);
	Display_State=1;
    }
  case 1: //Main DEFCON status monitor
    {
    myGLCD.clrScr();
    myGLCD.setFont(SmallFont);
    if(DEFCON==0)
    {
      
      myGLCD.print("[ INACTIVO ]", CENTER, 0);
	  
	  uint32_t segundos_runtime=(millis()/1000);
      uint32_t minutos_runtime=(segundos_runtime/60);
      uint32_t horas_runtime=(minutos_runtime/60);
	  
	  myGLCD.setFont(TinyFont);
	  myGLCD.print("Ultimo reinicio hace", LEFT, 36);
	  myGLCD.print("t=   h   m   s", LEFT, 42);
	  myGLCD.printNumI(segundos_runtime-(minutos_runtime*60), 44, 42,2,' ');
      myGLCD.printNumI(minutos_runtime-(horas_runtime*60), 28, 42,2,' ');
      myGLCD.printNumI(horas_runtime, 12, 42,2,' ');
    }
    else
    {
	  myGLCD.drawRect(0,20,66,34);
	  myGLCD.clrLine(0,20,0,34);
	  myGLCD.clrLine(0,34,66,34);
	  
      myGLCD.print("[  ACTIVO  ]", CENTER, 0);
      myGLCD.print("DEFCON", CENTER, 12);
      myGLCD.setFont(BigNumbers);
      myGLCD.printNumI(DEFCON, RIGHT, 10);
      
      uint32_t segundos_s2=(millis()-t0_s1)/1000;
      uint32_t minutos_s2=(segundos_s2/60);
      uint32_t horas_s2=(minutos_s2/60);
      
      myGLCD.setFont(TinyFont);
	  myGLCD.print("Ultimo incidente", LEFT, 22);
      myGLCD.print("t=   h   m   s", LEFT, 28);
      myGLCD.printNumI(segundos_s2-(minutos_s2*60), 44, 28,2,' ');
      myGLCD.printNumI(minutos_s2-(horas_s2*60), 28, 28,2,' ');
      myGLCD.printNumI(horas_s2, 12, 28,2,' ');
	  
	  
            
      // gates open
      myGLCD.print("PUERTAS ", LEFT, 36);
      if(gates==0)
      {
      myGLCD.print("CERRADAS", 32, 36);
      }
      else
      {
      myGLCD.print("ABIERTAS",32, 36);
      myGLCD.print("X",74, 36);
      }
      //lockdown set
      myGLCD.print("ALARMA ", LEFT, 43);
      if(lockdown==0)
      {
      myGLCD.print("INACTIVA", 32, 43);
      }
      else
      {
      myGLCD.print("ACTIVA",32, 43);
      myGLCD.print("X",74, 43);
      }

    }
    myGLCD.update();
    }
  case 2: //Alert symbol
  {
	myGLCD.drawBitmap(0, 0, Alert_symbol, 84, 48);
	myGLCD.update();
  }
  }
  
}

void setup()
{
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  
  pinMode(NOKIA_LED_PIN,OUTPUT); //Backlight
  digitalWrite(NOKIA_LED_PIN,LOW); //Backlight off
  t0_s1=0;
  Display_State=0;
  
}
void loop()
{
    Display_Control_s2();
}