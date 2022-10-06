const int sensor1Pin = A0;
const int sensor2Pin = A1;
const int sensorMidPin = A2;

#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if (!AFMS.begin() {
    Serial.println("Check wiring");
    while (1)
  }

  leftMotor->setSpeed(150);  
  rightMotor->setSpeed(150);

  leftMotor->run(FORWARD);
  rightMotor->run(BACKWARD);

  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
