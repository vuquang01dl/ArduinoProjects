#include <DHT.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

const int DHTPIN = 6;
const int DHTTYPE = DHT11;
const int lightSensor = 4;
const int waterSensor = A1;
DHT dht (DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x3F,16,2);

byte degree[8] = {
  0B01110,
  0B01010,
  0B01110,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000
};

void setup(){
  pinMode(lightSensor, INPUT);
  pinMode(waterSensor, INPUT);
  dht.begin();
  lcd.createChar(1, degree);
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop(){
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  int waterValue = map(analogRead(waterSensor), 0, 1024, 0, 100);
  if(isnan(t) || isnan(h)){
    //not to do
  } else {
    lcd.setCursor(0,0);
    lcd.print("Nhiet Do: ");
    lcd.print(round(t));
    lcd.write(1);
    lcd.print("C");
    lcd.setCursor(0,1);
    lcd.print("Do am: ");
    lcd.setCursor(10, 1);
    lcd.print(round(h));
    lcd.print("%");
  }
  delay(2000);
  lcd.clear();
  lcd.print("Muc nuoc: ");
  lcd.print(waterValue);
  lcd.print("%");
  delay(2000);
  lcd.clear();
  if(digitalRead(lightSensor)==1){
    lcd.print("Troi da toi");
  } else {
    lcd.print("Troi dang sang");
  }
  delay(2000);
  lcd.clear();
}
