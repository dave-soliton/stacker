/* This is the bale stacker algorithm - all sections */

/* 12V of the black

/*
  This is the pack formation code - five layers of eight = 50
 
 PIN   RELAY J  RAM
 53    J1        Top block
 23    J2        Grab down
 35    J3        Grab up
 25    J4        Grab aligne
 27    J5        grab unaligne
 29    J6        Grab front
 31    J7        Grab back
 33    J8        UnGrab
 37    J9        Grab
 39    J10       Slide down
 41    J11       Slide up
 43    J12       Tip down
 45    J13       Tip up
 47    J14       2 back
 49    J15       2 out
 51    J16       Bottom block
 */
 //---Bale Sensor -------
const int pack_formed = 24;//High = new pack
 //---Discharge Sensor -------
const int Discharge = 40; //High = in correct position to work.
//----Back end tipping inputs------
const int tip =22;
const int crowd= 36;
 //---Ram switches ------
const int lift_sens_2 =32; //High = layer 2 - Can only drive to layer 2.
const int lift_sens_3f =34; //High = layer 3/flat eight 
const int lift_sens_4 =28; //High = layer 4
const int lift_sens_5 =30; //High = layer 5/top
const int front_back_sens =A2;// 0 undefined, 2.5 V front, 5V rear.
const int grab_sens =A1;//0 Undefined, 2.5V in, 5V relesed.
const int grab_rotate_sens =A0;//0 Undefined, 2.5V Aligned, 5V perpendicular.
//----Ram outputs----------
const int liftram_UP=35;  //--Drives grab up
const int liftram_DOWN=23;//--Drives grab down
const int front_back_FRONT=29;// Drives grab to the front
const int front_back_BACK=31;// Drives the grab to the back
const int grab_GRAB=37; // Drives the grab to grab
const int grab_RELEASE=33; // Releases the grab
const int grab_rotate_ALIGNE=25; // Rotates the grab clockwise
const int grab_rotate_PERPEN=27; // Rotates the grab anticlockwise
const int tip_ram_out = 43; // Tips the rear section.
const int tip_ram_in = 45; //Crowds the rear section.
//----Ram outputs----------
int slideram_up=41;
int slideram_down=39;
int crossram_in=47;
int crossram_out=49;
//----Block outputs ----
const int top_block=53;
const int bottom_block=51;

int window=200;
//-----Ram States ------------
int liftram_State =0; //0 undefined, n =layer
int front_back_State =0;// 0 undefined, 2.5 V front, 5V rear.
int grab_State =0;//0 Undefined, 2.5V in, 5V relesed.
int grab_rotate_State =0;//0 Undefined, 2.5V Clockwise, 5V anticlockwise.
int tipping_so_stop=0; //Interupts tipping
int pack_formed_State =0;

int flat_eight_count =0;
int input_count =0;
int Initial_config_back =20;
int Initial_config_count =20;
int Starting_reset =0;
int Discharge_State = 0;
int buttonState =0;
int buttoncheck =0;//-- Used in D-bounce
int digital_check =0; //--Used in D-bounce
int main_count =0;
int aligne_count =0;
int Layer =0;
int Starting_config =0;

 //---Bale Sensor -------
const int twobales = 26; // This pin sences two bales 
 //---Ram switches ------
const int slideram_sens =A4; //  Lifts bales up 0=Undefined, 2=top,1=bottom
const int crossram_sens =A3; // Moves two bales 2 =out, 1=in, 0=undefined.
int twobales_State =0;



int Initial_config_front =20;


int slideram_State=0;
int crossram_State=0;
int lift_mode =0;
int slide_mode =0;
int incomingByte=0;

int write_a =0;
int write_b =0;
int write_c =0;
int write_d= 0;
int run =0;

