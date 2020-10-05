#include <AFMotor.h>
#include <SD.h>
#include <TMRpcm.h>
#define SD_ChipSelectPin 7
AF_DCMotor motor(4, MOTOR12_64KHZ); //create motor #1 using M1 output on Motor Drive Shield, set to 64kHz PWM frequency

TMRpcm tmrpcm;
const int rainSensor = A0 ;
const int lightSensor = A1;
const int trigIn = A2;
const int echoIn = A3;
const int trigOut = A4;
const int echoOut = A5;

int distanceIn(int trig, int echo){
  unsigned long duration; // biến đo thời gian
  int distance;           // biến lưu khoảng cách

  /* Phát xung từ chân trig */
  digitalWrite(trig, LOW); // tắt chân trig
  delayMicroseconds(2);
  digitalWrite(trig, HIGH); // phát xung từ chân trig
  delayMicroseconds(5);   // xung có độ dài 5 micro giây
  digitalWrite(trig, LOW); // tắt chân trig

  /* Tính toàn thời gian */
  // Đo độ rộng xung HIGH ở chân echo
  duration = pulseIn(echo, HIGH);
  // Tính khoảng cách đến vật (cm)
  distance = int(duration/2/29.412);
  return distance;
} 

int distanceOut(int trig, int echo){
  unsigned long duration; // biến đo thời gian
  int distance;           // biến lưu khoảng cách

  /* Phát xung từ chân trig */
  digitalWrite(trig, LOW); // tắt chân trig
  delayMicroseconds(2);
  digitalWrite(trig, HIGH); // phát xung từ chân trig
  delayMicroseconds(5);   // xung có độ dài 5 micro giây
  digitalWrite(trig, LOW); // tắt chân trig

  /* Tính toàn thời gian */
  // Đo độ rộng xung HIGH ở chân echo
  duration = pulseIn(echo, HIGH);
  // Tính khoảng cách đến vật (cm)
  distance = int(duration/2/29.412);
  return distance;
}

bool checkLightDark(int lightSensor){
  // Trả về TRUE khi trời tối, FALSE khi trời sáng
  return digitalRead(lightSensor);
}

bool checkRain(int rainSensor){
  // Trả về TRUE khi trời không mưa, FALSE khi trời mưa
  return digitalRead(rainSensor);
}

void keoRa(){
  motor.setSpeed(255);
  while(distanceOut(trigOut, echoOut)>20){
    motor.run(FORWARD);
  }
  motor.run(RELEASE);
  delay(3000);
}

void keoVao(){
  motor.setSpeed(255);
  while(distanceIn(trigIn, echoIn)>20){
    motor.run(BACKWARD);
  }
  motor.run(RELEASE);
  delay(3000);
}

void setup(){
  pinMode(lightSensor, INPUT);
  pinMode(rainSensor, INPUT);
  pinMode(trigIn, OUTPUT);
  pinMode(echoIn, INPUT);
  pinMode(trigOut, OUTPUT);
  pinMode(echoOut, INPUT);

  tmrpcm.speakerPin = 9;
  if(!SD.begin(SD_ChipSelectPin)){
    return;
  }
  tmrpcm.setVolume(6);
  motor.setSpeed(255);
  keoVao();
}
void loop(){
  if(checkLightDark(lightSensor) == 0 && checkRain(rainSensor) == 1){
    tmrpcm.play("keora.wav");
    keoRa();
  } else {
    tmrpcm.play("keovao.wav");
    keoVao();
  }
}
