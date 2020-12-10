#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Servo.h>
Servo servo;

#define WIFI_SSID "Shine"// sửa tên wifi chỗ bạn chú ý viết hoa và viết thường
#define WIFI_PASS "123456789"// nhập mật khẩu wifi chỗ bạn

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "vuquangclone06"
#define MQTT_PASS "aio_LoEo75B2Vh2kBNhvWmJ8B16QHC6g"

#define servoPin D5
#define trigPin D7
#define echoPin D6

long duration, dist, average;
long aver[3];

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

//Set up MQTT and WiFi clients
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

//Set up the feed you're subscribing to
Adafruit_MQTT_Subscribe forward = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/forward");
Adafruit_MQTT_Subscribe backward = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/backward");
Adafruit_MQTT_Subscribe turnleft = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/turnleft");
Adafruit_MQTT_Subscribe turnright = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/turnright");

//
void goForward() {
  digitalWrite(D1, HIGH);
  digitalWrite(D2, LOW);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, LOW);
}

void Stop() {
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
}

void goBackward() {
  digitalWrite(D1, LOW);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, LOW);
  digitalWrite(D4, HIGH);
}

void turnLeft() {
  digitalWrite(D1, LOW);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, LOW);
}

void turnRight() {
  digitalWrite(D1, HIGH);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, HIGH);
}

void setup()
{
  Serial.begin(9600);

  //Connect to WiFi
  Serial.print("\n\nConnecting Wifi... ");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  Serial.println("OK!");

  //Subscribe to the device feed
  mqtt.subscribe(&forward);
  mqtt.subscribe(&backward);
  mqtt.subscribe(&turnleft);
  mqtt.subscribe(&turnright);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  Stop();

  servo.attach(servoPin);
  servo.write(90);
  delay(500);
  servo.detach();

}

void loop()
{
  MQTT_connect();

  //Read from our subscription queue until we run out, or
  //wait up to 5 seconds for subscription to update
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    //If we're in here, a subscription updated...
    if (subscription == &forward)
    {
      //Print the new value to the serial monitor
      Serial.print("forward: ");
      Serial.println((char*) forward.lastread);

      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) forward.lastread, "ON"))
      {
        //Active low logic
        goForward();
        delay(5000);
        // dừng xe
        Stop();
      }
      else
      {
        Stop();
      }
    }

    if (subscription == &backward)
    {
      //Print the new value to the serial monitor
      Serial.print("backward: ");
      Serial.println((char*) backward.lastread);

      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) backward.lastread, "ON"))
      {
        goBackward();
        delay(5000);
        // dừng xe
        Stop();
      }
      else
      {
        Stop();
      }
    }

    if (subscription == &turnleft)
    {
      //Print the new value to the serial monitor
      Serial.print("turnleft: ");
      Serial.println((char*) turnleft.lastread);

      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) turnleft.lastread, "ON"))
      {
        // d2, d1 low high là tiến
        // d4, d3 low high là lùi
        servo.attach(servoPin);
        servo.write(180);
        delay(1000);
        long khoangCach = distance();
        delay(1000);
        servo.write(90);
        delay(1000);
        servo.detach();
        if (khoangCach > 15 ) {
          turnLeft();
          delay(1000);
          // dừng xe
          Stop();
        } else {
          Stop();
        }
      }
      else
      {
        Stop();
      }
    }

    if (subscription == &turnright)
    {
      //Print the new value to the serial monitor
      Serial.print("turnright: ");
      Serial.println((char*) turnright.lastread);

      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) turnright.lastread, "ON"))
      {
        servo.attach(servoPin);
        servo.write(0);
        delay(1000);
        long khoangCach = distance();
        delay(1000);
        servo.write(90);
        delay(1000);
        servo.detach();
        if (khoangCach > 15 ) {
          turnLeft();
          delay(1000);
          // dừng xe
          Stop();
        } else {
          Stop();
        }
      }
      else
      {
        Stop();
      }
    }
  }

  // ping the server to keep the mqtt connection alive
  if (!mqtt.ping())
  {
    mqtt.disconnect();
  }
}

void MQTT_connect()
{
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected())
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0)
    {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
