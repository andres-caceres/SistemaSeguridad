
#define MAIN_LED_PIN       13

#define SMALL_BUZZER_PIN    3
#define BIG_BUZZER_PIN      0
#define SIREN_ALARM_PIN     0

#define PANIC_BUTTON_PIN    3 //INT1
#define RESET_BUTTON_PIN    2 //INT0

#define LOCKDOWN_SWITCH_PIN 0
#define ARMED_SWITCH_PIN    5

#define GATE_SWITCH_1_PIN   4
#define MOTION_SWITCH_PIN   0


//main systems control variables
bool rst=0;
bool gates=0;
bool armed=0;
bool lockdown=0;
bool motion=0;
bool panic=0;

volatile int DEFCON=0;
int prev_DEFCON=5;

//time control variables

unsigned long t_s1=0;
unsigned long t0_s1=0;

const unsigned long T1=8000;
const unsigned long T2=8000;
const unsigned long T3=10000;

//working variables

bool rst_flag=0;

//Functions for imputs

void record_imputs (){
  
  rst=digitalRead(RESET_BUTTON_PIN);
  panic=digitalRead(PANIC_BUTTON_PIN);
  gates=digitalRead(GATE_SWITCH_1_PIN);
  armed=digitalRead(ARMED_SWITCH_PIN);
  lockdown=0;
  motion=0;
  
}

//Functions for actions


void sleep()
{
  //Serial.println("DISARMED");
}
void guard()
{
  //Serial.println("ARMED");
}
void warn()
{
  //Serial.println("WARNING");
}
void speak()
{ 
  //Serial.println("SPEAKING");
}
void shout()
{
  //Serial.println("SHOUTING"); 
}
void defend()
{
  //Serial.println("DEFENDING");  
}

//Functions for indicators

void beep_once()
{
   Serial.println("beep_once");
   delay(100);
}
void beep_twice()
{
  Serial.println("beep_twice");
  delay(200);
}
void beep_long()
{
  Serial.println("beep_long");
  delay(1000);
}

void beep_longer()
{
  Serial.println("beep_longer");
  delay(2000);
}

//Buttons

unsigned long debounce_t0=0;
unsigned long debounce_tc=50;

bool button_release(int BTN_PIN)
{
  if (digitalRead(BTN_PIN)==1)
  {
    debounce_t0=millis();
    while(digitalRead(BTN_PIN)==1);
    if((millis()-debounce_tc)>=debounce_t0)
    {
    return 1;
    }
  }
  return 0;
}
/*
//ISR variables

//ISR functions

void reset_ISR()//reset_ISR
{
	//uint8_t t_s2=0;
	Serial.println("reset_ISR");
	rst_flag=1;
    DEFCON=5;
    beep_once();
	
}

void panic_ISR()
{
	//uint8_t t_s2=0;
	Serial.println("panic_ISR");
	DEFCON=2;
    beep_twice();
}
*/
//configs

void configs()
{
  pinMode(SMALL_BUZZER_PIN, OUTPUT);
  pinMode(RESET_BUTTON_PIN, INPUT);
  pinMode(ARMED_SWITCH_PIN, INPUT);
  pinMode(GATE_SWITCH_1_PIN, INPUT);
  Serial.println("I/O pin configuration successfull");
  
  /*
  //interupt configs
  
	attachInterrupt(digitalPinToInterrupt(RESET_BUTTON_PIN),reset_ISR,FALLING);
  
    attachInterrupt(digitalPinToInterrupt(PANIC_BUTTON_PIN),panic_ISR,FALLING);
	
	Serial.println("ISR configuration successfull");
	
  */
}



//MAIN CODE

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  configs();
  Serial.println("All systems initialized");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  State_Machine_s1();
    if(DEFCON!=prev_DEFCON)
  {
    Serial.print("DEFCON=");
    Serial.println(DEFCON);
	t0_s1=millis();
  }
  prev_DEFCON =DEFCON; //solo para ver estado actual
  record_imputs();
  delay(200);
}

