//Firstly we have to include necessary libraries.

#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <Arduino.h>
#include <TM1637Display.h>

//Below are some variables that the libraries want us to define.

#define CLK 4
#define DIO 5
#define DHTPIN 2
#define DHTTYPE DHT11

/*Signals of my IR remote controller are defined down below. 
 You have to do that for your controller again. Because probably 
 signals of your controller and my controller are different.*/

#define b1 3001999165
#define b2 211564353
#define b3 46690913
#define b4 373604801
#define b5 2797147069
#define b6 3495072293
#define b7 32049505
#define b8 2455591773
#define b9 3409801085
#define b0 1113853081
#define yildiz 2914729472
#define kare 1811778305
#define up 1811778305
#define down 1474512609
#define right 2246090081
#define left 3419589885
#define ok 629155073



int red = 8 ;       //These are the red,green and blue pins of  
int green = 12 ;    //RGB LED which inform us about temperature.
int blue = 13 ;

int red1 = 9 ;      //These pins are red,green,blue and gnd pins
int green1 = 10 ;   //of our main RGB LEDs.
int blue1 = 11 ;
int ledGND = 7;

int ldrLed = 6 ;    //This is single colour lightmode inform LED.
int ldrMode = 0 ;   //This is not mention a pin.

float h = 0;
int r=0, g=0, b=0;

int displayBrightnes = 0x0b;
int displayBrightnesSetting = 0x00;


    DHT dht(DHTPIN , DHTTYPE);        //These codes from libraries.
    IRrecv irrecv(3);
    decode_results results;
    DS3231 clock;
    RTCDateTime dt;
    TM1637Display display(CLK, DIO);
    LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  lcd.begin(16,2);
  lcd.clear();
  dht.begin();
  Serial.begin(9600); 
  clock.begin();
  //clock.setDateTime(__DATE__, __TIME__);
  display.setBrightness(displayBrightnes);   //This code is setting up brightness of 7 segment screen.
  irrecv.enableIRIn();

  pinMode(red ,OUTPUT);            //Here pins are assigned output.
  pinMode(green ,OUTPUT);
  pinMode(blue ,OUTPUT);

  pinMode(red1 ,OUTPUT);
  pinMode(green1 ,OUTPUT);
  pinMode(blue1 ,OUTPUT);

  pinMode(ldrLed , OUTPUT);
  pinMode(ledGND , OUTPUT);

  digitalWrite(ledGND , LOW);

}

void loop() {

float t = dht.readTemperature();
int   h = dht.readHumidity();
float temp = dht.computeHeatIndex(t, h, false);

int light = analogRead(A3);

/*Below part is about temperature LED. If temperature is less than 20 degree LED lights up 
  blue. If temperature is between 20 and 25 degree LED lights up green. If temperature more
  than 25, LED lights up red.
 */
if (25 < temp)
{
  digitalWrite(red , HIGH);
  digitalWrite(blue , LOW);
  digitalWrite(green , LOW); 
}

else if (temp < 20)
{
  digitalWrite(red , LOW);
  digitalWrite(blue , HIGH);
  digitalWrite(green , LOW);
}
else
{
  digitalWrite(red , LOW);
  digitalWrite(blue , LOW);
  digitalWrite(green , HIGH);
}

/*Below part is about light mode. If the light mode is on and the environment is dark, 
  the brightness of the 7 segment display is dimmed and the main lights are turned on.If the light 
  mode is on and the environment is bright, the brightness of the 7 segment display will turn on and 
  the main lights will turn themselves off.
 */
 
if(light > 130 && ldrMode == 1)
 {
   digitalWrite(ledGND , HIGH);
   displayBrightnes = 0x0b;
   display.setBrightness(displayBrightnes);
 }
else if(light < 100 && ldrMode == 1)
 {
   digitalWrite(ledGND , LOW);
   displayBrightnes = displayBrightnesSetting;
   display.setBrightness(displayBrightnes);
 }



if (irrecv.decode(&results))   //This part is about IR remote control.
  {
        if(results.value == kare && ldrMode == 1) // light mode closed
        {
          analogWrite(ldrLed , 0);
          ldrMode = 0 ;
          analogWrite(ledGND , 0);
          displayBrightnes = 0x0b;
        }
        else if(results.value == kare && ldrMode == 0) // light mode opened
        {
          analogWrite(ldrLed , 1);
          ldrMode = 1 ;
        }
        else if(results.value == b1)     //When you press button 1, main RGB LEDs light up red.
        {
          digitalWrite(red1 , HIGH);
          digitalWrite(blue1 , LOW);
          digitalWrite(green1 , LOW);
        }
       else if(results.value == b2)      //When you press button 2, main RGB LEDs light up blue.
        {
           digitalWrite(red1 , LOW);    
           digitalWrite(blue1 , HIGH);
           digitalWrite(green1 , LOW);
        }
        else if(results.value == b3)     //When you press button 3, main RGB LEDs light up green.
        {
           digitalWrite(red1 , LOW);
           digitalWrite(blue1 , LOW);
           digitalWrite(green1 , HIGH);
        }
        else if(results.value == b4)     //When you press button 4, main RGB LEDs light up purple.
        {
           analogWrite(red1 , 150);      //The reason why I use analogwrite here is to obtain the color I want.
           analogWrite(blue1 , 255);
           analogWrite(green1 , 0);
        }
        else if(results.value == b5)     //When you press button 5, main RGB LEDs light up yellow.
        {
           analogWrite(red1 , 200);
           analogWrite(blue1 , 0);
           analogWrite(green1 , 255);
        }
        else if(results.value == b6)     //When you press button 6, main RGB LEDs light up cyan.
        {
           digitalWrite(red1 , LOW);
           digitalWrite(blue1 , HIGH);
           digitalWrite(green1 , HIGH);
        }
        else if(results.value == b7)     //When you press button 7, main RGB LEDs light up white.  
        {
           analogWrite(red1 , 190);
           digitalWrite(blue1 , HIGH);
           analogWrite(green1 , 220);
        }
        else if(results.value == ok)     //When you press okey button, main RGB LEDs go out.
        {
           digitalWrite(red1 , LOW);
           digitalWrite(blue1 , LOW);
           digitalWrite(green1 , LOW);
        }
        else if(results.value == left)   //When you press left arrow button, light mode brightness of 7 segment display decreases.
        {
          displayBrightnesSetting = 0x00;
          display.setBrightness(displayBrightnes);
        }
        else if(results.value == right) //When you press right arrow button, light mode brightness of 7 segment display increases.
        {
          displayBrightnesSetting = 0x02;
          display.setBrightness(displayBrightnes);
        }
      irrecv.resume();     //This is necessary to continue IR signal receiving.
  }

  lcd.clear();             //This part of code is about 16*2 lcd screen.
                                              
  lcd.setCursor(0,1);      //These codes are preparing screen to settle variables.
  lcd.print("T:");         //   Date:**/**/****
  lcd.print(temp);         //   T:**.**'C  H:%**
  lcd.print((char)223);
  lcd.print("C");

  lcd.setCursor(11,1);
  lcd.print("H:");
  lcd.print("%");
  lcd.print(h);

  dt = clock.getDateTime();
  
  lcd.setCursor(0,0);
  lcd.print("Date: ");
  lcd.print(dt.day);        lcd.print("/");
  lcd.print(dt.month);      lcd.print("/");
  lcd.print(dt.year);
  
  display.showNumberDecEx(dt.hour,0b11100000, true, 2, 0);    //These codes print clock data to 7 segment display.
  display.showNumberDecEx(dt.minute,0b11100000, true, 2, 2);
 
delay(1000);
}
