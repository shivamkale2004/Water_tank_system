
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);//Declaring LCD address and config

#define button_B    9
#define button_D    8
#define button_U    7
#define button_E    6


const int trigpin= 12;
const int echopin= 0;
const int Buzzer= 3;
const int WtrLvlLED=2;
const int TdsLED=4;
const int TurbLED=13;
const int phLED=5;
int i=0;
int lvl;

#define DEFAULT_DELAY 300


char buttonPressed = '0';

byte menuLevel = 0;     // Level 0: no menu display, display anything you like
                        // Level 1: display main menu
                        // Level 2: display sub menu

byte menu = 1;
byte sub = 1;

float MINph = 0.;
float  MAXph = 0.;
float  MINtds = 0.;
float  MAXtds = 0.;
float  MINturb = 0.;
float  MAXturb = 0.;
float  MAXwtr = 10.;
float  lvl0 = 0.;


bool currState_B = HIGH;
bool currState_D = HIGH;
bool currState_U = HIGH;
bool currState_E = HIGH;
          
bool prevState_B = HIGH; 
bool prevState_D = HIGH; 
bool prevState_U = HIGH; 
bool prevState_E = HIGH; 

unsigned long prevTime_B = 0;
unsigned long prevTime_D = 0;
unsigned long prevTime_U = 0;
unsigned long prevTime_E = 0;

unsigned long waitTime_B = 50;
unsigned long waitTime_D = 50;
unsigned long waitTime_U = 50;
unsigned long waitTime_E = 50;

#define rxPin 11
#define txPin 10
SoftwareSerial Arduino_SoftSerial(rxPin,txPin);//RX TX





long duration;
int distance;
float Percentage;
int del[]={10,50,100,250,500,1000,2000,3000};//Delay array

//temp variables
#define pH_sensor A3
float pH_value=0.,Sensor_value=0.;
float TDS=45.89;
float TURB=34.32;

byte charUp[8] = {
        0b00100,
        0b01110,
        0b11111,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000
};
byte charDown[8] = {
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b11111,
        0b01110,
        0b00100
};
byte charUpDown[8] = {
        0b00100,
        0b01110,
        0b11111,
        0b00000,
        0b00000,
        0b11111,
        0b01110,
        0b00100
};
//Custom test character
byte test[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
  };
//Custom blank character
 byte blank[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
 };


void setup() {
  lcd.backlight();//Turning on backlight
  lcd.init();//Initializing LCD
  
  lcd.createChar(0, charUp);
  lcd.createChar(1, charDown);
  lcd.createChar(2, charUpDown);
   lcd.createChar(3, test);//Character for test
  lcd.createChar(4, blank);//Character for blanks 

 pinMode(trigpin,OUTPUT);

 pinMode(echopin,INPUT);

 pinMode(Buzzer,OUTPUT);
  
 pinMode(WtrLvlLED,OUTPUT);  

 pinMode(TdsLED,OUTPUT); 

 pinMode(phLED,OUTPUT); 

 pinMode(TurbLED,OUTPUT); 

 pinMode(10, INPUT);

 pinMode(11, OUTPUT);
  
  
  Arduino_SoftSerial.begin(9600);//Communication with Arduino-NodeMcu

  Serial.begin(9600);

  pinMode(button_B, INPUT_PULLUP);
  pinMode(button_D, INPUT_PULLUP);
  pinMode(button_U, INPUT_PULLUP);
  pinMode(button_E, INPUT_PULLUP);

  pinMode(A4,INPUT);

  

  lcd.setCursor(0,0);
  lcd.print(F("******WELCOME!******"));
  lcd.setCursor(0,1);
  lcd.print(F("Initialinzing."));
  lcd.setCursor(0,1);
  digitalWrite(phLED,HIGH);
  delay(del[3]);
  digitalWrite(phLED,LOW);
  delay(del[3]);
  lcd.print(F("Initialinzing.."));
  lcd.setCursor(0,1);
  digitalWrite(TdsLED,HIGH);
  delay(del[3]);
  digitalWrite(TdsLED,LOW);
  delay(del[3]);
  lcd.setCursor(0,1);
  lcd.print(F("Initialinzing..."));
  digitalWrite(TurbLED,HIGH);
  delay(del[3]);
  digitalWrite(TurbLED,LOW);
  delay(del[3]);
  lcd.setCursor(0,1);
  lcd.print(F("Initialinzing...."));
  digitalWrite(WtrLvlLED,HIGH);
  delay(del[3]);
  digitalWrite(WtrLvlLED,LOW);
  delay(del[35]);
  lcd.setCursor(0,1);
  lcd.print(F("Initialinzing....."));
  delay(del[3]);
  lcd.setCursor(0,1);
  lcd.print(F("Initialinzing......"));
  sound();
  lcd.setCursor(0,1);
  lcd.print(F("Initialinzing......."));
  delay(del[3]);
 
 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("System.Ver1.0"));
  delay(del[5]);
  lcd.clear();
 for(i=0;i<=19;i++)//For loop for LED check
  { lcd.setCursor(0+i, 0);  
    lcd.write(3);
    lcd.setCursor(0+i, 1);
    lcd.write(3);
    lcd.setCursor(0+i, 2);
    lcd.write(3);
    lcd.setCursor(0+i, 3);
    lcd.write(3);
    delay(del[2]);          
  }
  lcd.clear();
  lcd.print(F("LCD OK!"));
  delay(del[4]);
  lcd.clear();
  
  showHomeScreen();
}

