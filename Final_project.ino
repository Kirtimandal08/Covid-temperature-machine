/*************************                  ---ENGINEERING CAPESTONE PROJECT 2021---         ***************************************************
 *                   
 *                
 * Project By:    Robocon's
 * Group:         6
 * Group Members: Mandal, Kirti 
 *                Anaparthy, Sai Teja 
 *                Kurmaighari, Sai Teja
 * Project Name:  Temperature Based Entrance System
 * Submitted To:  Prof. Jarabek, Mike
 * Instructor:    Prof. Jarabek, Mike
  References:     Adafruit, Jul 29, 2012 http://www.adafruit.com/products/684
                  Shedboy71, 6th May 2016 http://www.arduinoprojects.net/lcd-projects/mlx90614-and-oled-display-example.php#codesyntax_1
 *
 *                
 **************************                 ---ENGINEERING CAPESTONE PROJECT 2021---         **************************************************/

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "pitches.h"
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

#include <Stepper.h>

const int stepsPerRevolution = 300;  // change this to fit the number of steps per revolution
// for your motor
int stepcount=0;

// initialize the stepper library on pins :
Stepper myStepper(stepsPerRevolution, 2, 4, 7, 12);

// pin definition of oled connection
#define sclk 13
#define mosi 11
#define cs   10
#define rst  9
#define dc   8
int ledpinred = 5;         //red
int ledpingreen = 6;         //green

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

Adafruit_SSD1331 display = Adafruit_SSD1331(&SPI, cs, dc, rst);

float p = 3.1415926;
float noteDurations[] = {1,1,1};
int melody[] = {NOTE_CS1};
int melodyy[] = {NOTE_E5};
int variation= 0;

void setup(void) 
{
      pinMode (ledpinred, OUTPUT);
      pinMode (ledpingreen, OUTPUT);
      //pinMode(buzzer,OUTPUT);//initialize the buzzer pin as an output
      pinMode(3, OUTPUT);
      // set the speed at 60 rpm:
  myStepper.setSpeed(60);
      
      Serial.begin(9600);
      Serial.println("hello!");
      display.begin();
      Serial.println("MLX_90614 testing");  
      mlx.begin();
}

void loop() 
{
    /*MLX90614 LOGIC*/
      Serial.print("Ambient = "); 
      Serial.print(mlx.readAmbientTempC()); 
      Serial.print("*C\tObject = "); 
      Serial.print(mlx.readObjectTempC()); 
      Serial.println(" Celsius");
      Serial.print("Ambient = ");
      Serial.print(mlx.readAmbientTempF()); 
      Serial.print("*F\tObject = "); 
      Serial.print(mlx.readObjectTempF()); 
      Serial.println(" Farenhite");
       Serial.println(" variation is: ");
       /*to check variation between ambient temp and body temp for detection of any body */
      variation = mlx.readObjectTempF()- mlx.readAmbientTempF();
      Serial.println(variation);
      Serial.println();
      delay(1000);
      /*MLX90614 ends*/
  
      /*OLED CODE LOGIC STARTS*/
       display.fillScreen(BLACK);
      display.setCursor(0, 5);
      display.setTextColor(RED);
      display.setTextSize(1);
      display.println("TEMPERATURE TEST");
      display.setTextColor(WHITE, CYAN);
      display.setTextSize(2);
      display.print("Amb=");
      display.print(mlx.readAmbientTempF());
      display.print("\n");
      display.setTextColor(WHITE, YELLOW);
      display.setTextSize(2);
      display.print("Bod=");
      display.println(mlx.readObjectTempF());
      
       display.setTextSize(1);
        display.setTextColor(YELLOW);
      display.print("ALL TEMPERATURES IN FARHENITE");
      delay(5000);
      display.setCursor(0, 5);
      display.fillScreen(BLACK); 

      /* Start for Entry Validate */
      if(mlx.readObjectTempF()<= 90.0 && variation > 4)
      { 
        digitalWrite(ledpingreen, HIGH );    // turn the LED on by making the voltage high
        display.setTextColor(CYAN);
        display.setTextSize(0);
        display.println("    Action   ");
        display.setTextSize(2);
        display.setTextColor(GREEN);
        display.println(" Entry  Validate  *_* ");
        
        // iterate over the notes of the melody:
      for (int thisNote = 0; thisNote < 3; thisNote++)
      {   
         // to calculate the note duration, take one second divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(3, melodyy[thisNote], noteDuration);
    
        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
      
        delay(1000);
        
         // stop the tone playing:
         noTone(3);
     }
      digitalWrite(ledpingreen, LOW);    // turn the LED off by making the voltage low
     // delay(1000);

       // step one revolution  in one direction:
        Serial.println("clockwise");
        myStepper.step(stepsPerRevolution); 
        delay(1000);
      
        // step one revolution in the other direction:
        Serial.println("counterclockwise");
        myStepper.step(-stepsPerRevolution); 
       delay(1000);     
    }
       /* End for Entry Validate */
      else if(mlx.readObjectTempF()> 90.0 && variation > 4)
      /* Start for COVID-19 alert */
       { 
        display.setCursor(0, 5);
        display.fillScreen(BLACK);
        digitalWrite(ledpinred, HIGH);    // turn the LED ON by making the voltage HIGH
        display.setTextSize(0);
        display.setTextColor(CYAN);
        display.println("    Action   ");
        display.setTextSize(2);
        display.setTextColor(RED);
        display.print("COVID-19 Alert     X ");
         delay(2000);
          // iterate over the notes of the melody:   
        for (int thisNote = 0; thisNote < 3; thisNote++)
          {
               // to calculate the note duration, take one second divided by the note type.
              //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
              int noteDuration = 1000 / noteDurations[thisNote];
              tone(3, melody[thisNote], noteDuration);
          
              // to distinguish the notes, set a minimum time between them.
              // the note's duration + 30% seems to work well:
              int pauseBetweenNotes = noteDuration * 1.30;
              delay(pauseBetweenNotes);
              // stop the tone playing:/
              noTone(3); 
        
          }
        digitalWrite(ledpinred, LOW);    // turn the LED ON by making the voltage HIGH
          delay(1000); 
  
      }
/* End for COVID-19 alert */
/*Detection of the person starts*/
else
{
      display.fillScreen(BLACK);
      display.setCursor(0, 5);
      display.setTextColor(RED);
      display.setTextSize(1);
      display.println("TEMPERATURE TESt");
      display.setTextColor(WHITE, MAGENTA );
      display.setTextSize(2);
      display.print("waiting for");
      display.print("\n");
      display.setTextColor(WHITE, MAGENTA );
      display.setTextSize(2);
      display.print("Person");
      delay(2000);
     
}
/*Detection of the person ends*/
}
/*OLED CODE LOGIC ENDS*/