void setup() {  

  Serial.begin(9600); 

  // prints title with ending line break 
  //Serial.println("ASCII Table ~ Character Map"); 
  
//-----Digital input ----
pinMode(Discharge, INPUT); //---Interupt --- 
pinMode(tip, INPUT);
pinMode(crowd, INPUT);
  //---Analog inputs --------
pinMode(lift_sens_2, INPUT);
pinMode(lift_sens_3f, INPUT);
pinMode(lift_sens_4, INPUT);
pinMode(lift_sens_5, INPUT);
pinMode(front_back_sens, INPUT);
pinMode(grab_sens, INPUT);
pinMode(grab_rotate_sens,INPUT);
  
  //----Ram outputs----------
pinMode(liftram_UP,OUTPUT);
pinMode(liftram_DOWN,OUTPUT);
pinMode(front_back_FRONT,OUTPUT);
pinMode(front_back_BACK,OUTPUT);
pinMode(grab_GRAB,OUTPUT);
pinMode(grab_RELEASE,OUTPUT);
pinMode(grab_rotate_ALIGNE,OUTPUT);
pinMode(grab_rotate_PERPEN,OUTPUT);
pinMode(tip_ram_out,OUTPUT);
pinMode(tip_ram_in,OUTPUT); 

//----Sets ALL rams HIGH ------
  //----Ram outputs----------
digitalWrite(liftram_UP,HIGH);
digitalWrite(liftram_DOWN,HIGH);
digitalWrite(front_back_FRONT,HIGH);
digitalWrite(front_back_BACK,HIGH);
digitalWrite(grab_GRAB,HIGH);
digitalWrite(grab_RELEASE,HIGH);
digitalWrite(grab_rotate_ALIGNE,HIGH);
digitalWrite(grab_rotate_PERPEN,HIGH);
digitalWrite(tip_ram_out,HIGH);
digitalWrite(tip_ram_in,HIGH);
 
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
pinMode(top_block, OUTPUT);
pinMode(bottom_block, OUTPUT);
digitalWrite(top_block,HIGH);
digitalWrite(bottom_block,HIGH);
digitalWrite(slideram_up, HIGH);
   digitalWrite(slideram_down, HIGH);
   digitalWrite(crossram_out,HIGH);
   digitalWrite(crossram_in,HIGH);
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
    digitalWrite(slideram_up, HIGH);
    digitalWrite(bottom_block,LOW);      
    }
    if(slideram_State ==1)
    {
    digitalWrite(slideram_down, HIGH); 
   // digitalWrite(bottom_block,HIGH);  
  //delay(1000);  
   
    }
  
    
     if(crossram_State !=1)
    {
    digitalWrite(crossram_in, LOW); 
    digitalWrite(crossram_out, HIGH); 
    digitalWrite(bottom_block,LOW);    
    }
    if(crossram_State ==1)
    {
    digitalWrite(crossram_in, HIGH); 
   // digitalWrite(bottom_block,HIGH);  
  //delay(1000);    
  
  if(crossram_State ==1 && slideram_State ==1)
    {Initial_config_front=100;Serial.println("flat eight ready");digitalWrite(bottom_block,HIGH); }
    }
  
  }
  //----- Places pack rams into inital positions 
  