void loop() {

  checkButton();
  Arduino_SoftSerial.print(pH_value,5);   Arduino_SoftSerial.print("A");
    Arduino_SoftSerial.print(TDS,5);   Arduino_SoftSerial.print("B");
    Arduino_SoftSerial.print(TURB,5);   Arduino_SoftSerial.print("C");
    Arduino_SoftSerial.print(Percentage,5);   Arduino_SoftSerial.print("D");

  // You can do other things here below

  
}

void checkButton() {

  // Button Debouncing
  bool currRead_B = digitalRead(button_B);
  bool currRead_D = digitalRead(button_D);
  bool currRead_U = digitalRead(button_U);
  bool currRead_E = digitalRead(button_E);

  if (currRead_B != prevState_B) {
    prevTime_B = millis();
  }
  if (currRead_D != prevState_D) {
    prevTime_D = millis();
  }
  if (currRead_U != prevState_U) {
    prevTime_U = millis();
  }
  if (currRead_E != prevState_E) {
    prevTime_E = millis();
  }

  if ((millis() - prevTime_B) > waitTime_B) {
    if (currRead_B != currState_B) {
      currState_B = currRead_B;
      if (currState_B == LOW) {
        buttonPressed = 'B';
      } 
    }
  } else buttonPressed = '0';
  if ((millis() - prevTime_D) > waitTime_D) {
    if (currRead_D != currState_D) {
      currState_D = currRead_D;
      if (currState_D == LOW) {
        buttonPressed = 'D';
      } 
    }
  } else buttonPressed = '0';
  if ((millis() - prevTime_U) > waitTime_U) {
    if (currRead_U != currState_U) {
      currState_U = currRead_U;
      if (currState_U == LOW) {
        buttonPressed = 'U';
      } else {
        //buttonPressed = '0';
      }
    }
  } else buttonPressed = '0';
  if ((millis() - prevTime_E) > waitTime_E) {
    if (currRead_E != currState_E) {
      currState_E = currRead_E;
      if (currState_E == LOW) {
        buttonPressed = 'E';
      } 
    }
  } else buttonPressed = '0';

  prevState_B = currRead_B;
  prevState_D = currRead_D;
  prevState_U = currRead_U;
  prevState_E = currRead_E;

  processButton(buttonPressed);

}

