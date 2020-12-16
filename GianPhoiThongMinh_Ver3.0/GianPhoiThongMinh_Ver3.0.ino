const int rainSensor = A0 ;
const int lightSensor = A1;
const int relayRaVao = A2;
const int swtChucNang = A3;
const int btnKeoRa = 5;
const int btnKeoVao = 6;

bool checkLightDark(int lightSensor) {
  // Trả về TRUE khi trời tối, FALSE khi trời sáng
  return digitalRead(lightSensor);
}

bool checkRain(int rainSensor) {
  // Trả về TRUE khi trời không mưa, FALSE khi trời mưa
  return digitalRead(rainSensor);
}

void keoRa() {
  digitalWrite(relayRaVao, HIGH);
}

void keoVao() {
  digitalWrite(relayRaVao, LOW);
}

void setup() {
  pinMode(lightSensor, INPUT);
  pinMode(rainSensor, INPUT);
  pinMode(relayRaVao, OUTPUT);
  pinMode(swtChucNang, INPUT);
  pinMode(btnKeoRa, INPUT);
  pinMode(btnKeoVao, INPUT);
  keoVao();
}
void loop() {
  if (digitalRead(swtChucNang) == 1) {
    if (checkLightDark(lightSensor) == 0 && checkRain(rainSensor) == 1) {
      keoRa();
    } else {
      keoVao();
    }
  } else {
    if(digitalRead(btnKeoRa)==1){
      keoRa();
    }
    if(digitalRead(btnKeoVao)==1){
      keoVao();
    }
  }
}
