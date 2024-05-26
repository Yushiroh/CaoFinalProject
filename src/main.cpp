#include <Arduino.h>
#include <SPI.h>
#include "RTClib.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 4; 
char hexaKeys[ROWS][COLS] = {
  {'D','#','0','*'},
  {'C','9','8','7'},
  {'B','6','5','4'},
  {'A','3','2','1'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; 
byte colPins[COLS] = {9, 8, 7, 6}; 


Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

String setH = "00";
String setM = "00";
String setS = "00";
String kVal = "";
char modeState = 'A';
String fVal;

void modeA(String currentTime, String alarmTime);
void modeB();
void modeC();
void modeD();
void valConfirm(String prepVal);
String zeroCheck(String val);

void setup () {
  Serial.begin(57600);
  
  lcd.begin();
	lcd.backlight();
  lcd.print("Hello!");
  lcd.setCursor(0,1);
  lcd.print("Welcome");
  delay(2000);
  lcd.clear();


  if(! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
}

void loop() {
  DateTime time = rtc.now();
  String currentTime = String(time.timestamp(DateTime::TIMESTAMP_TIME));
  char customKey = customKeypad.getKey();
  String alarm = setH + ":" + setM + ":" + setS;

  if(currentTime == alarm){
    lcd.clear();
    lcd.print("ALARMING");
    delay(3000);
    modeA(currentTime, alarm);
  }

  
  if(customKey){
    if(customKey == 'A'){
      kVal = "";
      modeState = 'A';
      lcd.clear();
      modeA(currentTime, alarm);
    }else if(customKey == 'B'){
      kVal = "";
      modeState = 'B';
      lcd.clear();
      modeB();
    }else if(customKey == 'C'){
      kVal = "";
      modeState = 'C';
      lcd.clear();
      modeC();
    }else if(customKey == 'D'){
      kVal = "";
      modeState = 'D';
      lcd.clear();
      modeD();
    }else if(customKey == '#'){
      kVal = "";
      if(modeState == 'A'){
        Serial.println("Mode Invalid!");
      }else{
        lcd.clear();
        lcd.print("Input Cancelled!");
        delay(1000);
        modeA(currentTime, alarm);
      }

    }else if(customKey == '*'){

      valConfirm(kVal);
      lcd.clear();
      modeA(currentTime, alarm);

    }else{
      lcd.print(customKey);
      kVal += customKey;
    }

  }


    if(modeState == 'A'){
      modeA(currentTime, alarm);
    }
    // else if(modeState == 'B'){
    //   Serial.println(kVal);
    // }


}

void modeA(String currentTime, String alarmTime){

  //home
  lcd.setCursor(0,0);
  lcd.print(" Time: ");
  lcd.print(currentTime);
  lcd.setCursor(0,1);
  lcd.print(" Alarm: ");
  lcd.print(alarmTime);
  
  modeState = 'A';


}

void modeB(){
  lcd.setCursor(0,0);
  lcd.print("Set Alarm time!");
  lcd.setCursor(0,1);
  lcd.print("Hour:");
}

void modeC(){
  lcd.setCursor(0,0);
  lcd.print("Set Alarm time!");
  lcd.setCursor(0,1);
  lcd.print("Mins:");
}

void modeD(){
  lcd.setCursor(0,0);
  lcd.print("Set Alarm time!");
  lcd.setCursor(0,1);
  lcd.print("Secs:");
}

void valConfirm(String prepVal){
  String finalVal;
  int pValInt = prepVal.toInt();

  if(pValInt < 24 && modeState == 'B'){
    finalVal = String(pValInt);
    lcd.clear();
    lcd.print("Value Set!");
    delay(1000);
    setH = zeroCheck(finalVal);
  }else if (pValInt < 60 && modeState == 'C'){
    finalVal = String(pValInt);
    lcd.clear();
    lcd.print("Value Set!");
    delay(1000);
    setM = zeroCheck(finalVal);
  }else if (pValInt < 60 && modeState == 'D'){
    finalVal = String(pValInt);
    lcd.clear();
    lcd.print("Value Set!");
    delay(1000);
    setS = zeroCheck(finalVal);
  }else if(modeState == 'A'){

    modeState = 'A';

  }else{
    lcd.clear();
    lcd.print("Invalid Value!");
    delay(1000);
  }

  kVal = "";

}

String zeroCheck(String val){
  if(val == "0"){
    fVal = "00";
  }else if(val == ""){
    fVal = "00";
  }else if(val == "1"){
    fVal = "01";
  }else if(val == "2"){
    fVal = "02";
  }else if(val == "3"){
    fVal = "03";
  }else if(val == "4"){
    fVal = "04";
  }else if(val == "5"){
    fVal = "05";
  }else if(val == "6"){
    fVal = "06";
  }else if(val == "7"){
    fVal = "07";
  }else if(val == "8"){
    fVal = "08";
  }else if(val == "9"){
    fVal = "09";
  }else{
    fVal = val;
    Serial.println("invalid!");
  }

  return fVal;

}