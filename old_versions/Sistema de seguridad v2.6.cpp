/*
SISTEMA DE SEGURIDAD v2.6

	21/03
	pre pantalla nokia

*/


#define MAIN_LED_PIN        13

#define SMALL_BUZZER_PIN    8
#define SPEAKER_PIN         9
#define SIREN_ALARM_PIN     10

#define PANIC_BUTTON_PIN    3 //INT1
#define RESET_BUTTON_PIN    2 //INT0

#define LOCKDOWN_SWITCH_PIN 6
#define ARMED_SWITCH_PIN    5

#define GATE_SWITCH_1_PIN   4
#define MOTION_SWITCH_PIN   0

#define SPEAKER_FREQ_MIN    200
#define SPEAKER_FREQ_MAX  1000

#define debug           	0

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

uint32_t t_s1=0;
uint32_t t0_s1=0;

const uint32_t T1=8000;
const uint32_t T2=8000;
const uint32_t T3=10000;

//led, buzzer and speaker control variables

uint32_t t0_guard=0;
uint32_t t0_warn=0;
uint32_t t0_speak=0;
uint32_t t0_shout=0;

const uint32_t T_guard=2000;
const uint32_t T_warn=1000;
const uint32_t T_speak=1;
const uint32_t T_shout=3000;


//working variables

bool rst_flag=0;

uint16_t freq=SPEAKER_FREQ_MIN;

//Functions for imputs
void record_imputs (){
  
  rst=digitalRead(RESET_BUTTON_PIN);
  panic=digitalRead(PANIC_BUTTON_PIN);
  gates=digitalRead(GATE_SWITCH_1_PIN);
  armed=digitalRead(ARMED_SWITCH_PIN);
  lockdown=digitalRead(LOCKDOWN_SWITCH_PIN);
  motion=0;
  
}


//Functions for indicators
void beep_once()
{
  if(debug){Serial.println("beep_once");
  delay(100);}
  else{
    
    PORTB|= (1<<0); //buzzer
	PORTB|= (1<<5); //led MAIN_LED_PIN
    delay(50);
    PORTB&=~(1<<0);
	PORTB&=~(1<<5);
    delay(50);
  }
}
void beep_twice()
{
  if(debug){Serial.println("beep_twice");
  delay(200);}
  else
  {
    PORTB|= (1<<0); //buzzer
	PORTB|= (1<<5); //led MAIN_LED_PIN
    delay(50);
    PORTB&=~(1<<0);
	PORTB&=~(1<<5);
    delay(50);
    PORTB|= (1<<0);
	PORTB|= (1<<5);
    delay(50);
    PORTB&=~(1<<0);
	PORTB&=~(1<<5);
    delay(50);
  }

}
void beep_long()
{
  if(debug){Serial.println("beep_long");
  delay(1100);}
  else
  {
	delay(100);
    PORTB|= (1<<0);
    delay(500);
    PORTB&=~(1<<0);
	
  }
}
void beep_longer()
{
  if(debug){Serial.println("beep_longer");
  delay(2100);}
  else
  {
  delay(100);
    PORTB|= (1<<0);
    delay(2000);
    PORTB&=~(1<<0);
	
  }
}


//Functions for actions
void sleep()
{

  if(debug){Serial.println("DISARMED");}
}
void guard()
{
	uint32_t t_guard=millis();
	
	if(t_guard-t0_guard>=T_guard) //time to led change
	{
		PORTB^=(1<<5); //led pin 13 change
		
		t0_guard=t_guard;
	}


  if(debug){Serial.println("ARMED");}
}
void warn()
{
	
	uint32_t t_warn=millis();
	
	if(t_warn-t0_warn>=T_warn) //time to warn
	{
		PORTB^=(1<<5); //led pin 13 change
		beep_once(); //buzz change
		
		t0_warn=t_warn;
	}

  if(debug){Serial.println("WARNING");}
}
void speak() //speaker active
{ 
	uint32_t t_speak=millis();
	
  if(t_speak-t0_speak>=T_speak) //time until speaker tone change
  {
    
    if(freq==SPEAKER_FREQ_MAX)
    {
      freq=SPEAKER_FREQ_MIN;
    }
    else
    {
      freq++;
    }
	tone(SPEAKER_PIN,freq);
	
    t0_speak=t_speak;
  }
  
  if(debug){Serial.println("SPEAKING");}
  
}
void shout() //siren, buzzer active
{
  if(debug){Serial.println("SHOUTING");}
    
	uint32_t t_shout=millis();
	
	if(t_shout-t0_shout>=T_shout) //time of siren
	{
		PORTB^= (1<<2); //siren on/off
		PORTB^= (1<<0); //beeper on/off
		
		t0_shout=t_shout;
	}
  
}
void defend()
{
	
  if(debug){Serial.println("DEFENDING");}
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

//ISR variables

//ISR functions


//configs

void configs()
{
  pinMode(SMALL_BUZZER_PIN, OUTPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(SIREN_ALARM_PIN,OUTPUT);
  
  pinMode(MAIN_LED_PIN,OUTPUT);
  
  pinMode(RESET_BUTTON_PIN, INPUT);
  pinMode(ARMED_SWITCH_PIN, INPUT);
  pinMode(GATE_SWITCH_1_PIN, INPUT);
  pinMode(LOCKDOWN_SWITCH_PIN, INPUT);
  Serial.println("I/O pin configuration successfull");
  
  /*
  //interupt configs
  
  
  
  
  Serial.println("ISR configuration successfull");
  
  */
}



//MAIN CODE

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  configs();
  beep_twice();
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
	
	//reset values
	PORTB&=~(1<<5); //set led off
	PORTB&=~(1<<0); //set buzzer off
	noTone(SPEAKER_PIN); //set speaker off
	freq=SPEAKER_FREQ_MIN; //reset speaker freq
	PORTB&=~(1<<2); //siren off
	
  }
  prev_DEFCON =DEFCON;
  record_imputs(); //refresh imput data
  if(debug){
	  Serial.println(freq);
	  delay(100);}
}

void State_Machine_s1(){

  switch(DEFCON)
  {
    case 0: //disarmed
  { 
      //actions
    
    sleep();

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
			beep_twice();
			beep_long();
			}
			else
			{
			DEFCON=4;
			beep_twice();
			}
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
			beep_long();
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

