const int sensorLeftPin = A2;
const int sensorRightPin = A1;

const int boundaryValue = 510;

int currSerialInput;
int sensorLeftValue;
int sensorRightValue;
int speedSet;
int leftWheelSpeed;
int rightWheelSpeed;
long startTime;
String leftDir;
String rightDir;

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

  leftMotor->setSpeed(30);
  rightMotor->setSpeed(30);

  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);

  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);

  startTime = millis();
  speedSet = 30;
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
    leftDir = 'f';
    rightWheelSpeed = 0;
    rightDir = 'f';
  }

  else if (sensorLeftValue > boundaryValue) {
    leftMotor->run(BACKWARD);
    leftDir = 'b';
    rightMotor->run(FORWARD);
    rightDir = 'f';
    leftWheelSpeed = speedSet;
    rightWheelSpeed = speedSet;
  }

  else if (sensorRightValue > boundaryValue) {
    leftMotor->run(FORWARD);
    leftDir = 'f';
    rightMotor->run(BACKWARD);
    rightDir = 'b';
    leftWheelSpeed = speedSet + 0;
    rightWheelSpeed = speedSet;
    //    leftMotor->setSpeed(speedSet + 100);
    //    rightMotor->setSpeed(0);
  }

  else {
    leftMotor->run(FORWARD);
    leftDir = 'f';
    rightMotor->run(FORWARD);
    rightDir = 'f';
    leftWheelSpeed = speedSet;
    rightWheelSpeed = speedSet;
  }

  leftMotor->setSpeed(leftWheelSpeed);
  rightMotor->setSpeed(rightWheelSpeed);

  Serial.println("leftWheel " + leftDir + " " + String(leftWheelSpeed) + " " + String(millis() - startTime));
  Serial.println("rightWheel " + rightDir + " " + String(rightWheelSpeed) + " " + String(millis() - startTime));
  Serial.println("leftIR " + String(sensorLeftValue) + " " + String(millis() - startTime));
  Serial.println("rightIR " + String(sensorRightValue) + " " + String(millis() - startTime));
}
