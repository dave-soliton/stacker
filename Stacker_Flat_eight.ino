/*
  This is the flat eight formation code.
  There are three sensors. 
  
  Twobales is a digital input. HIGH = bales at top. LOW = undefined.
  
  liftram_sens senses the position of the slider ram. Analog 5 V = top, 2.5 =bottom
  0V = undefined.
  
  crossram slides two bales across. Analog 5V = out, 2.5 = in, 0V= undefined.
 */
 //---Bale Sensor -------
const int twobales = 13; // This pin sences two bales 
 //---Ram switches ------
const int slideram_sens =A0; //  Lifts bales up 0=Undefined, 2=top,1=bottom
const int crossram_sens =A1; // Moves two bales 2 =out, 1=in, 0=undefined.
//----Ram outputs----------
int slideram_up=0;
int slideram_down=1;
int crossram_in=2;
int crossram_out=3;

int window=100; // Voltage divider window.

int Initial_config_front =0;
int buttonState =0;
int buttoncheck =0;
int slideram_State=0;
int crossram_State=0;
int lift_mode =0;
int slide_mode =0;
int digital_check =0;

void setup() {                
  // initialise the digital pins as an output.   
  pinMode(slideram_up, OUTPUT);  
  pinMode(slideram_down, OUTPUT);     
  pinMode(crossram_in, OUTPUT);  
  pinMode(crossram_out, OUTPUT);
  // initialise analog pins as input
  pinMode(slideram_sens, INPUT); //0= undefined, 500= down, 1025 = up
  pinMode(crossram_sens, INPUT);//0= undefined, 500 = in,  1025 = out
  // initialise digital pins as input
  pinMode(twobales, INPUT);
}

void loop() {
  //----- Places flat eight rams into inital positions 
while(Initial_config_front ==0)
  { 
 slideram_State = analogDbounce(slideram_sens);
 crossram_State = analogDbounce(crossram_sens);
    if(slideram_State !=1)
    {
    digitalWrite(slideram_down, LOW);     
    }
    if(slideram_State ==1)
    {
    digitalWrite(slideram_down, HIGH);   
  delay(1000);  
   
    }
  
    
     if(crossram_State !=1)
    {
    digitalWrite(crossram_in, LOW);     
    }
    if(crossram_State ==1)
    {
    digitalWrite(crossram_in, HIGH);   
  delay(1000);    
  
  if(crossram_State ==1 && slideram_State ==1)
    {Initial_config_front=100;}
    }
  
  }
 //----------Main loop --------------
 
 slideram_State = analogDbounce(slideram_sens);
 crossram_State = analogDbounce(crossram_sens);
 
 //-----Bale lifter ------
 if(slideram_State !=2 && lift_mode ==0)
 {
   digitalWrite(slideram_up, LOW);
   digitalWrite(slideram_down, HIGH);
 }
 if(slideram_State ==2)
 {
  lift_mode=1; 
  digitalWrite(slideram_up, HIGH);
   digitalWrite(slideram_down, HIGH);
 }
 
 if(lift_mode ==1)
 {
   digitalWrite(slideram_up, HIGH);
   digitalWrite(slideram_down, LOW); 
 }
 
 if(slideram_State ==1)
 {
  lift_mode=0; 
 }
 //------ Bale lifter end--
 
 //----- 2-Bale slider -------

 if(digitalDbounce(twobales) == LOW)
 {
    lift_mode=1;
    digitalWrite(crossram_out,LOW);
    digitalWrite(crossram_in,HIGH);
 }
 
 if(crossram_State == 2)
 {
    digitalWrite(crossram_out,LOW);  
    digitalWrite(crossram_in,HIGH);  
 }
 
   if(crossram_State == 1)
 {
    digitalWrite(crossram_out,HIGH);  
    digitalWrite(crossram_in,HIGH);  
 }
  
  
}//--- End of main loop


int analogDbounce(int buttonPin)
{
  if(analogRead(buttonPin) >0)
  {
    buttoncheck =analogRead(buttonPin);
    delay(20);            
  }
  if(buttoncheck >analogRead(buttonPin)-0.5*window && buttoncheck <analogRead(buttonPin)+0.5*window)
  {
  if(analogRead(buttonPin) < window )
  {
    return 0;// Undefined position - 0V         
  }
   if(analogRead(buttonPin) > 510-window && analogRead(buttonPin) < 510+window )
  {
    return 1;// In - 2.5 V
  }
  
   if(analogRead(buttonPin) > 1023-window && analogRead(buttonPin) < 1023+window )
  {
    return 2;// Out - 5V
  }
  }
  else{return 0;}
  
}


int digitalDbounce(int buttonPin)
{ // Usually 0 V
  digital_check =0;
  if(digitalRead(buttonPin) == HIGH)
  { 
    digital_check =1; //- 5V
    delay(20);
  }    
  if(digitalRead(buttonPin) == HIGH && digital_check == 1)
  {return HIGH;}
  else
  {return LOW;}
  
}

