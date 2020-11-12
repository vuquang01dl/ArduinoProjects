#include <Servo.h>   //Servo library. This is standard library. (Sketch -> Include Library -> Servo)
String voice;

#define STOP      'S'
#define FORWARD   'F'
#define TURNLEFT  'L'
#define TURNRIGHT 'R'
#define BACKWARD 'B'

Servo myServo; //define servo name
int ServoPin = 10;

int LED1 = A0; //define LED 1 pin
int LED2 = A1; //define LED 2 pin
int buzzerPin = A2; //define buzzer pin
//Motor Right
int speedRight = 8;
int IN1 = 2;
int IN2 = 3;
//Motor Left
int speedLeft = 7;
int IN3 = 4;
int IN4 = 5;
//Distance Sensor]
int trigPin = A3;
int echoPin = A4;
//
long duration, dist, average;
long aver[3];
//
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

void motorRun(int cmd) {
  switch (cmd) {
    case FORWARD:
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      //Tuỳ chỉnh tốc độ tiến
      analogWrite(speedLeft, 200);
      analogWrite(speedRight, 200);
      break;
    case BACKWARD:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      //Tuỳ chỉnh tốc độ tiến
      analogWrite(speedLeft, 200);
      analogWrite(speedRight, 200);
      break;
    case STOP:
      analogWrite(speedLeft, 0);
      analogWrite(speedRight, 0);
      break;
    case TURNLEFT:
      analogWrite(speedLeft, 0);
      analogWrite(speedRight, 200);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      delay(1000);
      analogWrite(speedLeft, 200);
      break;
    case TURNRIGHT:
      analogWrite(speedLeft, 200);
      analogWrite(speedRight, 0);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      delay(1000);
      analogWrite(speedRight, 200);
      break;
    default: break;
  }
  return;
}

void LED_on ()
{
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
}

void LED_off ()
{
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
}

void buzzer_on ()
{
  tone(buzzerPin, 100);
  delay(800);
  noTone(buzzerPin);
}

void setup()
{
  Serial.begin(9600); //start serial communication
  myServo.attach(ServoPin); //define our servo pin (the motor shield servo1 input = digital pin 10)
  myServo.write(90);  //servo position is 90 degrees
  pinMode(LED1, OUTPUT); //A0 is output pin
  pinMode(LED2, OUTPUT); //A1 is output pin
  pinMode(buzzerPin, OUTPUT); //A2 is output pin
  pinMode(speedRight, OUTPUT);
  pinMode(speedLeft, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop()
{
  while (Serial.available()) { //Check if there is an available byte to read
    delay(10); //Delay added to make thing stable
    char c = Serial.read(); //Conduct a serial read
    if (c == '#') {
      break; //Exit the loop when the # is detected after the word
    }
    voice += c; //Shorthand for voice = voice + c
  }
  if (voice.length() > 0) {
    if (voice == "*Tiến") {
      myServo.write(90);
      while (distance() > 10) {
        motorRun(FORWARD);
      }
      motorRun(STOP);
    }
    else if (voice == "*lùi") {
      motorRun(BACKWARD);
      delay(15000);
      motorRun(STOP);
    }
    else if (voice == "*rẽ phải") {
      myServo.write(0);
      if (distance() > 10) {
        motorRun(TURNRIGHT);
        delay(1000);
        motorRun(STOP);
      }
      myServo.write(90);
    }
    else if (voice == "*rẽ trái") {
      myServo.write(180);
      if (distance() > 10) {
        motorRun(TURNLEFT);
        delay(1000);
        motorRun(STOP);
      }
      myServo.write(90);
    }
    else if (voice == "*bật đèn") {
      LED_on();
    }
    else if (voice == "*Tắt Đèn") {
      LED_off();
    }
    else if (voice == "*buzzer") {
      buzzer_on();
    }
    else if (voice == "*stop") {
      motorRun(STOP);
    }

    voice = ""; //Reset the variable after initiating
  }
}