void State_Machine_s1(){

  switch(DEFCON)
  {
    case 0: //disarmed
  { 
      //actions
    
    sleep();

      /*
      digitalWrite(SMALL_BUZZER_PIN,0);
      digitalWrite(BIG_BUZZER_PIN,0);
      digitalWrite(SIREN_ALARM_PIN,0);
      */
      //transitions
      
      if(armed==1)
      {
        DEFCON=5;
        beep_twice();
      }

  }   
    break;
    
  case 5: //DEFCON 5
  {
      //actions
      guard();
      
      //transitions
      
      if(armed==1) //armed switch
      {
        if(gates==1) //gates open
        {
		  if(rst_flag==0)
		  {
			
			if(lockdown==1) //lockdown switch
			{
			DEFCON=2;
			beep_long();
			}else
			{
			DEFCON=4;
			}
		  
			beep_twice();
		  }
        }
		else
		{
		  rst_flag=0;
		}
      }
    else
      {
        DEFCON=0;
        beep_once();
      }
	  
	if(panic==1) //panic button pressed
	{
	DEFCON=2;
	beep_twice();
	beep_long();
	}
  }   
    break;
    
  case 4: //DEFCON 4
  {        
      //actions
      warn();
	  t_s1=millis();
      //transitions
      
      if(armed==1) //armed switch
      {
          
		  if(gates==1)
		  {
			  if(t_s1>=t0_s1+T1) //time until DEFCON 3
			  {
			  DEFCON=3;
			  beep_twice();           
			  }
		  }
		  else
		  {
			  DEFCON=5;
			  beep_once();
		  }          

      }
      else
      {
        DEFCON=0;
        beep_once();
      }
	  
	if(panic==1) //panic button pressed
	{
	DEFCON=2;
	beep_twice;
	beep_long();
	}
	
	if(rst==1) //rst button pressed
	{
	rst_flag=1;
	DEFCON=5;
	beep_once();
	}
  }   
    break;
    
  case 3: //DEFCON 3
  {        
      //actions
      warn();
	  speak();
      t_s1=millis();
      //transitions
      
      if(armed==1) //armed switch
      {

          if(gates==1)
          {
            if(t_s1>=t0_s1+T2) //time until DEFCON 2
            {
            DEFCON=2;
            beep_twice();
            }
          }
          else
          {
            DEFCON=5;
            beep_once();
          }
      }
      else
      {
        DEFCON=0;
        beep_once();
      }
	  
	if(panic==1) //panic button pressed
	{
	DEFCON=2;
	beep_twice();
	beep_long();
	}
	
	if(rst==1) //rst button pressed
	{
	rst_flag=1;
	DEFCON=5;
	beep_once();
	}	  
  }   
    break;
    
  case 2: //DEFCON 2
  {     
      //actions
      warn();
      speak();
      shout();
      t_s1=millis();
      //transitions
      
      if(armed==1) //armed switch
      {
        
        if(t_s1>=t0_s1+T3) //time until DEFCON 1
        {
          DEFCON=1;          
          beep_twice();
		  beep_longer();//longer
          
        }
      }
      else
      {
        DEFCON=0;
        beep_once();        
      }
	  	
	if(rst==1) //rst button pressed
	{
	rst_flag=1;	
	DEFCON=5;
	beep_once();
	}
	
  }   
    break;
    
  case 1: //DEFCON 1
  {
      //actions
      warn();
      speak();
      shout();
      defend();
      
      //transitions
      
      if(armed==1) //armed switch
      {
        
      }
      else
      {
        DEFCON=0;
        beep_once();  
      }
	
	if(rst==1) //rst button pressed
	{
	rst_flag=1;	
	DEFCON=5;
	beep_once();
	}	  
	  
  }   
    break;
      
  }

}