void processButton(char buttonPressed) {

  switch(menuLevel) {
    case 0: // Level 0
      switch ( buttonPressed ) {
        case 'E': // Enter
          menuLevel = 1;
          menu = 1;
          updateMenu();
          delay(DEFAULT_DELAY);
          break;
        case 'U': // Up
          break;
        case 'D': // Down
          break;
        case 'B': // Back
          break;
        default:
          break;
      }
      break;
    case 1: // Level 1, main menu
      switch ( buttonPressed ) {
        case 'E': // Enter
          updateSub();
          menuLevel = 2;  // go to sub menu
          updateSub();
          delay(DEFAULT_DELAY);
          break;
        case 'U': // Up
          menu++;
          updateMenu();
          delay(DEFAULT_DELAY);
          break;
        case 'D': // Down
          menu--;
          updateMenu();
          delay(DEFAULT_DELAY);
          break;
        case 'B': // Back
          menuLevel = 0;  // hide menu, go back to level 0
          showHomeScreen();
          delay(DEFAULT_DELAY);
          break;
        default:
          break;
      } 
      break;
    case 2: // Level 2, sub menu
      switch ( buttonPressed ) {
        case 'E': 
          menuLevel = 1;
          updateMenu();
          delay(DEFAULT_DELAY);
          break;
        case 'D': // U
          if (menu == 1) {
            if (MINph < 14) {  // 1 hour max
              MINph = MINph + 1;
            } else {
              MINph = 14;
            }
          } else if (menu == 2) {       
            if (MAXph < 14) {  // 1 hour max
              MAXph = MAXph + 1;
            } else {
              MAXph = 14;
            }
          } else if (menu == 3) {
            if (MINtds < 150) {  // 1 hour max  
              MINtds = MINtds + 20;
            } else {
              MINtds = 150;
            }
          } else if (menu == 4) {
            if (MAXtds < 150) {  // 1 hour max  
              MAXtds = MAXtds + 20;
            } else {
              MAXtds = 150;
            }
          } else if (menu == 5) {
            if (MINturb < 1) {  // 1 hour max  
              MINturb = MINturb + 0.20;
            } else {
              MINturb = 1;
            }
          } else if (menu == 6) {
            if (MAXturb < 1) {  // 1 hour max  
              MAXturb = MAXturb + 0.20;
            } else {
              MAXturb = 1;
            }
          } else if (menu == 7) {
            if (MAXwtr < 400) {  // 1 hour max  
              MAXwtr = MAXwtr + 20;
            } else {
              MAXwtr = 400;
            }
          } else if (menu == 8) {
            if (lvl0 < 400) {  // 1 hour max  
              lvl0 = lvl0 + 20;
            } else {
              lvl0 = 400;
            }
          }
          updateSub();
          delay(DEFAULT_DELAY);
          break;
        case 'U': // D
          if (menu == 1) {
            if (MINph == 0) {
              MINph = 0;
            } else {
              MINph = MINph - 0.5;
            }
          } else if (menu == 2) {
            if (MAXph == 0) {
              MAXph = 0;
            } else {
              MAXph = MAXph - 0.5;
            }
          } else if (menu == 3) {
            if (MINtds == 0) {
              MINtds = 0;
            } else {
              MINtds = MINtds - 10;
            }
          } else if (menu == 4) {
            if (MAXtds == 0) {
              MAXtds = 0;
            } else {
              MAXtds = MAXtds - 10;
            }
          } else if (menu == 5) {
            if (MINturb == 0) {
              MINturb = 0;
            } else {
              MINturb = MINturb - 0.10;
            }
          } else if (menu == 6) {
            if (MAXturb == 0) {
              MAXturb = 0;
            } else {
              MAXturb = MAXturb - 0.10;
            }
          } else if (menu == 7) {
            if (MAXwtr == 0) {
              MAXwtr = 0;
            } else {
              MAXwtr = MAXwtr - 10;
            }
          } else if (menu == 8) {
            if (lvl0 == 0) {
              lvl0 = 0;
            } else {
              lvl0 = lvl0 - 10;
            }
          } 
          updateSub();
          delay(DEFAULT_DELAY);
          break;
        case 'B': // L
          menuLevel = 1;  // go back to main menu
          updateMenu();
          delay(DEFAULT_DELAY);
          break;
        default:  
          break;
      } 
      break;
    case 3: // Level 3, sub menu of sub menu
    
      break;
    default:
      break;
  }
  
}

