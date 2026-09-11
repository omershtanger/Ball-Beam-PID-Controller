#include <Servo.h>

const int TRIGGER_PIN = 10;
const int ECHO_PIN = 11;
const int SERVO_PIN = 9;

const float SOUND_SPEED = 0.0343;
const float CENTER_POINT = 24.0;

// PID constants 
float KP = 1.2;         
float KI = 0.0008;       
float KD = 15.0;         

// PID variables
float last_error = 0;
float integral = 0;

const int AVG_SAMPLES = 10;
float distance_buffer[AVG_SAMPLES];
int buffer_index = 0;
float avg_distance = CENTER_POINT;

Servo myservo;

// Horizontal angle
const int HORIZONTAL_ANGLE = 73;

// Helper variable to prevent sharp jumps
int prev_target_angle = HORIZONTAL_ANGLE;

float measure_distance() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return (duration * SOUND_SPEED) / 2;
}

float get_avg_distance(float new_distance) {
  for (int i = 0; i < AVG_SAMPLES - 1; i++) {
    distance_buffer[i] = distance_buffer[i + 1];
  }
  distance_buffer[AVG_SAMPLES - 1] = new_distance;

  float sum = 0;
  for (int i = 0; i < AVG_SAMPLES; i++) {
    sum += distance_buffer[i];
  }
  return sum / AVG_SAMPLES;
}

// PID control function
void control_servo(float distance) {
  float error = CENTER_POINT - distance;
  integral = constrain(integral + error, -50, 50);

  float derivative = error - last_error;

  float output = KP * error + KI * integral + KD * derivative;

  last_error = error;

  int target_angle = HORIZONTAL_ANGLE - output;
  target_angle = constrain(target_angle, HORIZONTAL_ANGLE - 5, HORIZONTAL_ANGLE + 5);  // Tighter movement range restriction

  // Smooth servo movement
  int angle_diff = target_angle - prev_target_angle;
  if (abs(angle_diff) > 2) {
    target_angle = prev_target_angle + (angle_diff > 0 ? 2 : -2);
  }
  prev_target_angle = target_angle;

  myservo.write(target_angle);
}

void setup() {
  Serial.begin(9600);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  myservo.attach(SERVO_PIN);

  myservo.write(HORIZONTAL_ANGLE);
  delay(2000);

  for (int i = 0; i < AVG_SAMPLES; i++) {
    distance_buffer[i] = CENTER_POINT;
  }
}

void loop() {
  float distance = measure_distance();
  distance = get_avg_distance(distance);

  Serial.print("Distance: ");
  Serial.println(distance);

  control_servo(distance);

  delay(30);
}