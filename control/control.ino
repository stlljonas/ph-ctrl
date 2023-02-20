#define MOTOR1_IN1 6
#define MOTOR2_IN1 5

void setup() {
  Serial.begin(9600);

  Serial.println("DRV8871 test");
  
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
}

void loop() {

  // ramp up forward
  digitalWrite(MOTOR1_IN1, LOW);
  for (int i=0; i<255; i++) {
    analogWrite(MOTOR1_IN1, i);
    delay(10);
  }

  // forward full speed for one second
  delay(5000);
  
  // ramp down forward
  for (int i=255; i>=0; i--) {
    analogWrite(MOTOR1_IN1, i);
    delay(10);
  }

  // ramp up forward
  digitalWrite(MOTOR2_IN1, LOW);
  for (int i=0; i<255; i++) {
    analogWrite(MOTOR2_IN1, i);
    delay(10);
  }

  // forward full speed for one second
  delay(5000);
  
  // ramp down forward
  for (int i=255; i>=0; i--) {
    analogWrite(MOTOR2_IN1, i);
    delay(10);
  }

}