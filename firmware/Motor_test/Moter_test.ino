// Arduino Uno + L293D + 2 TT motors test

// Motor A
const int ENA = 9;
const int IN1 = 8;
const int IN2 = 7;

// Motor B
const int IN3 = 5;
const int IN4 = 4;
const int ENB = 3;

int motorSpeed = 255;  // 0-255

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopCar();

  Serial.println("L293D TT Motor Test Start");
}

void loop() {
  Serial.println("Forward");
  moveForward();
  delay(2000);

  Serial.println("Stop");
  stopCar();
  delay(1000);

  Serial.println("Backward");
  moveBackward();
  delay(2000);

  Serial.println("Stop");
  stopCar();
  delay(1000);

  Serial.println("Turn Left");
  turnLeft();
  delay(1500);

  Serial.println("Stop");
  stopCar();
  delay(1000);

  Serial.println("Turn Right");
  turnRight();
  delay(1500);

  Serial.println("Stop");
  stopCar();
  delay(2000);
}

void moveForward() {
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopCar() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void turnLeft() {
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}