while(Initial_config_back ==0)
  { 
    if(digitalRead(tip)==HIGH)
    {digitalWrite(bottom_block,LOW); digitalWrite(tip_ram_out,LOW);
     Serial.println(digitalRead(tip));}
    else
    {digitalWrite(bottom_block,HIGH); digitalWrite(tip_ram_out,HIGH);}
    
    if(digitalRead(crowd)==HIGH)
    {digitalWrite(bottom_block,LOW); digitalWrite(tip_ram_in,LOW);}
    else
    {digitalWrite(bottom_block,HIGH); digitalWrite(tip_ram_in,HIGH);}

    
    
    
    if(digitalRead(Discharge)==HIGH)// only allow initialisation when in crowd position
  {
liftram_State =lift_Dbounce_int_ass();
front_back_State =analogDbounce(front_back_sens);
grab_State =analogDbounce(grab_sens);
grab_rotate_State =analogDbounce(grab_rotate_sens);

    if(liftram_State !=5 && Initial_config_count == 0)
    {
    digitalWrite(liftram_UP, LOW); 
    digitalWrite(top_block,LOW);    
    }
    if(liftram_State ==5)
    {
    digitalWrite(liftram_UP, HIGH); 
    digitalWrite(top_block,HIGH); 
   Initial_config_count = 1; // This forces the grab to be lifted first
 // delay(1000);  
  Serial.println("Grab UP");
   
    }
  
     if(front_back_State !=1&& Initial_config_count == 1) 
    {
    digitalWrite(front_back_FRONT, LOW);  
    digitalWrite(top_block,LOW);   
    }
    if(front_back_State ==1 && Initial_config_count ==1)
    {
    digitalWrite(front_back_FRONT, HIGH);  
   digitalWrite(top_block,HIGH); 
    Initial_config_count = 2;//This forces the grab to be moved to the front second
  //delay(1000);    
  Serial.println("Grab at FRONT");
    }

  
  if(grab_rotate_State !=1&& Initial_config_count == 2) // When the grab is raised and at the front it can be rotated and the grab retracted at the same time.
    {
    digitalWrite(grab_rotate_ALIGNE, LOW);  
    digitalWrite(top_block,LOW);   
    }
    if(grab_rotate_State ==1 && Initial_config_count == 2)
    {
    digitalWrite(grab_rotate_ALIGNE, HIGH);
    digitalWrite(top_block,HIGH);   
  //delay(1000); 
  Serial.println("Grab Aligned");
    }
    
    
  if(grab_State !=1&& Initial_config_count == 2 ) // When the grab is raised and at the front it can be rotated and the grab retracted at the same time.
    {
    digitalWrite(grab_RELEASE, LOW);
    digitalWrite(top_block,LOW);    
    }
    if(grab_State ==1&&Initial_config_count == 2)
    {
    digitalWrite(grab_RELEASE, HIGH);
    //digitalWrite(top_block,HIGH);   
 // delay(1000);
  Serial.println("Grab OPEN");
    }
  
  if(grab_State ==1 && grab_rotate_State ==1 && front_back_State ==1 && liftram_State ==5)
    {
      Initial_config_back=10;//Can only exit While loop when all impliments are in correct position
    digitalWrite(top_block,HIGH);
  }
  }
  }
  
  
 while(Initial_config_back ==10 && Starting_config ==0)
 {Serial.println("Waiting for discharge");
  if(digitalRead(tip)==HIGH)
    {digitalWrite(bottom_block,LOW); digitalWrite(tip_ram_out,LOW); digitalWrite(tip_ram_in,HIGH);
     Serial.println(digitalRead(tip));}
   /* else
    {digitalWrite(bottom_block,HIGH); digitalWrite(tip_ram_out,HIGH);}
    */
    if(digitalRead(crowd)==HIGH)
    {digitalWrite(bottom_block,LOW); digitalWrite(tip_ram_in,LOW);digitalWrite(tip_ram_out,HIGH);}
    if(digitalRead(crowd)==LOW && digitalRead(tip)==LOW)
    {digitalWrite(bottom_block,HIGH); digitalWrite(tip_ram_in,HIGH);digitalWrite(tip_ram_out,HIGH);}
  /*  else
    {digitalWrite(bottom_block,HIGH); digitalWrite(tip_ram_in,HIGH);}*/
  if(digitalDbounce(Discharge) == LOW)
  {Discharge_State =1;Serial.println("Pack out");}
  if(digitalDbounce(Discharge) == HIGH && Discharge_State ==1)
  {Starting_config = 100; Layer = 0;Serial.println("INITAILISED");
digitalWrite(bottom_block,HIGH); digitalWrite(tip_ram_in,HIGH);digitalWrite(tip_ram_out,HIGH); 
}
 
 }
 
 //----------Main loop --------------
  liftram_State =lift_Dbounce_int_ass();
  front_back_State =analogDbounce(front_back_sens);
  grab_State =analogDbounce(grab_sens);
  grab_rotate_State =analogDbounce(grab_rotate_sens);
  
 slideram_State = analogDbounce(slideram_sens);
 crossram_State = analogDbounce(crossram_sens);
 
 //---Tipping algorithm --------
 if(analogDbounce(front_back_sens)!=2)
 {
 if(digitalRead(tip)==HIGH)
    {tipping_so_stop =10;
  Serial.print("tip requesr");}
}
 
 if(tipping_so_stop ==10)
 {
  Serial.print(tipping_so_stop);
digitalWrite(liftram_UP,HIGH);
digitalWrite(liftram_DOWN,HIGH);
digitalWrite(front_back_FRONT,HIGH);
digitalWrite(front_back_BACK,HIGH);
digitalWrite(grab_GRAB,HIGH);
digitalWrite(grab_RELEASE,HIGH);
digitalWrite(grab_rotate_ALIGNE,HIGH);
digitalWrite(grab_rotate_PERPEN,HIGH);
digitalWrite(tip_ram_out,HIGH);
digitalWrite(tip_ram_in,HIGH);
digitalWrite(top_block,HIGH);
if(digitalRead(tip)==HIGH)
    {digitalWrite(bottom_block,LOW); digitalWrite(tip_ram_out,LOW); digitalWrite(tip_ram_in,HIGH);
     Serial.println(digitalRead(tip));}
   /* else
    {digitalWrite(bottom_block,HIGH); digitalWrite(tip_ram_out,HIGH);}
    */
    if(digitalRead(crowd)==HIGH)
    {digitalWrite(bottom_block,LOW); digitalWrite(tip_ram_in,LOW);digitalWrite(tip_ram_out,HIGH);}
    if(digitalRead(crowd)==LOW && digitalRead(tip)==LOW)
    {digitalWrite(bottom_block,HIGH); digitalWrite(tip_ram_in,HIGH);digitalWrite(tip_ram_out,HIGH);}
   
if(digitalDbounce(Discharge)==HIGH && digitalRead(tip)!=HIGH){tipping_so_stop =0;digitalWrite(bottom_block,HIGH); digitalWrite(tip_ram_in,HIGH);digitalWrite(tip_ram_out,HIGH);}
 Layer = 0;}

 else
 {
   Serial.print("Ready"); 
    if(run ==0)
  {
   
  while(input_count ==0)
{
          if(write_a==0)
          { 
            Serial.print("\n slideram_State \n");
          write_a=10;
          }
        if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();
                switch (incomingByte) {
                case 'o':    
                Serial.print("slideram: out (2) \n");
                slideram_State = 2;
                input_count =1;
                break;
                case 'i':    
                Serial.print("slideram: in (1)\n");
                slideram_State =1;
                input_count =1;
                break;
                
                }
        }
        
}

 while(input_count ==1)
{
          if(write_b==0)
          { Serial.print("\n crossram_State \n");
          write_b=10;
          }
        if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();
                switch (incomingByte) {
                case 'o':    
                Serial.print("crossram: out (2)\n");
                crossram_State = 2;
                input_count =2;
                break;
                
                case 'i':    
                Serial.print("crossram: in (1)\n");
                crossram_State =1;
                input_count =2;
                break;
                
                }
        }
        
}

 while(input_count ==2)
{
          if(write_c==0)
          { Serial.print("\n two bale \n");
          write_c=10;
          }
        if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();
                switch (incomingByte) {
                case 'y':    
                Serial.print("two bale: yes (HIGH)\n");
                twobales_State = HIGH;
                input_count =3;
                break;
                
                case 'n':    
                Serial.print("two bale: no (LOW)\n");
                twobales_State = LOW;
                input_count =3;
                break;
                
                }
        }
        
}

 while(input_count ==3)
{
          if(write_d==0)
          { Serial.print("\n pack_formed \n");
          write_d=10;
          }
        if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();
                switch (incomingByte) {
                case 'y':    
                Serial.print("pack_formed: yes (HIGH) \n \n----\n");
                pack_formed_State =HIGH;
                input_count =0, write_a=write_b=write_c=write_b=0,run=1; 
                break;
                case 'n':    
                Serial.print("pack_formed: no (LOW)\n \n-----\n");
                pack_formed_State =LOW;
                input_count =0, write_a=write_b=write_c=write_d=0,run=1; 
                break;
               
                }
        }
        
}}
 if(run ==1)
{ 

//Serial.print("dog");
/*  
 slideram_State = analogDbounce(slideram_sens);
 crossram_State = analogDbounce(crossram_sens);
 pack_formed_State = digitalDbounce(pack_formed);
*/
  

  if(twobales_State == HIGH && flat_eight_count ==0)
 {
   Serial.print("slideram_up: LOW \n");
   Serial.print("slideram_down: HIGH \n"); 
   Serial.print("bottom_block: LOW \n");
   flat_eight_count=1;
   Serial.print("flat eight count:");
            Serial.print(flat_eight_count);
           
   run =0;
 }
  
  if(flat_eight_count ==1 && slideram_State == 2)
  {
   Serial.print("slideram_up: HIGH \n");
   Serial.print("slideram_down: HIGH \n"); 
   Serial.print("crossram_out: LOW \n");
   Serial.print("crossram_in: HIGH \n");
   Serial.print("bottom_block: LOW \n");
    flat_eight_count =2;
    Serial.print("flat eight count:");
            Serial.print(flat_eight_count);
    run =0;
  }
  
    if(pack_formed_State == HIGH && flat_eight_count ==2)
  { 
   crossram_State =2; flat_eight_count =2;
   Serial.print("\n INTERUPTION \n");
  }
  
    if(flat_eight_count ==2 && crossram_State == 2)
  { 
   Serial.print("crossram_out: HIGH \n");
   Serial.print("crossram_in: LOW \n");
   Serial.print("bottom_block: LOW \n");
    flat_eight_count =3;
    Serial.print("flat eight count:");
            Serial.print(flat_eight_count);
    run =0;
  }
  
      
  
  
     if(flat_eight_count ==3 && crossram_State == 1)
  { 
   Serial.print("crossram_out: HIGH \n");
   Serial.print("crossram_in: HIGH \n");
   Serial.print("bottom_block: HIGH \n");
    flat_eight_count =4;
    Serial.print("flat eight count:");
            Serial.print(flat_eight_count);
    run =0;
  }
  
 
  
      if(flat_eight_count ==4 )
  { 
  Serial.print("slideram_up: HIGH \n");
   Serial.print("slideram_down: LOW \n");
   Serial.print("bottom_block: LOW \n");
    flat_eight_count =5;
    Serial.print("flat eight count:");
            Serial.print(flat_eight_count);
    run =0;
  }
  
      if(flat_eight_count ==5 && slideram_State == 1 )
  { 
  Serial.print("slideram_up: HIGH \n");
   Serial.print("slideram_down: HIGH \n");
   Serial.print("bottom_block: HIGH \n");
    flat_eight_count =0;
    Serial.print("flat eight count:");
            Serial.print(flat_eight_count);
    run = 0;
  }
  
}
   
   
/*
  if(digitalDbounce(twobales) == HIGH && flat_eight_count ==0)
 {
   digitalWrite(slideram_up, LOW);
   digitalWrite(slideram_down, HIGH); 
   digitalWrite(bottom_block,LOW);
   flat_eight_count=1;
 }
  
  if(flat_eight_count ==1 && slideram_State == 2)
  {
   digitalWrite(slideram_up, HIGH);
   digitalWrite(slideram_down, HIGH); 
   digitalWrite(crossram_out,LOW);
   digitalWrite(crossram_in,HIGH);
   digitalWrite(bottom_block,LOW);
    flat_eight_count =2;
  }
  
    if(digitalRead(pack_formed) == HIGH && flat_eight_count ==2)
  { 
   crossram_State =2; flat_eight_count =2;
  }
  
    if(flat_eight_count ==2 && crossram_State == 2)
  { 
   digitalWrite(crossram_out,HIGH);
   digitalWrite(crossram_in,LOW);
   digitalWrite(bottom_block,LOW);
    flat_eight_count =3;
  }
  
      
  
  
     if(flat_eight_count ==3 && crossram_State == 1)
  { 
   digitalWrite(crossram_out,HIGH);
   digitalWrite(crossram_in,HIGH);
   digitalWrite(bottom_block,HIGH);
    flat_eight_count =4;
  }
  
 
  
      if(flat_eight_count ==4 )
  { 
  digitalWrite(slideram_up, HIGH);
   digitalWrite(slideram_down, LOW);
   digitalWrite(bottom_block,LOW);
    flat_eight_count =5;
  }
  
      if(flat_eight_count ==5 && slideram_State == 1 )
  { 
  digitalWrite(slideram_up, HIGH);
   digitalWrite(slideram_down, HIGH);
   digitalWrite(bottom_block,HIGH);
    flat_eight_count =0;
  }
  */
