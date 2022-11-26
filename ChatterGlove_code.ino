#include "BluetoothSerial.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(22,21,5,18,23,19);
int page_counter=1 ;       //next page


BluetoothSerial SerialBT;
String data;
String data2;
String senddata="";
int k,lungime=0,i=0,j=0;

//.....Pins......//
int up = 13;               //TEXT UP Scroll Down button
int down = 12;           // TEXT DOWN Scroll Up button
//.....debounce functions.....//
boolean current_up = LOW;          
boolean last_up=LOW;            
boolean last_down = LOW;
boolean current_down = LOW;

//Custom UP arrow char
byte UP[8] = {
  0b00100,
  0b01110,
  0b11111,
  0b00100,
  0b00100,
  0b00000,
  0b00000,
  0b00000
};

//Custom DOWN arrow char
byte DOWN[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00100,
  0b00100,
  0b11111,
  0b01110,
  0b00100
};

const int flexPin_mare = 34;   //mare
const int flexPin_mijloc = 32; //mijloc
const int flexPin_inelar = 33; //inelar      
const int flexPin_aratator= 35; //aratator
const int flexPin_mic = 25; //mic            
const float VCC = 5;      // voltage at Ardunio 5V line
const float R_DIV = 47000.0;  // resistor used to create a voltage divider
const float flatResistance = 25000.0; // resistance when flat
const float bendResistance = 100000.0; // resistance at 90 deg
char string[100],h[1]={'H'},e[1]={'E'},l[1]={'L'},o[1]={'O'},b[1]={'B'},v[1]={'V'},k_string[1]={'K'},z[1]={'Z'};
int k1=0;
char Screen1[27]="                         ";
char Screen2[27]="                         ";

char BScreen1[27]="                         ";

int r=1;
int s=1;
int kx=0;
void setup() {
  SerialBT.begin("ChatterGlove"); //Bluetooth device name
  Serial.println("ChatterGlove Online");
  lcd.begin(16,2); 
  lcd.createChar(1, UP);
  lcd.createChar(2, DOWN);
  Serial.begin(115200);
  pinMode(flexPin_mare, INPUT);
  pinMode(flexPin_mijloc, INPUT);
  pinMode(flexPin_inelar, INPUT);
  pinMode(flexPin_aratator, INPUT);
  pinMode(flexPin_mic, INPUT);
  /*for(int g=0;g<27;g++)
  {
    Screen1[g]=' ';
    Screen2[g]=' ';
  }*/
}


void send()
    {
      for(int p=0;p<12;p++)senddata.concat(Screen1[p]);
      for(int l=0;l<14;l++)senddata.concat(Screen2[l]); 
      
      SerialBT.println(senddata);
      senddata="";
    }





