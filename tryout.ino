#include <Keypad.h>
#include <Wire.h>  
#include <LiquidCrystal_I2C.h>



LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address, if it's not working try 0x3F.
long currentMillis;
bool sound = false;
const byte ROWS = 4; 
const byte COLS = 4; 
bool explosion = false;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
String password;
String guess;
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

long interval = 1000;  
long previousMillis = 0;
long tijd = 0;
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  lcd.begin(16,2);
  lcd.backlight();
  askPass();
  setTime();
  start();
}

void startCountDown(){
  
}

void pauzeCountdown(){
  
}

void start(){
  lcd.setCursor(2,1);
  lcd.print("Start? press A");
  char customKey = customKeypad.getKey();
    while(customKey != 'A'){
      customKey = customKeypad.getKey();
       if (customKey){
          lcd.setCursor(2,1);
          lcd.print("Start:          ");
       }
    }
    lcd.setCursor(9,1);
    delay(1000);
    for(int  i=3; i > 0;i--){
      lcd.print(i);
      lcd.print(" ");
      delay(1000);
    }
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(4,1);
    lcd.print("Code: ");

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
  tijd += timeInput() * 60;
}
void setSeconds(){
  lcd.setCursor(12,0);
  tijd += timeInput();
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
  bool gewonnen = false;
  while(tijd >= 0 and !gewonnen){
  currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;  
    updateTime();
   }else if(password.equals(guess)){
      gewonnen = true;
   
   }else{
      char customKey = customKeypad.getKey();
      if(customKey){
          lcd.setCursor(10,1);
          if (password.equals(guess)){
              lcd.clear();
              gewonnen = true;
          }else if(guess.length() < 4){
            lcd.setCursor(11+guess.length(),1);
            lcd.print("*");
            guess += customKey;
          }else{
            guess = "";
            lcd.setCursor(4,1);
            lcd.print("Code: ");
          }
      }
   }
 }
 setWinnaar(gewonnen);
 while(sound){
     tone(12,261);
 }
 exit(0);
}

void setWinnaar(bool gewonnen){
  lcd.clear();
  lcd.setCursor(0,0);
    
  if(!gewonnen){
    lcd.print("P@tje heeft");
    lcd.setCursor(3,1);
    lcd.print("Gewonnen");
  }else{
    lcd.print("Bom ontmanteld!!");
  }
}

void updateTime(){ 
  long hour = tijd / 3600;
      lcd.setCursor(6,0);
  if(hour > 9){
    lcd.print(hour);
  }else{
    lcd.print(0);
    lcd.print(hour);
  }
  long minute = tijd / 60;
      lcd.setCursor(9,0);

    if(minute > 9){
    lcd.print(minute);
  }else{
    lcd.print(0);
    lcd.print(minute);
  }
  long sec = tijd % 60;
  lcd.setCursor(12,0);
    if(sec > 9){
    lcd.print(sec);
  }else{
    lcd.print(0);
    lcd.print(sec);
  }
  tijd -= 1;
}
