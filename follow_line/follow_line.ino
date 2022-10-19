#include <Adafruit_MotorShield.h>

// Declares the analog pin numbers to measure the IR sensors
const int sensorLeftPin = A2;
const int sensorRightPin = A1;

// Defines the threshold that defines whether the IR sensor is above
// the black tape or not
const int boundaryValue = 510;

// Used to store the value received over serial
int currSerialInput;

// Stores the current reading of the IR sensors
int sensorLeftValue;
int sensorRightValue;

// Stores the tunable default speed of the wheels
int speedSet;

// Contains the instantaneous wheel speeds
int leftWheelSpeed;
int rightWheelSpeed;

// Defines the direction of the wheel, 'f' is forward and 'b' is backwards
String leftDir;
String rightDir;

// Declares and instantiates DCMotor objects on pins 2 and 3 of the motorshield
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftMotor = AFMS.getMotor(3);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);

void setup() {
  // Starts the serial output
  Serial.begin(9600);

  // Stalls other processes until the motorshield has been properly connected
  if (!AFMS.begin()) {
    Serial.println("Check wiring");
    while (1);
  }

  // Declares the 2 IR sensors as inputs
  pinMode(sensorLeftPin, INPUT);
  pinMode(sensorRightPin, INPUT);

  // Sets the initial base speed to 0
  speedSet = 0;
}

void loop() {
  // Reads the analog value of both IR sensors
  sensorLeftValue = analogRead(sensorLeftPin);
  sensorRightValue = analogRead(sensorRightPin);

  // If there is a new non zero value received over serial, use that value to update
  // the base wheel speed
  if (Serial.available() > 0) {
    currSerialInput = (Serial.parseInt());
    if (currSerialInput != 0) {
      speedSet = currSerialInput;
    }
  }

  // If both sensors are above the threshold value (detecting the black tape), then
  // set both wheel speeds to 0
  if (sensorLeftValue > boundaryValue && sensorRightValue > boundaryValue) {
    leftWheelSpeed = 0;
    leftDir = 'f';
    rightWheelSpeed = 0;
    rightDir = 'f';
  }

  // If only the left sensor is above the threshold value, then turn the robot towards
  // the left (right wheel moves forward and left wheel moves backward)
  else if (sensorLeftValue > boundaryValue) {
    leftMotor->run(BACKWARD);
    leftDir = 'b';
    rightMotor->run(FORWARD);
    rightDir = 'f';
    leftWheelSpeed = speedSet;
    rightWheelSpeed = speedSet;
  }
  
  // If only the right sensor is above the threshold value, then turn the robot towards
  // the right (right wheel moves backward and left wheel moves forward)
  else if (sensorRightValue > boundaryValue) {
    leftMotor->run(FORWARD);
    leftDir = 'f';
    rightMotor->run(BACKWARD);
    rightDir = 'b';
    leftWheelSpeed = speedSet;
    rightWheelSpeed = speedSet;
  }

  // If no sensor is above the threshold value, then drive straight
  else {
    leftMotor->run(FORWARD);
    leftDir = 'f';
    rightMotor->run(FORWARD);
    rightDir = 'f';
    leftWheelSpeed = speedSet;
    rightWheelSpeed = speedSet;
  }

  // Send the speeds determined above to the respective wheels
  leftMotor->setSpeed(leftWheelSpeed);
  rightMotor->setSpeed(rightWheelSpeed);

  // Send the sensor readings and the wheel speeds (with direction) over the serial connection
  // to be visualized by a python backend system
  Serial.println("leftWheel " + leftDir + " " + String(leftWheelSpeed));
  Serial.println("rightWheel " + rightDir + " " + String(rightWheelSpeed));
  Serial.println("leftIR " + String(sensorLeftValue));
  Serial.println("rightIR " + String(sensorRightValue));
}
