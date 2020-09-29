#include <AFMotor.h>
AF_DCMotor motor (1, MOTOR12_1KHZ); //create motor #1 using M1 output on Motor Drive Shield, set to 1kHz PWM frequency

const int rainSensor = 3;
const int lightSensor = 4;
const int buttonIn = 5;
const int buttonOut = 6;
const int trigIn = A0;
const int echoIn = A1;
const int trigOut = A3;
const int echoOut = A4;

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

bool checkButtonIn(int buttonIn){
  return digitalRead(buttonIn);
}

bool checkButtonOut(int buttonOut){
  return digitalRead(buttonOut);
}

void keoRa(){
  while(distanceOut(trigOut, echoOut)>20){
    motor.run(FORWARD);
    motor.setSpeed(255);
  }
  motor.run(RELEASE);
}

void keoVao(){
  while(distanceIn(trigIn, echoIn)>20){
    motor.run(BACKWARD);
    motor.setSpeed(255);
  }
  motor.run(RELEASE);
}

void DungLai(){
  motor.run(RELEASE);
}

void setup(){
  pinMode(lightSensor, INPUT_PULLUP);
  pinMode(rainSensor, INPUT);
  pinMode(buttonIn, INPUT);
  pinMode(buttonOut, INPUT);
  pinMode(trigIn, OUTPUT);
  pinMode(echoIn, INPUT);
  pinMode(trigOut, OUTPUT);
  pinMode(echoOut, INPUT);

  keoVao();
}
void loop(){
  if(checkLightDark(lightSensor) == 0 && checkRain(rainSensor) == 1){
    keoRa();
  } else {
    keoVao();
  }
  if(checkButtonIn(buttonIn)==1){
    keoVao();
  }
  if(checkButtonOut(buttonOut)==1){
    keoRa();
  }
}
