#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 16, 2);
#include <SPI.h>
#include <MFRC522.h> // thu vien "RFID".
#include <SD.h>
#include <TMRpcm.h>
#include <Servo.h>
TMRpcm tmrpcm;
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
#define SD_ChipSelectPin 4

int trigPin1 = A0;
int echoPin1 = A1;
int trigPin2 = A2;
int echoPin2 = A3;
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

long duration1, dist1, average1;
long aver1[3];
long duration2, dist2, average2;
long aver2[3];

void moCua() {
  for (int i = 0; i <= 100; i++) {
    servo.write(i);
    delay(20);
  }
  statusDoor = true;
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
    case 581646388: S = "  Tran Hai Nam  "; kiemTraSoLuong(S);  break;
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
void measure1() {
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  dist1 = (duration1 / 2) / 29.1;
}
void measure2() {
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  dist2 = (duration2 / 2) / 29.1;
}
long distance1() {
  for (int i = 0; i <= 2; i++) {
    measure1();
    aver1[i] = dist1;
    delay(50);
  }
  average1 = (aver1[0] + aver1[1] + aver1[2]) / 3;
  return average1;
}
long distance2() {
  for (int i = 0; i <= 2; i++) {
    measure2();
    aver2[i] = dist2;
    delay(50);
  }
  average2 = (aver2[0] + aver2[1] + aver2[2]) / 3;
  return average2;
}

void setup() {
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(button, INPUT);

  servo.attach(servoPin);
  SPI.begin();
  mfrc522.PCD_Init();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  demHS = 0;
  dongCua();
}

void loop() {
  if (statusDoor == true) {
    if (docButton == true) {
      moCua();
    }
    if (distance2() < 15) {
      moCua();
      delay(4000);
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
      delay(1000);
      moCua();
      delay(4000);
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