//---------------Pack Stacking -----------
 if(digitalRead(Discharge)==LOW)
 {
   Layer =0;
   digitalWrite(top_block,HIGH);
digitalWrite(liftram_UP,HIGH);
digitalWrite(liftram_DOWN,HIGH);
digitalWrite(front_back_FRONT,HIGH);
digitalWrite(front_back_BACK,HIGH);
 }
 else
 { 
 Serial.println("Start of main loop");
 //delay(5000);
 if( digitalDbounce(pack_formed) == HIGH && main_count ==0)
 {main_count=1;}
 Serial.println(main_count);
 //---Lower grab to pack -----
 if(main_count ==1)
 {
   digitalWrite(liftram_DOWN, LOW);
   digitalWrite(liftram_UP, HIGH);
   Serial.println("Lower grab");
   digitalWrite(top_block,LOW);
 }
 //---Stop grab going down ---
 if(liftram_State == 3 && main_count ==1)
 {
   digitalWrite(liftram_DOWN, HIGH);
   digitalWrite(liftram_UP, HIGH);
   digitalWrite(top_block,HIGH);
   main_count=2;
   Serial.println("Stop grab");
  
 }
 
  //--- grab pack -----
 if(main_count ==2)
 {
   digitalWrite(grab_GRAB, LOW);
   digitalWrite(grab_RELEASE, HIGH);
   digitalWrite(top_block,LOW);
     Serial.println("grab pack");
 }
 //--Stop grabing pack - ie stop driving the rams --
  if(main_count ==2 && grab_State ==2)
 {
   digitalWrite(grab_GRAB, HIGH);
   digitalWrite(grab_RELEASE, HIGH);
   digitalWrite(top_block,HIGH);
   main_count = 3;
     Serial.println("Stop grabing");
 }
 //-- Raise the grab + bales --
 if(main_count ==3)
 {
   digitalWrite(liftram_DOWN, HIGH);
   digitalWrite(liftram_UP, LOW);
   digitalWrite(top_block,LOW);
     Serial.println("Raise gab");
 }
  //---Stop raising the grab ---
 if(liftram_State == 5 && main_count ==3)
 {
   digitalWrite(liftram_DOWN, HIGH);
   digitalWrite(liftram_UP, HIGH);
   digitalWrite(top_block,HIGH);
   main_count=4;
 }
 if(main_count ==4)
 {
 if(Layer == 0 || Layer ==2 ||Layer ==4) // Do nothing - Layer is the number of layers already in the back.
    {main_count = 5;}
 if(Layer == 1 ||Layer == 3)// Rotate grab to be pern
    {
      digitalWrite(grab_rotate_ALIGNE,HIGH);
      digitalWrite(grab_rotate_PERPEN,LOW);
      digitalWrite(top_block,LOW);
    }
 }
  if(main_count ==4 && grab_rotate_State == 2)
  {
      digitalWrite(grab_rotate_ALIGNE,HIGH);
      digitalWrite(grab_rotate_PERPEN,HIGH);
      digitalWrite(top_block,HIGH);
      main_count =5; //---- Can now move grab to back.
  }
   //---Move the grab to the back ---
 if(main_count ==5)
 {
   digitalWrite(front_back_BACK, LOW); 
   digitalWrite(front_back_FRONT, HIGH);
  digitalWrite(top_block,LOW); 
 }
 if(main_count ==5 && front_back_State ==2 )
 {
   digitalWrite(front_back_BACK, HIGH); 
   digitalWrite(front_back_FRONT, HIGH); 
   digitalWrite(top_block,HIGH);
   main_count =6; // Now at the back.
 }
 //--- Lower flat eight into pack
  if(main_count ==6 && Layer != 4)
 {
   digitalWrite(liftram_DOWN, LOW);
   digitalWrite(liftram_UP, HIGH);
   digitalWrite(top_block,LOW);
 }
 
 if(liftram_State==2 && Layer ==0 && main_count == 6)
 {
   digitalWrite(liftram_DOWN, HIGH);
   digitalWrite(liftram_UP, HIGH); 
   digitalWrite(top_block,HIGH);
  main_count = 7; 
 }
 
  if(liftram_State==2 && Layer ==1 && main_count == 6)// Same as the previouse (can only go to layer 2)
 {
   digitalWrite(liftram_DOWN, HIGH);
   digitalWrite(liftram_UP, HIGH); 
   digitalWrite(top_block,HIGH); 
    main_count = 7; 
 }
 
    if(liftram_State==3 && Layer ==2 && main_count == 6)
 {
   digitalWrite(liftram_DOWN, HIGH);
   digitalWrite(liftram_UP, HIGH); 
   digitalWrite(top_block,HIGH); 
    main_count = 7; 
 }
 
  if(liftram_State==4 && Layer ==3 && main_count == 6)
 {
   digitalWrite(liftram_DOWN, HIGH);
   digitalWrite(liftram_UP, HIGH); 
   digitalWrite(top_block,HIGH); 
    main_count = 7; 
 }
 
  if(liftram_State==5 && Layer ==4 && main_count == 6)
 {
   digitalWrite(liftram_DOWN, HIGH);
   digitalWrite(liftram_UP, HIGH);  
   digitalWrite(top_block,HIGH);
    main_count = 7; 
 }
 // End of grab lowering - this could have been written more compactly
 // but I have chosen not to. This is to avoid ram collision.
 
 //----Release bales in the back NOW incriment layer.----
   if(main_count ==7)
 {
   digitalWrite(grab_GRAB, HIGH);
   digitalWrite(grab_RELEASE, LOW);
   digitalWrite(top_block,LOW);
 }
 //--Stop releasing pack - ie stop driving the rams --
  if(main_count ==7 && grab_State ==1)
 {
   digitalWrite(grab_GRAB, HIGH);
   digitalWrite(grab_RELEASE, HIGH);
   digitalWrite(top_block,HIGH);
   main_count = 8;
   ++Layer; // NOW incriment layer.
 }
 
 //-- Raise the grab, NO bales --
 if(main_count ==8)
 {
   digitalWrite(liftram_DOWN, HIGH);
   digitalWrite(liftram_UP, LOW);
   digitalWrite(top_block,LOW);
 }
  //---Stop raising the grab ---
 if(liftram_State == 5 && main_count ==8)
 {
   digitalWrite(liftram_DOWN, HIGH);
   digitalWrite(liftram_UP, HIGH);
   digitalWrite(top_block,HIGH);
   main_count=9;
 }
 
 //--- Move the grab to the front ---
 if(main_count ==9)
 {
   digitalWrite(front_back_BACK, HIGH); 
   digitalWrite(front_back_FRONT, LOW);
  digitalWrite(top_block,LOW); 
 }
 if(main_count ==9 && front_back_State ==1 )
 {
   digitalWrite(front_back_BACK, HIGH); 
   digitalWrite(front_back_FRONT, HIGH);
   digitalWrite(top_block,HIGH); 
   main_count =10; // Now at the front.
 }
 // Align the grab to the bales ----
 if(main_count ==10 && grab_rotate_State != 1 && front_back_State ==1)
  {
      digitalWrite(grab_rotate_ALIGNE,LOW);
      digitalWrite(grab_rotate_PERPEN,HIGH);
      digitalWrite(top_block,LOW);
  }
 if(main_count ==10 && grab_rotate_State == 1)
  {
      digitalWrite(grab_rotate_ALIGNE,HIGH);
      digitalWrite(grab_rotate_PERPEN,HIGH);
      digitalWrite(top_block,HIGH);
      main_count =0; // Restarts stacking proccedure.
  }
  }
 }
}//--- End of main loop


