#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

int timer1;
int timer2;

float Time;

int flag1 = 0;
int flag2 = 0;


float distance = 5.0;
double velocity = 0;

const int ir_s1 = A2;
const int ir_s2 = A3;
const int ledGreen = 2;
const int ledYellow = 3;
const int ledRed = 4;

void TurnOfLight() {
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledRed, LOW);
}
void TurnOnRed() {
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledRed, HIGH);
}
void TurnOnYellow() {
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledYellow, HIGH);
  digitalWrite(ledRed, LOW);
}
void TurnOnGreen() {
  digitalWrite(ledGreen, HIGH);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledRed, LOW);
}

void setup() {
  pinMode(ir_s1, INPUT);
  pinMode(ir_s2, INPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);

  Serial.begin(9600);
  TurnOfLight();

  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  if (digitalRead (ir_s1) == LOW && flag1 == 0) {
    timer1 = millis();
    flag1 = 1;
  }

  if (digitalRead (ir_s2) == LOW && flag2 == 0) {
    timer2 = millis();
    flag2 = 1;
  }

  if (flag1 == 1 && flag2 == 1) {
    if (timer1 > timer2) {
      Time = timer1 - timer2;
    }
    else if (timer2 > timer1) {
      Time = timer2 - timer1;
    }
    Time = Time / 1000;
    velocity = (distance / Time);
    velocity = velocity * 3600;
    velocity = velocity / 1000;
  }
  Serial.println(velocity);
  if (velocity == 0) {
    lcd.setCursor(0, 1);
    if (flag1 == 0 && flag2 == 0) {
      TurnOfLight();
      lcd.print("  Khong co xe.  ");
    }
    else {
      lcd.print("   Dang do...   ");
    }
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Speed:");
    lcd.print(velocity, 1);
    lcd.print("Km/Hr  ");
    lcd.setCursor(0, 1);
    if (velocity > 50) {
      lcd.print(" Qua toc do roi ");
      TurnOnRed();
    }
    else if (velocity > 40 && velocity < 50)
    {
      TurnOnYellow();
      lcd.print("Canh bao toc do ");
    }
    else {
      TurnOnGreen();
      lcd.print(" Lai xe an toan ");
    }
    delay(3000);
    TurnOfLight();
    velocity = 0;
    flag1 = 0;
    flag2 = 0;
  }
}
