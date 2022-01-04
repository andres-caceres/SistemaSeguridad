    /////////////////////////////////////////////////////////////////
   //             Arduino Nokia 5110 Graphics           v1.00     //
  //       Get the latest version of the code here:              //
 //      http://educ8s.tv/nokia-5110-bitmap-graphics            //
/////////////////////////////////////////////////////////////////


#include <LCD5110_Graph.h>

LCD5110 lcd(8,9,10,12,11);

extern uint8_t icons[];
extern uint8_t pi[];
extern uint8_t cube[];

void setup()
{
  lcd.InitLCD();
}

void loop()
{
  lcd.clrScr();
  lcd.drawBitmap(0,0,pi,84,48);
  lcd.update();
  delay(2000);

  lcd.clrScr();
  lcd.drawBitmap(0,0,icons,84,48);
  lcd.update();
  delay(2000);
  
  lcd.clrScr();
  lcd.drawBitmap(0,0,cube,84,48);
  lcd.update();
  delay(2000);

}


