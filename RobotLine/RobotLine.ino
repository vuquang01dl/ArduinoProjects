//Cài đặt chân cắm Motor 
/* Speed cắm chân anolog
 * Chiều chuyển động cắm chân digital
 */
//Motor Phải
int speedRight = 3; //Cắm chân A3 trên arduino
int IN1 = 1; //Cắm chân D1 trên arduino
int IN2 = 2; //Cắm chân D2 trên arduino
//Motor Trái
int speedLeft = 4; //Cắm chân A4 trên arduino
int IN3 = 3; //Cắm chân D3 trên arduino
int IN4 = 4; //Cắm chân D4 trên arduino

//Cài đặt chân cắm SensorLine (Toàn bộ trên port Digital)
int leftSen = 5;
int midSen =6;
int rightSen = 7;

//Định nghĩa hướng di chuyển
#define STOP      'S'
#define FORWARD   'F'
#define TURNLEFT  'L'
#define TURNRIGHT 'R'

void motorRun(int cmd){
  switch(cmd){
    case FORWARD:
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      //Tuỳ chỉnh tốc độ tiến
      analogWrite(speedLeft, 100);
      analogWrite(speedRight, 100);
      break;
    case STOP:
      analogWrite(speedLeft, 0);
      analogWrite(speedRight, 0);
      break;
    case TURNLEFT:
      analogWrite(speedLeft, 0);
      analogWrite(speedRight, 100);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      delay(1000);
      analogWrite(speedLeft, 100);
      break;
    case TURNRIGHT:
      analogWrite(speedLeft, 100);
      analogWrite(speedRight, 0);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      delay(1000);
      analogWrite(speedRight, 100);
      break;
    default: break;
  }
  return;
}



void setup() {
  pinMode(speedRight, OUTPUT);
  pinMode(speedLeft, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(leftSen, INPUT);
  pinMode(midSen, INPUT);
  pinMode(rightSen, INPUT);
}

void loop() {
  int readLeft = digitalRead(leftSen);
  int readMid = digitalRead(midSen);
  int readRight = digitalRead(rightSen);

  if(readLeft==0 && readMid==1 && readRight==0){
    motorRun(FORWARD);
  }
  if(readLeft==1 && readMid==1 && readRight==1){
    motorRun(STOP);
  }
  if(readLeft==1 && readMid==1 && readRight==0){
    motorRun(TURNLEFT);
  }
  if(readLeft==0 && readMid==1 && readRight==1){
    motorRun(TURNRIGHT);
  }
}
