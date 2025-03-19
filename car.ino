#include <PID_v1.h>

uint8_t speed_robot = 150;
int8_t check_out = 0;

#define line_1 A0
#define line_2 A1
#define line_3 A2
#define line_4 A3
#define line_5 A4

double Setpoint = 0, Input, Output;
double Kp = 20, Ki = 0.04555555, Kd = 11.898989;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
int sensor;

void setup() {
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(line_1, INPUT);
  pinMode(line_2, INPUT);
  pinMode(line_3, INPUT);
  pinMode(line_4, INPUT);
  pinMode(line_5, INPUT);
  
  Input = 0;
  myPID.SetSampleTime(1);
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-speed_robot, speed_robot);
}

void motorControl(int16_t duty_value) {
  int16_t speed_a, speed_b;
  int speed_zero = speed_robot / 2;
  
  if (duty_value > 1) {
    speed_b = -speed_zero;
    speed_a = duty_value;
  } else if (duty_value == 0) {
    speed_a = speed_b = 0;
  } else if (duty_value < -1) {
    speed_a = -speed_zero;
    speed_b = -duty_value;
  }
  
  analogWrite(6, speed_b + speed_zero);
  analogWrite(5, speed_a + speed_zero);
}

void scan_sensor() {
  if (digitalRead(line_5)) sensor = 4;
  else if (digitalRead(line_4) && digitalRead(line_5)) sensor = 3;
  else if (digitalRead(line_4)) sensor = 2;
  else if (digitalRead(line_3) && digitalRead(line_4)) sensor = 1;
  else if (digitalRead(line_3)) sensor = 0;
  else if (digitalRead(line_2) && digitalRead(line_3)) sensor = -1;
  else if (digitalRead(line_2)) sensor = -2;
  else if (digitalRead(line_1) && digitalRead(line_2)) sensor = -3;
  else if (digitalRead(line_1)) sensor = -4;
}

void loop() {
  scan_sensor();
  Input = sensor;
  myPID.Compute();
  motorControl(Output);
}