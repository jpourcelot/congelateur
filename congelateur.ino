
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "OneWire.h"
#include "DallasTemperature.h"
 

//const int chipSelect = 10;
const int eeAddress = 0;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
const int alarmPin = 3;
const int relayPin = 2;


OneWire oneWire(A0);

DallasTemperature ds(&oneWire);

int lcd_key = 0;
int adc_key_in = 0;
#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

signed char saved;

boolean changing;
const int refreshIni = 20;
int refresh;

int mesurement;
int min = 100;
int max = -100;

boolean relayState;

signed char alarm = -10;

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  delay(1000);
  lcd.begin(16, 2);   
  saved = readSavedTemp();
  if(saved > -10 || saved < -30){
    Serial.println("Saved out of bounds");
    saveTemp({-18});
  }
  changing = false;
  ds.begin();          // sonde activée
  mesurement = readTemp();
  line1();
  refresh = refreshIni;
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, 0);
  pinMode(alarmPin, OUTPUT);
  digitalWrite(alarmPin, 0);
  relayState = false;
}

void loop() {     
  if(changing || refresh < 0){
    if(refresh <= 0){
      refresh = refreshIni;
      mesurement = readTemp();
    }
    if(mesurement >= alarm){
      digitalWrite(alarmPin, 1);
    } else{
      digitalWrite(alarmPin, 0);
    }
    if(mesurement > max){
      max = mesurement;
    }
    if(mesurement < min){
      min = mesurement;
    }
    if(mesurement <= saved - 1){
      relayState = false;
    }
    if(mesurement >= saved){
      relayState = true;
    }
    digitalWrite(relayPin, relayState);
    line1();
    line2();
  }
  
  lcd_key = read_LCD_buttons();
  switch (lcd_key){
    case btnRIGHT:
      {
      break;
      }
    case btnUP:
      {
      changing = true;
      saved = saved +1;
      break;
      }
    case btnDOWN:
      {
      changing = true;
      saved = saved - 1;
      break;
      }
    case btnLEFT:
      {
      break;
      }
    case btnSELECT:
      {
      saveTemp(saved);
      saved = readSavedTemp(); //to ensure saved
      changing = false;
      line1();
      break;
      }
    case btnNONE:
      {
      break;
      }
  }
  delay(200);
  refresh--;
}

int read_LCD_buttons()
{
  adc_key_in = analogRead(0); // read the value from the sensor
  if (adc_key_in > 1000) return btnNONE;
  /*
  if (adc_key_in < 50) return btnRIGHT;
  if (adc_key_in < 195) return btnUP;
  if (adc_key_in < 380) return btnDOWN;
  if (adc_key_in < 555) return btnLEFT;
  if (adc_key_in < 790) return btnSELECT;*/
  if (adc_key_in < 50) return btnRIGHT;
  if (adc_key_in < 195) return btnUP;
  if (adc_key_in < 380) return btnDOWN;
  if (adc_key_in < 555) return btnLEFT;
  if (adc_key_in < 790) return btnSELECT;
  return btnNONE;
}

void saveTemp(signed char temp){
//  signed char c {-50};  // Or int8_t.
  EEPROM.put(eeAddress, temp);
}

byte readSavedTemp(){
  signed char d;
  EEPROM.get(eeAddress, d);
  Serial.println(d);
  return (d);
}

int readTemp(){
  ds.requestTemperatures();
  int t = ds.getTempCByIndex(0);
  return t;
}

void line1(){
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  String state = "(off)";
  if(relayState){
    state = "(on)";
  }
  if(changing){
    lcd.print("<" + String(saved) + ">");
  }else{
    lcd.print(saved);
  }
  lcd.print( " => " + String( mesurement ) + state);
}

void line2(){
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("min" + String(min) + "/max" + String(max));
}
