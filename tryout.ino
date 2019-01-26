#include <Keypad.h>
#include <Wire.h>  
#include <LiquidCrystal_I2C.h>



LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address, if it's not working try 0x3F.
long currentMillis;

const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
String password;

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

long interval = 1000;  
long previousMillis = 0;
long tijd = 0;
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  lcd.begin(16,2);
  lcd.backlight();
  //askPass();
  setTime();
}

void setTime(){
 lcd.setCursor(0,0); //First line
 lcd.print("Time: 00:00:00");
 lcd.blink();
  while(tijd == 0){
    setHours();
    setMinutes();
    setSeconds();
    
  } 
}

void setHours(){
  lcd.setCursor(6,0);
  lcd.blink();
  tijd = timeInput() * 3600;

}

long timeInput(){
  int i = 0;
  long t = 0;
  while(i < 2){
      char customKey = customKeypad.getKey();
      if (customKey and isDigit(customKey)){
        if(i == 0 and (customKey - 48 < 6)){
         lcd.print(customKey);
         t = (customKey-'0')*10;
         i++;
        }else if(i != 0){
          lcd.print(customKey);
          i++;
          t += (customKey-'0');
        }
      }
  }
  return t;
}


void setMinutes(){
  lcd.setCursor(9,0);
  lcd.blink();
  tijd += timeInput() * 60 * 1000;
}
void setSeconds(){
  lcd.setCursor(12,0);
  tijd += timeInput() * 1000;
  lcd.noBlink();
} 

void askPass(){
    lcd.setCursor(0,0);
    lcd.print("Enter a pass: ");
    lcd.setCursor(9,1);
    while(password.length() < 4){
        lcd.blink();
      char customKey = customKeypad.getKey();
      if (customKey){
        lcd.print("*");
        password += customKey;
      }
    }
    lcd.noBlink();
    delay(500);
    lcd.clear();
}


void loop()
{
  currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;  
    updateTime();
   }else{
      char customKey = customKeypad.getKey();
      lcd.setCursor(10,1);
      if (customKey == 'C'){
        setWinnaar(true);
      }
   }
}

void setWinnaar(bool gewonnen){
  lcd.clear();
  lcd.setCursor(0,0);
    
  if(!gewonnen){
    lcd.print("P@tje heeft");
    lcd.setCursor(3,1);
    lcd.print("Gewonnen");
  }else{
    lcd.print("Proficiat!!");
  }
}

void updateTime(){
 
}