void updateMenu() {
  
  switch (menu) {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">MINph : ");
      lcd.print(MINph);
      lcd.print(" ph");
      lcd.setCursor(0, 1);
      lcd.print(" MAXph ");
      lcd.setCursor(0, 2);
      lcd.print(" MINtds ");
      lcd.setCursor(0, 3);
      lcd.print(" MAXtds ");
      lcd.setCursor(19,3);
      lcd.write((byte)1); 
      break;
    case 2:
      lcd.clear();
      lcd.print(" MINph ");
      lcd.setCursor(0, 1);
      lcd.print(">MAXph : ");
      lcd.print(MAXph);
      lcd.print("ph");
      lcd.setCursor(0, 2);
      lcd.print(" MINtds ");
      lcd.setCursor(0, 3);
      lcd.print(" MAXtds ");
      lcd.setCursor(19,3);
      lcd.write((byte)2); 
      break;
    case 3:
      lcd.clear();
      lcd.print(" MINph ");
      lcd.setCursor(0, 1);
      lcd.print(" MAXph ");
      lcd.setCursor(0, 2);
      lcd.print(">MINtds: ");
      lcd.print(MINtds);
      lcd.print("mg/L");
      lcd.setCursor(0, 3);
      lcd.print(" MAXtds  ");
      lcd.setCursor(19,3);
      lcd.write((byte)2); 
      break;
    case 4:
      lcd.clear();
      lcd.print(" MINph ");
      lcd.setCursor(0, 1);
      lcd.print(" MAXph  ");
      lcd.setCursor(0, 2);
      lcd.print(" MINtds ");
      lcd.setCursor(0, 3);
      lcd.print(">MAXtds: ");
      lcd.print(MAXtds);
      lcd.print("mg/L");
      lcd.setCursor(19,3);
      lcd.write((byte)2); 
      break;
    case 5:
      lcd.clear();
      lcd.print(">MINturb: ");
      lcd.print(MINturb);
      lcd.print("NTU");
      lcd.setCursor(0, 1);
      lcd.print(" MAXturb  ");
      lcd.setCursor(0, 2);
      lcd.print(" MAXwtr  ");
      lcd.setCursor(0, 3);
      lcd.print(" lvl0 ");
      lcd.setCursor(19,3);
      lcd.write((byte)2); 
      break;
    case 6:
      lcd.clear();
      lcd.print(" MINturb");
      lcd.setCursor(0, 1);
      lcd.print(">MAXturb: ");
      lcd.print(MAXturb);
      lcd.print("NTU");
      lcd.setCursor(0, 2);
      lcd.print(" MAXwtr ");
      lcd.setCursor(0, 3);
      lcd.print(" lvl0 ");
      lcd.setCursor(19,3);
      lcd.write((byte)2); 
      break;
    case 7:
      lcd.clear();
      lcd.print(" MINturb");
      lcd.setCursor(0, 1);
      lcd.print(" MAXturb  ");
      lcd.setCursor(0, 2);
      lcd.print(">MAXwtr : ");
      lcd.print(MAXwtr);
      lcd.print("Cm");
      lcd.setCursor(0, 3);
      lcd.print(" lvl0 ");   
      lcd.setCursor(19,3);
      lcd.write((byte)2); 
      break;
    case 8:
      lcd.clear();
      lcd.print(" MINturb");
      lcd.setCursor(0, 1);
      lcd.print(" MAXturb  ");
      lcd.setCursor(0, 2);
      lcd.print(" MAXwtr ");
      lcd.setCursor(0, 3);
      lcd.print(">lvl0   : ");
      lcd.print(lvl0);  
      lcd.print("Cm");    
      lcd.setCursor(19,3);
      lcd.write((byte)2); 
      break;  
    case 9:
      lcd.clear();
      lcd.print(">Monitoring Disp ");
      lcd.setCursor(0, 1);
      lcd.setCursor(19,3);
      lcd.write((byte)0); 
      break;
    case 10:
      menu = 9;
      break;
  }
  
}

