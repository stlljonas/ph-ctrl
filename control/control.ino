#define MOTOR1_IN1 6
#define MOTOR2_IN1 5

float calibration = 17.09 + 4.00 + 0.09; //change this value to calibrate
const int analogInPin = A0;
int sensorValue = 0;
unsigned long int avgValue;
float b;
int buf[10],temp;
unsigned long int loop_delay_ms = 5 * 60000;
unsigned long int control_input_length_ms = 3000;
float margin_of_error = 0.3;
float ph_max = 6.0 + margin_of_error; // optimal: 6.5
float ph_min = 6.0 - margin_of_error; // optimal: 5.5
unsigned long int last_timestamp = 0;
 
void setup() {
  Serial.begin(9600);
  pinMode(MOTOR1_IN1, OUTPUT);
  pinMode(MOTOR2_IN1, OUTPUT);
  Serial.println("Startup: Waiting for pH value to settle");
}

void loop() {
  delay(10000);
  if (millis() >= last_timestamp + loop_delay_ms){
    Serial.println("checking sensor");
    last_timestamp = millis();
    for(int i=0;i<10;i++) {
      buf[i]=analogRead(analogInPin);
      delay(30);
    }
    for(int i=0;i<9;i++) {
      for(int j=i+1;j<10;j++) {
        if(buf[i]>buf[j]) {
          temp=buf[i];
          buf[i]=buf[j];
          buf[j]=temp;
        }
      }
    }
    avgValue=0;
    for(int i=2;i<8;i++)
    avgValue+=buf[i];
    float pHVol=(float)avgValue*5.0/1024/6;
    float phValue = -5.70 * pHVol + calibration;
    Serial.print("sensor:");
    Serial.println(phValue);

    if (phValue > ph_max) {
      Serial.println("control input: phDown");
      // ramp up forward
      digitalWrite(MOTOR1_IN1, LOW);
      for (int i=0; i<255; i++) {
        analogWrite(MOTOR1_IN1, i);
        delay(10);
      }

      // forward full speed for 3 seconds
      delay(control_input_length_ms);
      
      // ramp down forward
      for (int i=255; i>=0; i--) {
        analogWrite(MOTOR1_IN1, i);
        delay(10);
      }
    } else if (phValue < ph_min) {
      Serial.println("control input: phUp");

      // ramp up forward
      digitalWrite(MOTOR2_IN1, LOW);
      for (int i=0; i<255; i++) {
        analogWrite(MOTOR2_IN1, i);
        delay(10);
      }

      delay(control_input_length_ms);
      
      // ramp down forward
      for (int i=255; i>=0; i--) {
        analogWrite(MOTOR2_IN1, i);
        delay(10);
      }
    } else {
      Serial.println("control input: None");
    }
    Serial.println("Sleeping for about five minute.");
  }
  // delay(loop_delay_ms);s
}