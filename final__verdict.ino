#include <Adafruit_GFX.h>  // Include core graphics library
#include <Adafruit_ST7735.h>  // Include Adafruit_ST7735 library to drive the display


// Declare pins for the display:
#define TFT_CS     10
#define TFT_RST    9  // You can also connect this to the Arduino reset in which case, set this #define pin to -1!
#define TFT_DC     8
#define ledPin     7
// The rest of the pins are pre-selected as the default hardware SPI for Arduino Uno (SCK = 13 and SDA = 11)
const int pwm = 3 ;
int lcdCount = 20;
const int btnPin = 5;



// Create display:
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
int held =0;
int restart=0;
int buttonState;

void setup()  // Start of setup
{Serial.begin(9600);
 pinMode(7,OUTPUT);
tft.initR(INITR_BLACKTAB);  // Initialize a ST7735S chip, black tab

  tft.fillScreen(ST7735_BLACK);  // Fill screen with black

 tft.setRotation(1); // Set orientation of the display. Values are from 0 to 3. If not declared, orientation would be 0,
 tft.setTextWrap(true); 
}                         
void loop()  // Start of loop
{ 
  digitalWrite(7,HIGH);
  

 while(Serial.available() > 0){
    
    //READ THE STRING TO THE FIRST "|" DIVIDER AND STORE AS A VARIABLE
    String myDate = Serial.readStringUntil('|');
    Serial.read();
    //STORE THE NEXT SECTION OF STRING AS A VARIABLE
    String myTime = Serial.readStringUntil('|');
    Serial.read();
    //STORE THE THIRD SECTION OF STRING AS A VARIABLE
    String myPhone = Serial.readStringUntil('|');
    Serial.read();
    //STORE THE FINAL SECTION AS A VARIABLE
    String myText  = Serial.readStringUntil('\n');
    
    //CHECK TO SEE WHAT IS STORED IN THE TIME VARIABLE
    //AND DISPLAY IT TO THE SCREEN
    if(myTime.indexOf('time') >= 0){
      tft.setCursor(0,0);
      tft.setTextColor(ST7735_CYAN, ST7735_BLACK); 
      tft.setTextSize(2); 
      tft.println("Comm Error");
   }else{
      tft.setCursor(0,0);
      tft.setTextColor(ST7735_CYAN, ST7735_BLACK); 
      tft.setTextSize(2);
      tft.println(myTime);
     
    }

    //CHECK TO SEE WHAT IS STORED IN THE DATE VARIABLE
    //AND DISPLAY IT TO THE SCREEN
    
 
       if(myDate.indexOf('date') >= 0){
    
     tft.setCursor(0,35);
      
      tft.println("                                      ");
   }else{
     
     tft.setCursor(0,35);
    tft.setTextColor(ST7735_YELLOW, ST7735_BLACK); 
    tft.setTextSize(2); 
      tft.println(myDate);
   }
    
    if(myPhone.indexOf('phone') >= 0){
      
       tft.setCursor(0,65);
       tft.print("                                           ");

      digitalWrite(pwm, LOW);
   }else{
    digitalWrite(ledPin, HIGH);
     
      tft.setCursor(0,65);
     tft.setTextColor(ST7735_GREEN, ST7735_BLACK);
    
      tft.setTextSize(2); 
      tft.println(myPhone);
      //Turn LCD On
      lcdCount = 10;
      //Motor Vibration Pattern
      digitalWrite(pwm, HIGH);
      delay(500);
      digitalWrite(pwm, LOW);
      delay(500);
      
   }

   //CHECK THE PHONE VARIABLE TO SEE IF A CALL IS
   //BEING RECIEVED. IF SO, TURN ON THE BACKLIGHT
   //AND VIBRATE THE MOTOR
   if(myText.indexOf('text') >= 0){
      tft.setCursor(0,65);
      tft.print("                                           ");
      tft.setCursor(0,75);
      digitalWrite(pwm, LOW);
      digitalWrite(ledPin, LOW);
   }else{
      digitalWrite(ledPin, HIGH);
      tft.setCursor(0,65);
      tft.setTextColor(ST7735_GREEN, ST7735_BLACK);
      tft.setTextSize(2); 
     
      tft.println("Text From:");
    
      tft.println(myText);
      //Turn LCD On
      lcdCount = 10;
      //Motor Vibration Pattern
      digitalWrite(pwm, HIGH);
      delay(250);
      digitalWrite(pwm, LOW);
      delay(250);
      digitalWrite(pwm, HIGH);
      delay(250);
      digitalWrite(pwm, LOW);
      delay(250);
   }
  }
  
    //DETECT WHEN THE BUTTON HAS BEEN PRESSED
    //AND TURN ON THE BACKLIGHT
    buttonState = digitalRead(btnPin);
    if (buttonState == LOW) {
       held++;
    } else {
       held=0;
    }
    if(held > 10 && held < 100 && restart == 0){
     restart++;
      held=0;
    }else if(held > 500 && restart > 0){
      restart=0;
     held=0;
     //softReset();
     lcdCount = 0;
   }
   
   //BACKLIGHT LOOP
   if(lcdCount < 15 ){
     digitalWrite(ledPin, HIGH);
     lcdCount += 1;
   }
   if(lcdCount >= 15){
     digitalWrite(ledPin, LOW);
     lcdCount = 20;
    }
    
}