void updateSub() {
  switch (menu) {
    case 0:

      break;
    case 1:
      lcd.clear();
      lcd.print(" MIN ph:");
      lcd.setCursor(0, 1);
      lcd.print("  Value = ");
      lcd.print(MINph);
      lcd.print("ph");
      break;
    case 2:
      lcd.clear();
      lcd.print(" MAX ph:");
      lcd.setCursor(0, 1);
      lcd.print("  Value = ");
      lcd.print(MAXph);
      lcd.print("ph");
      break;
    case 3:
      lcd.clear();
      lcd.print(" MIN tds:");
      lcd.setCursor(0, 1);
      lcd.print("  Value = ");
      lcd.print(MINtds);
      lcd.print("mg/L");
      break;
    case 4:
      lcd.clear();
      lcd.print(" MAX tds:");
      lcd.setCursor(0, 1);
      lcd.print("  Value = ");
      lcd.print(MAXtds);
      lcd.print("mg/L");
      break;  
    case 5:
      lcd.clear();
      lcd.print(" MIN turb:");
      lcd.setCursor(0, 1);
      lcd.print("  Value = ");
      lcd.print(MINturb);
      lcd.print("NTU");
      break;  
    case 6:
      lcd.clear();
      lcd.print(" MAX turb:");
      lcd.setCursor(0, 1);
      lcd.print("  Value = ");
      lcd.print(MAXturb);
      lcd.print("NTU");
      break;   
    case 7:
      lcd.clear();
      lcd.print(" MAX wtr:");
      lcd.setCursor(0, 1);
      lcd.print("  Value = ");
      lcd.print(MAXwtr);
      lcd.print("Cm");
      break;  
    case 8:
      lcd.clear();
      lcd.print(" lvl 0:");
      lcd.setCursor(0, 1);
      lcd.print("  Value = ");
      lcd.print(lvl0);
      lcd.print("Cm");
      break;   
    case 9:
      lcd.clear();
      display();
      break;                    
    case 10:
      menu = 9;
      break;
  }
}

void showHomeScreen() {
  lcd.clear();
  lcd.print("    LTCOE's STC5  ");
  lcd.setCursor(0,1);
  lcd.print("-   Press Enter ");
}












