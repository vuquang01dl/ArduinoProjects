#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 16, 2);
#include <SPI.h>
#include <MFRC522.h> // thu vien "RFID".
#include <Servo.h>
Servo servo;
/*
  Ket noi voi Arduino Uno hoac Mega
  ---------------------------------
   Pin layout should be as follows:
   Signal     Pin              Pin               Pin
              Arduino Uno      Arduino Mega      MFRC522 board
   ------------------------------------------------------------
   Reset      9                5                 RST
   SPI SS     10               53                SDA
   SPI MOSI   11               51                MOSI
   SPI MISO   12               50                MISO
   SPI SCK    13               52                SCK

*/

#define SS_PIN 10
#define RST_PIN 9

int trigPin = A2;
int echoPin = A3;
int servoPin = 2;
int button = 5;

MFRC522 mfrc522(SS_PIN, RST_PIN);

const int siSoLop = 2;
String students[siSoLop] = {"", ""};
int demHS = 0;
bool statusDoor = true;
bool hopLe = false;
String S = "";
unsigned long ID = 0;

long duration, dist, average;
long aver[3];

void moCua() {
  for (int i = 0; i <= 100; i++) {
    servo.write(i);
    delay(20);
  }
  statusDoor = false;
}
void dongCua() {
  for (int i = 100; i >= 0; i--) {
    servo.write(i);
    delay(20);
  }
  statusDoor = true;
}
bool docButton() {
  int buttonStatus = digitalRead(button);
  delay(500);
  return buttonStatus;
}
unsigned long uidDec, uidDecTemp;
unsigned long readID() {
  // Tim the moi
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return 0;
  }

  // Doc the
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }


  uidDec = 0;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidDecTemp = mfrc522.uid.uidByte[i];
    uidDec = uidDec * 256 + uidDecTemp;
  }
  delay(500);
  return uidDec;
}
void notFound() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("So luong HS: ");
  lcd.setCursor(13, 0);
  lcd.print(demHS);
  lcd.setCursor(0, 1);
  lcd.print("Hay quet the... ");
}

void checkIDstudents() {
  ID = readID();
  switch (ID) {
    case 3408792335: S = " Pham Ngoc Hai  "; kiemTraSoLuong(S);  break;
    case 1981842473: S = " Tran Minh Hieu  "; kiemTraSoLuong(S); break;
    default: S = ""; hopLe = false; break;
  }
}
void kiemTraSoLuong(String S) {
  bool OK = false;
  hopLe = true;
  for (int i = 0; i < siSoLop; i++) {
    if (S == students[i]) {
      OK = true;
      break;
    }
  }
  if (OK == false) {
    students[demHS] = S;
    demHS++;
    S = "";
  }
}
void measure() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  dist = (duration / 2) / 29.1;
}
long distance() {
  for (int i = 0; i <= 2; i++) {
    measure();
    aver[i] = dist;
    delay(50);
  }
  average = (aver[0] + aver[1] + aver[2]) / 3;
  return average;
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(button, INPUT);

  SPI.begin();
  mfrc522.PCD_Init();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  demHS = 0;
  servo.attach(servoPin);
  servo.write(0);
  statusDoor = true;
}


void loop() {
  if (statusDoor == true) {
    if (docButton == true) {
      moCua();
    }
    if (distance() < 15) {
      moCua();
      delay(3000);
      dongCua();
    }
    checkIDstudents();
    if (hopLe == true) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(S);
      lcd.setCursor(0, 1);
      lcd.print("So luong HS: ");
      lcd.setCursor(13, 1);
      lcd.print(demHS);
      moCua();
      delay(3000);
      dongCua();
      hopLe = false;
    } else {
      notFound();
    }
  } else {
    if (docButton == true) {
      dongCua();
    }
  }
}
