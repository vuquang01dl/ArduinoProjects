#include <DHT.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

const int DHTPIN = 4;
const int DHTTYPE = DHT11;
const int ledWarning = 2;
const int relayFan = 3;
const int relayWarm = 5;
DHT dht(DHTPIN, DHTTYPE);

void turnOnLedWarning(){
  digitalWrite(ledWarning, HIGH);
  delay(500);
  digitalWrite(ledWarning, LOW);
  delay(500);
}

void setup(){
  pinMode(relayFan, OUTPUT);
  pinMode(relayWarm, OUTPUT);
  pinMode(ledWarning, OUTPUT);

  digitalWrite(ledWarning, LOW);
  digitalWrite(relayFan, LOW);
  digitalWrite(relayWarm, LOW);

  dht.begin();
}

void loop(){
  float t = dht.readTemperature();
  if(isnan(t)){ 
  } else {
    if( t<21 || t>36){
      turnOnLedWarning();
    }
    if(33<=t && t <=36){
      digitalWrite(relayFan, HIGH);
    } else {
      digitalWrite(relayFan, LOW);
    }
    if (21<=t && t<=33){
      digitalWrite(relayWarm, HIGH);
    } else {
      digitalWrite(relayWarm, LOW);
    }
  }
}
