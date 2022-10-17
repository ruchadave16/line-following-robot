const int sensorLeftPin = A2;
const int sensorRightPin = A1;
const int sensorMidPin = A3;

const int boundaryValue = 550;

int sensorLeftValue;
int sensorRightValue;
int speed_set;

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
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorLeftValue = analogRead(sensorLeftPin);
  sensorRightValue = analogRead(sensorRightPin);

  if (Serial.available() > 0) {
    speed_set = (Serial.parseInt());
    if (speed_set == 0) {
        speed_set = 30;
    }
  }
    
  Serial.println(speed_set);

  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
  
  if (sensorLeftValue > boundaryValue) {
    Serial.println("Found Line");
    Serial.println(sensorLeftValue);
    leftMotor->setSpeed(0);
    rightMotor->setSpeed(speed_set + 50);
  }

  else if (sensorRightValue > boundaryValue) {
    Serial.println("Found Other Line");
    Serial.println(sensorRightValue);
    leftMotor->setSpeed(speed_set + 50);
    rightMotor->setSpeed(0);
  }
  
  else {
    Serial.println("Go Straight");
    leftMotor->setSpeed(speed_set);  
    rightMotor->setSpeed(speed_set);
  }
}
