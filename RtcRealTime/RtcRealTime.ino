#include <DS1302.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

//to detect correct LCD i2c address, read http://osoyoo.com/2014/12/07/16x2-i2c-liquidcrystal-displaylcd/
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 16, 2);
DS1302 rtc(5, 4, 3);
// DS1302:  RST pin    -> Arduino Digital 5
//          DAT pin   -> Arduino Digital 4
//          CLK pin  -> Arduino Digital 3

const int DHTPIN = 7;
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);

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

void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();

  dht.begin();

  rtc.halt(false);
  rtc.writeProtect(false);
  rtc.setDOW(WEDNESDAY);
  rtc.setTime(19, 30, 00);
  rtc.setDate(11, 10, 2020);
  rtc.writeProtect(true);
}

void loop()
{
  lcd.createChar(1, degree);
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  lcd.setCursor(4, 0);
  lcd.print(rtc.getTimeStr());
  lcd.setCursor(3, 1);
  lcd.print(rtc.getDateStr(FORMAT_LONG, FORMAT_LITTLEENDIAN, '/'));
  delay(1000);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Nhiet do: ");
  lcd.setCursor(1, 1);
  lcd.print("Do am: ");
  if(isnan(t) || isnan(h)){
    
  } else {
    lcd.setCursor(11,0);
    lcd.print(round(t));
    lcd.write(1);
    lcd.print("C");
    lcd.setCursor(8,1);
    lcd.print(round(h));
    lcd.print("%");
  }
  delay(1000);
  lcd.clear();
}
