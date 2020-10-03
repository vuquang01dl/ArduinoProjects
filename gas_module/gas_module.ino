#include <LiquidCrystal_I2C.h>
#include <SPI.h> //Library for SPI interface 
#include <Wire.h> //Library for I2C interface 

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

int led = 10; //LED pin
int buzzer = 9; //Buzzer pin
int gas_sensor = A0; //Sensor pin
float m = -0.318; //Slope
float b = 1.133; //Y-Intercept
float R0 = 11.820; //Sensor Resistance in fresh air from previous code

void setup() {
  Serial.begin(9600); //Baud rate
  lcd.init();
  lcd.backlight();
 
  pinMode(led, OUTPUT); //Set LED as output
  digitalWrite(led, LOW); //Turn LED off
  pinMode(buzzer, OUTPUT); //Set buzzer as output
  digitalWrite(buzzer, LOW); // Turn buzzer off
  pinMode(gas_sensor, INPUT); //Set gas sensor as input
}

void loop() {
  lcd.setCursor(0, 0); //Place cursor in (x,y) location
  float sensor_volt; //Define variable for sensor voltage
  float RS_gas; //Define variable for sensor resistance
  float ratio; //Define variable for ratio
  float sensorValue = analogRead(gas_sensor); //Read analog values of sensor
  sensor_volt = sensorValue * (5.0 / 1023.0); //Convert analog values to voltage
  RS_gas = ((5.0 * 10.0) / sensor_volt) - 10.0; //Get value of RS in a gas
  ratio = RS_gas / R0;   // Get ratio RS_gas/RS_air

  double ppm_log = (log10(ratio) - b) / m; //Get ppm value in linear scale according to the the ratio value
  double ppm = pow(10, ppm_log); //Convert ppm value to log scale
  double percentage = ppm / 10000; //Convert to percentage
  lcd.print(percentage); //Load screen buffer with percentage value
  lcd.print("%"); //Load screen buffer with "%"
  
  if (ppm > 2000) {
    //Check if ppm value is greater than 2000
    digitalWrite(led, HIGH); //Turn LED on
    digitalWrite(buzzer, HIGH); //Turn buzzer on
  } else {
    //Case ppm is not greater than 2000
    digitalWrite(led, LOW);
    //Turn LED off
    digitalWrite(buzzer, LOW);
    //Turn buzzer off
  }
}