int analogDbounce(int buttonPin)
{
  if(analogRead(buttonPin) >0)
  {
    buttoncheck =analogRead(buttonPin);
    delay(10);            
  }
  if(buttoncheck >analogRead(buttonPin)-0.5*window && buttoncheck <analogRead(buttonPin)+0.5*window)
  {
  if(analogRead(buttonPin) < window )
  {
    return 0;// Undefined position            
  }
   if(analogRead(buttonPin) > 510-window && analogRead(buttonPin) < 510+window )
  {
    return 1;// In
  }
  
   if(analogRead(buttonPin) > 1023-window && analogRead(buttonPin) < 1023+window )
  {
    return 2;// Out
  }
  }
  else{return 0;}
  
}


int digitalDbounce(int buttonPin)
{ digital_check =0;
  if(digitalRead(buttonPin) == HIGH)
  { 
    digital_check =1;
    delay(10);
  }    
  if(digitalRead(buttonPin) == HIGH && digital_check == 1)
  {return HIGH;}
  else
  {return LOW;}
  
}


//----Digital lift position dbounce and position integer assignment
int lift_Dbounce_int_ass()
{
buttoncheck =0;
  if(digitalRead(lift_sens_2) == HIGH ||digitalRead(lift_sens_3f) == HIGH ||
  digitalRead(lift_sens_4) == HIGH ||digitalRead(lift_sens_5) == HIGH)
  {
    buttoncheck =1;
    delay(10);      
 
  }
  
  if( buttoncheck ==0)
  {
    return 0;// Undefined position            
  }
  
   if( buttoncheck ==1 && digitalRead(lift_sens_2) == HIGH )
  {
    return 2;// 2
  }
  
   if( buttoncheck ==1 && digitalRead(lift_sens_3f) == HIGH )
  {
    return 3;// 3 // Also flat eight formation level.
  }
   if( buttoncheck ==1 && digitalRead(lift_sens_4) == HIGH )
  {
    return 4;// 4
  }
   if( buttoncheck ==1 && digitalRead(lift_sens_5) == HIGH )
  {
    return 5;// 5
  }
}
 