void display(){
 


  {
 digitalWrite(trigpin,HIGH);

 delayMicroseconds(10);

 digitalWrite(trigpin,LOW);

 duration=pulseIn(echopin,HIGH);

 distance = duration*0.034/2;

 distance=distance+2;//Correction

  Percentage=distance-MAXwtr;
 Percentage=Percentage/(lvl0-MAXwtr);
 Percentage=Percentage*100;
 Percentage=100-Percentage;
 int lvl=Percentage;

 //========================================================================================
 //||                                    PH CALCULATIONS                                 ||
 //========================================================================================

  lcd.setCursor(0,0);
  lcd.print(F("pH="));
  pH_value=analogRead((pH_sensor));
  pH_value=(Sensor_value-200)/35;

 
  if (pH_value<=MAXph&&pH_value>=MINph)
  {
    lcd.print(pH_value);
    lcd.setCursor(17,0);
    lcd.print("OK!");
  }
  if (pH_value>MAXph)
  {
    lcd.setCursor(3,0);
    lcd.print(pH_value);
    lcd.setCursor(15,0);
    lcd.print(F("HIGH!"));
    digitalWrite(phLED,HIGH);
    delay(del[5]);
    lcd.setCursor(15,0);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    digitalWrite(phLED,LOW);
    delay(del[5]);
  }
  if (pH_value<MINph)
  {
    lcd.setCursor(3,0);
    lcd.print(pH_value);
    lcd.setCursor(16,0);
    lcd.print(F("LOW!"));
    digitalWrite(phLED,HIGH);
    delay(del[5]);
    lcd.setCursor(16,0);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    digitalWrite(phLED,LOW);
    delay(del[5]);
  } 
 //=======================================================================================
 //||                                    TDS CALCULATIONS                               ||
 //=======================================================================================
  lcd.setCursor(0,1);
  lcd.print(F("TDS="));
  if (TDS<=MAXtds&&TDS>=MINtds)
  {
    lcd.print(TDS);
    lcd.print(F("mg/L"));
    lcd.setCursor(17,1);
    lcd.print("OK!");
  }
  if (TDS>MAXtds)
  {
    lcd.setCursor(4,1);
    lcd.print(TDS);
    lcd.print(F("mg/L"));
    lcd.setCursor(15,1);
    lcd.print(F("HIGH!"));
    digitalWrite(TdsLED,HIGH);
    delay(del[5]);
    lcd.setCursor(15,1);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    digitalWrite(TdsLED,LOW);
    
  }
  if (TDS<MINtds)
  {
    lcd.setCursor(4,1);
    lcd.print(TDS);
    lcd.print(F("mg/L"));
    lcd.setCursor(16,1);
    lcd.print(F("LOW!"));
    digitalWrite(TdsLED,HIGH);
    delay(del[5]);
    lcd.setCursor(16,1);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    digitalWrite(TdsLED,LOW);
    delay(del[5]);
    
  } 
 //=========================================================================================
 //||                                  TURBIDITY CALCULATIONS                             ||
 //=========================================================================================
 lcd.setCursor(0,2);
  lcd.print(F("Turb="));
  if (TURB<=MAXturb&&TURB>=MINturb)
  {
    lcd.print(TURB);
    lcd.print(F("NTU"));
    lcd.setCursor(17,2);
    lcd.print("OK!");
  }
  if (TURB>MAXturb)
  {
    lcd.setCursor(5,2);
    lcd.print(TURB);
    lcd.print(F("NTU"));
    lcd.setCursor(15,2);
    lcd.print(F("HIGH!"));
    digitalWrite(TurbLED,HIGH);
    delay(del[5]);
    lcd.setCursor(15,2);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    digitalWrite(TurbLED,LOW);
    delay(del[5]);
  }
  if (TURB<MINturb)
  {
    lcd.setCursor(5,2);
    lcd.print(TURB);
    lcd.print(F("NTU"));
    lcd.setCursor(16,2);
    lcd.print(F("LOW!"));
    digitalWrite(TurbLED,HIGH);
    delay(del[5]);
    lcd.setCursor(16,2);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    digitalWrite(TurbLED,LOW);
    delay(del[5]);
  } 
 //========================================================================================
 //||                                  WTR>LVL CALCULATIONS                               ||
 //=========================================================================================
 
 
  lcd.setCursor(0,3);
  lcd.print(F("Wtr.Lvl="));
  if (lvl<=99&&lvl>=0)
  {
    lcd.print(lvl);
    lcd.print(F("%"));
    lcd.setCursor(17,3);
  }
  if (Percentage>99)
  {
    lcd.setCursor(8,3);
    lcd.print("--");
    lcd.setCursor(14,3);
    lcd.print(F("OvrFlo"));
    digitalWrite(WtrLvlLED,HIGH);
    delay(del[5]);
    lcd.setCursor(14,3);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    digitalWrite(WtrLvlLED,LOW);
    delay(del[5]);
  }
  if (Percentage<0)
  {
    lcd.setCursor(8,3);
    lcd.print("--");
    lcd.setCursor(15,3);
    lcd.print(F("Empty"));
    digitalWrite(WtrLvlLED,HIGH);
    delay(del[5]);
    lcd.setCursor(14,3);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    lcd.write(4);
    digitalWrite(WtrLvlLED,LOW);
    delay(del[5]);
  
  }
   if(pH_value>MAXph||pH_value<MINph||TDS>MAXtds||TDS<MINtds||TURB>MAXturb||TURB<MINturb||Percentage<0||Percentage>99)
    {
      sound();
    }
    Arduino_SoftSerial.print(pH_value,2);   Arduino_SoftSerial.print("A");
    Arduino_SoftSerial.print(TDS,2);   Arduino_SoftSerial.print("B");
    Arduino_SoftSerial.print(TURB,2);   Arduino_SoftSerial.print("C");
    Arduino_SoftSerial.print(Percentage,2);   Arduino_SoftSerial.print("D");





checkButton();
 display();
 


}
   
 }
 void sound(){
   tone(Buzzer,120);
      delay(del[4]);
      tone(Buzzer,130);
      delay(del[4]);
      tone(Buzzer,210);
      delay(del[4]);
      noTone(Buzzer);
 }