boolean debounce(boolean last, int pin)
{
  boolean current = digitalRead(pin);
  if (last != current)
  {
    delay(5);
    current = digitalRead(pin);
  }
  return current;
}
char* split(int r,char sir[27] )
{
char sirout[15];
if (r=1)
{
  for (int i=0;i>12;i++)
  sirout[i]=sir[i];
  
}else
{
  for (int i=12;i>27;i++)
  sirout[i-12]=sir[i];
}
return sirout;


}
void display(char Screen[27])
{
for (int j=3;j++;j<30)
    { int row;
      if (j>15) row=1; else row=0;
      if (row=0)
      {
        lcd.setCursor(j,row);
        lcd.write(Screen[j-3]);
      }
      else
      {
        lcd.setCursor(j-16,row);
        lcd.write(Screen[j-15]);
      }
    }



}
void loop() {
  // Read the ADC, and calculate voltage and resistance from it
  int mare = analogRead(flexPin_mare);
  int mijlociu = analogRead(flexPin_mijloc);
  int inelar = analogRead(flexPin_inelar);
  int aratator = analogRead(flexPin_aratator);
  int mic = analogRead(flexPin_mic);
  
 

  k=0;
  while(SerialBT.available()) {      
    char c = SerialBT.read();  //gets one byte from serial buffer (bluetooth module)
    if (c != -1) {k++;
      if(k<16)
      data += c;
      else
      data2 += c;   // store character into buffer string
      if (c == '\n') { // end of line detected
        lcd.clear(); // clear display
        lcd.setCursor(0, 0);
        lcd.print(data);
           lcd.setCursor(0, 1);
        lcd.print(data2);// show new message on display
        data = "";
        data2 = "";// clear buffer string
        break;
     }
    }
  }  
current_up = debounce(last_up, up);         //Debounce for Page Up or Scroll down button
current_down = debounce(last_down, down);   //Debounce for Page Down or Scroll upbutton

//----Page counter function to move pages----//

//TEXT Up or Scroll Down
    if (last_up== LOW && current_up == HIGH){  //When SCROLL DOWN button is pressed
      lcd.clear();                     //When page is changed, lcd clear to print new page  
      if(page_counter <19){              //Page counter never higher than 19(total of pages)
      page_counter= page_counter +1;   //SCROLL DOWN
      
      }
      else{
      page_counter= 1;  //When clicking scroll down button on the last line, it will bring back to line 1 and 2
      }
  }
  
    last_up = current_up;

//TEXT Down or Scroll Up
  //if ((current_up == HIGH) && (current_down == HIGH)){lcd.setCursor(4, 0);lcd.print("Print Test");}
    if (last_down== LOW && current_down == HIGH){ //When down button is pressed 
      lcd.clear();                     //When page is changed, lcd clear to print new page    
      if(page_counter >1){              //Page counter never lower than 1 (total of pages)
      page_counter= page_counter -1;   //SCROLL UP
      
      }
      else{
      page_counter= 1;  
      }
  }
    
    last_down = current_down;

//------- WRITE ALL PAGE COUNTER---// 
  switch (page_counter) {
   
    case 1:{     //Page 1
      lcd.setCursor(0,0);
      lcd.print("ChatterGlove");
      lcd.setCursor(0,1);
      lcd.print("by Smart::Bits");
      lcd.setCursor(15,1);
      lcd.write(byte(2));
    }
    break;





  case 2:{
    





    lcd.setCursor(0,0);
    lcd.write("CG:");

    if (mare>=3400 && mare<=3500 && aratator>=3600 && mijlociu>=3600 && inelar>=3600 && mic>=3600) {delay(3000);if (kx>11)Screen2[kx-12]=('B'); else Screen1[kx]=('B');kx++;}if (kx>26)s=2;
    if (mare<=3500 && aratator<=3500 && mijlociu<=3500 && inelar<=3500 && mic<=3500) {delay(1000);if (kx>11)Screen2[kx-12]=('E');else Screen1[kx]=('E');kx++;}if (kx>26)s=2;//lcd.write("E");SerialBT.println("E");delay(1000);i++;lcd.setCursor(i,0);}
    if (mare>=3550 && aratator>=3550 && mijlociu>=3550 && inelar<=3500 && mic<=3500) {delay(1000);if (kx>11)Screen2[kx-12]=('K');else Screen1[kx]=('K');kx++;}if (kx>26)s=2;//lcd.write("K");SerialBT.println("K");delay(1000);i++;lcd.setCursor(i,0);}
    if (mare>=3500 && aratator>=3550 && mijlociu<=3500 && inelar<=3500 && mic<=3500) {delay(1000);if (kx>11)Screen2[kx-12]=('L');else Screen1[kx]=('L');kx++;}if (kx>26)s=2;//lcd.write("L");SerialBT.println("L");delay(1000);i++;lcd.setCursor(i,0);}
    if (mare<=3500 && aratator>=3550 && mijlociu>=3550 && inelar<=3500 && mic<=3500) {delay(1000);if (kx>11)Screen2[kx-12]=('V');else Screen1[kx]=('V');kx++;}if (kx>26)s=2;//lcd.write("V");SerialBT.println("V");delay(1000);i++;lcd.setCursor(i,0);}
    if (mare<=3500 && aratator>=3550 && mijlociu<=3500 && inelar<=3500 && mic<=3500) {delay(1000);if (kx>11)Screen2[kx-12]=('Z');else Screen1[kx]=('Z');kx++;}if (kx>26)s=2;//lcd.write("Z");SerialBT.println("Z");delay(1000);i++;lcd.setCursor(i,0);}
    if (s==2){kx=0;s=1;}
    //display(Screen1);
    lcd.setCursor(3,0);
    lcd.write(Screen1);//afisare rand 1
    lcd.setCursor(0,1);
    lcd.write(Screen2);//afisare rand 2
    //lcd.write(Screen1);
    if ((current_up == HIGH) && (current_down == HIGH)){send();delay(3000);senddata="";}

    lcd.setCursor(15,0);
    
    lcd.write(byte(1));
    lcd.setCursor(15,1);
    lcd.write(byte(2));
    
    }
    /* lcd.setCursor(0,1);
    //if (Serial.available()) SerialBT.write(Serial.read());
    if (SerialBT.available()) Serial.write(SerialBT.read());
    lcd.setCursor(5,0);
    for (j=5;j<=14;j++) Serial.println(" ");
   

     */
     break;
    case 3:{     //Page 3
      lcd.setCursor(0,0);
      lcd.write("AP:");




      /*
      if (mare<=3450 && aratator>=3600 && mijlociu>=3600 && inelar<=3500 && mic<=3500) {lcd.write('H');SerialBT.println("H");delay(1000);string[lungime++]=h[0];}
      if (mare<=3500 && aratator<=3500 && mijlociu<=3500 && inelar<=3500 && mic<=3500) {lcd.write("E");SerialBT.println("E");delay(1000);string[lungime++]=e[0];}
      if (mare>=3500 && aratator>=3550 && mijlociu<=3500 && inelar<=3500 && mic<=3500) {lcd.write("L");SerialBT.println("L");delay(1000);string[lungime++]=l[0];}
      if ((mare>3470 && mare<=3550) && (aratator>3400 && aratator<=3500)) {lcd.write("O");SerialBT.println("O");delay(1000);string[lungime++]=o[0];}*/
      lcd.setCursor(0,1);
      if (lungime==5) lcd.print(string);
      lcd.setCursor(15,0);
      lcd.write(byte(1));
      lcd.setCursor(15,1);
      lcd.write(byte(2));
    }break;

   
  
  }
}