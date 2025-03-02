
#include <LiquidCrystal.h>
#include <EEPROM.h>


const int chipSelect = 10;
const int eeAddress = 0;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
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

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  delay(1000);
  lcd.begin(16, 2);   
  saved = readTemp();
  if(saved > -10 || saved < -30){
    Serial.println("Saved out of bounds");
    saveTemp({-18});
  }
  changing = false;
}

void loop() {     
  lcd.setCursor(0,0);
  lcd.print("              ");
  lcd.setCursor(0,0);
  if(changing){
    lcd.print("<< " + String(saved) + " >>");
  }else{
    lcd.print(saved);
  }
  lcd_key = read_LCD_buttons();
  lcd.setCursor(0,1);
  switch (lcd_key){
    case btnRIGHT:
      {
      lcd.print("RIGHT ");
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
      lcd.print("left ");
      break;
      }
    case btnSELECT:
      {
      saveTemp(saved);
      saved = readTemp(); //to ensure saved
      changing = false;
      break;
      }
    case btnNONE:
      {
      break;
      }
  }
  delay(500);
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

byte readTemp(){
  signed char d;
  EEPROM.get(eeAddress, d);
  Serial.println(d);
  return (d);
}
