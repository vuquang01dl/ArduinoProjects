//The Wrench
#include <SD.h>
#include <TMRpcm.h>

#define SD_ChipSelectPin 4

TMRpcm tmrpcm;

#include <Servo.h>
Servo servo;
int lightSensor = 8;
int ledRelay = 7;

int trigPin1 = A0;
int echoPin1 = A1;

int trigPin2 = A2;
int echoPin2 = A4;

int servoPin = 2;

long duration1, dist1, average1;
long aver1[3];

long duration2, dist2, average2;
long aver2[3];

void setup() {
  servo.attach(servoPin);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(lightSensor, INPUT);
  pinMode(ledRelay, OUTPUT);

  servo.write(0);
  delay(1000);
  servo.detach();

  tmrpcm.speakerPin = 9;

  if (!SD.begin(SD_ChipSelectPin)) {
    return;
  }

  tmrpcm.setVolume(6);
}

void openDoor() {
  servo.attach(servoPin);
  delay(1);
  tmrpcm.setVolume(6);
  tmrpcm.play("1.wav");
  servo.write(90);
  delay(10000);
  servo.detach();
}
void closeDoor(){
  servo.attach(servoPin);
  delay(1);
  servo.write(0);
  tmrpcm.setVolume(6);
  tmrpcm.play("2.wav");
  delay(1000);
  servo.detach();
  delay(3000);
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
void loop() {
  if(distance1<50 || distance2 <50){
    openDoor();
    closeDoor();
  }
}
