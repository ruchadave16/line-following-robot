const int sensorLeftPin = A2;
const int sensorRightPin = A1;
const int sensorMidPin = A3;

const int boundaryValue = 510;

int currSerialInput;
int sensorLeftValue;
int sensorRightValue;
int speedSet;
int leftWheelSpeed;
int rightWheelSpeed;
long startTime;

#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftMotor = AFMS.getMotor(3);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if (!AFMS.begin()) {
    Serial.println("Check wiring");
    while (1);
  }

  pinMode(sensorLeftPin, INPUT);
  pinMode(sensorRightPin, INPUT);
//  pinMode(sensorMidPin, INPUT);

  leftMotor->setSpeed(30);  
  rightMotor->setSpeed(30);

  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);

  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);

  startTime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorLeftValue = analogRead(sensorLeftPin);
  sensorRightValue = analogRead(sensorRightPin);

  if (Serial.available() > 0) {
    currSerialInput = (Serial.parseInt());
    if (currSerialInput != 0) {
        speedSet = currSerialInput;
    }
  }
  if (sensorLeftValue > boundaryValue && sensorRightValue > boundaryValue) {
    leftWheelSpeed = 0;
    rightWheelSpeed = 0;
  }
  
  else if (sensorLeftValue > boundaryValue) {
    leftMotor->run(BACKWARD);
    rightMotor->run(FORWARD);
    leftWheelSpeed = speedSet;
    rightWheelSpeed = speedSet + 0;
//    leftMotor->setSpeed(0);
//    rightMotor->setSpeed(speedSet + 100);
  }

  else if (sensorRightValue > boundaryValue) {
    leftMotor->run(FORWARD);
    rightMotor->run(BACKWARD);
    leftWheelSpeed = speedSet + 0;
    rightWheelSpeed = speedSet;
//    leftMotor->setSpeed(speedSet + 100);
//    rightMotor->setSpeed(0);
  }
  
  else {
    leftMotor->run(FORWARD);
    rightMotor->run(FORWARD);
    leftWheelSpeed = speedSet;
    rightWheelSpeed = speedSet;
  }

  leftMotor->setSpeed(leftWheelSpeed);
  rightMotor->setSpeed(rightWheelSpeed);

  Serial.println("leftWheel " + String(leftWheelSpeed) + " " + String(millis() - startTime));
  Serial.println("rightWheel " + String(rightWheelSpeed) + " " + String(millis() - startTime));
  Serial.println("leftIR " + String(sensorLeftValue) + " " + String(millis() - startTime));
  Serial.println("rightIR " + String(sensorRightValue) + " " + String(millis() - startTime));
}
