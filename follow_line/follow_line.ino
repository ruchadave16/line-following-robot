const int sensorLeftPin = A0;
const int sensorRightPin = A1;
const int sensorMidPin = A2;

const int boundaryValue = 600;

int sensorLeftValue;
int sensorRightValue;
int sencorMidValue;

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

  pinMode(sensorLeftPin, INPUT);
  pinMode(sensorRightPin, INPUT);
  pinMode(sensorMidPin, INPUT);

  leftMotor->setSpeed(150);  
  rightMotor->setSpeed(150);

  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);

  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorLeftValue = analogRead(sensorLeftPin);
  sensorRightValue = analogRead(sensorRightPin);
  sensorMidValue = analogRead(sensorMidValue);

  if ((sensorMidValue < boundaryValue) && (sensorLeftValue < boundaryValue) && (sensorRightValue < boundaryValue)) {
    leftMotor->run(FORWARD);
    rightMotor->run(BACKWARD);

    leftMotor->setSpeed(100);
    rightMotor->setSpeed(100);
  }

  else if (sensorLeftValue > boundaryValue) {
    rightMotor->setSpeed(200);
  }

  else if (sensorRightValue > boundaryValue) {
    leftMotor->setSpeed(200);
  }
  
  else {
    leftMotor->setSpeed(150);  
    rightMotor->setSpeed(150);
  }

  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);
}
