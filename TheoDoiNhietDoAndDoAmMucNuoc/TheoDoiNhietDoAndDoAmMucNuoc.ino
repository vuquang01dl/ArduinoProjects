#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int DHTPIN = 4;
const int DHTTYPE = DHT11;
const int lightSensor = 3;
const int waterSensor = A0;
DHT dht (DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x3F, 16, 2);
const int relay = 6;

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

void setup() {
  pinMode(lightSensor, INPUT);
  pinMode(waterSensor, INPUT);
  pinMode(relay, OUTPUT);
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  digitalWrite(relay, LOW);
}

void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  int waterValue = map(analogRead(waterSensor), 0, 1023, 0, 100);
  if (isnan(t) || isnan(h)) {
    //not to do
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Nhiet Do: ");
    lcd.print(round(t));
    lcd.createChar(1, degree);
    lcd.setCursor(12, 0);
    lcd.write(1);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Do am: ");
    lcd.setCursor(10, 1);
    lcd.print(round(h));
    lcd.print("%");
  }
  delay(2000);
  lcd.clear();
  lcd.print("Muc nuoc: ");
  lcd.print(waterValue);
  if (waterValue < 10) {
    digitalWrite(relay, HIGH);
  }
  if (waterValue > 80) {
    digitalWrite(relay, LOW);
  }
  lcd.print("%");
  delay(2000);
  lcd.clear();
  if (digitalRead(lightSensor) == 1) {
    lcd.print("Troi da toi");
  } else {
    lcd.print("Troi dang sang");
  }
  delay(2000);
  lcd.clear();
}
