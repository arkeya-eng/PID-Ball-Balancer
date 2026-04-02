#include <Servo.h>

const int irPin = A0;
const int servoPin = 9;
float smoothed = 0;
float alpha = 0.2;

Servo myServo;

// PID constants - tune these
float Kp = 0.2;
float Kd = 0.0;

float lastError = 0;
unsigned long lastTime = 0;

// Calibration lookup table: {ADC, distance_cm}
const int NUM_CAL_POINTS = 16;
const float calTable[NUM_CAL_POINTS][2] = {
  {583.69, 13.0}, {564.56, 15.0}, {533.65, 18.0}, {515.26, 20.0},
  {480.56, 23.0}, {432.95, 28.0}, {382.91, 33.0}, {339.87, 38.0},
  {297.84, 43.0}, {249.49, 53.0}, {219.55, 63.0}, {195.27, 73.0},
  {173.48, 83.0}, {151.77, 93.0}, {133.37, 103.0}, {115.29, 107.0}
};

float adcToDistance(float adc) {
  if (adc >= calTable[0][0]) return calTable[0][1];
  if (adc <= calTable[NUM_CAL_POINTS - 1][0]) return calTable[NUM_CAL_POINTS - 1][1];

  for (int i = 0; i < NUM_CAL_POINTS - 1; i++) {
    if (adc <= calTable[i][0] && adc >= calTable[i + 1][0]) {
      float ratio = (adc - calTable[i + 1][0]) / (calTable[i][0] - calTable[i + 1][0]);
      return calTable[i + 1][1] + ratio * (calTable[i][1] - calTable[i + 1][1]);
    }
  }
  return -1;
}

int computeServoAngle(float currentDist, float targetDist) {
  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0;
  if (dt <= 0) dt = 0.001;

  float error = targetDist - currentDist;
  float dError = (error - lastError) / dt;

  float output = Kp * error + Kd * dError;

  // 120 is roughly level — adjust to your beam's neutral angle
  int angle = 120 + (int)output;

  // Clamp to 60-180
  if (angle < 60) angle = 60;
  if (angle > 180) angle = 180;

  lastError = error;
  lastTime = now;

  return angle;
}

void setup() {
  Serial.begin(9600);
  myServo.attach(servoPin);
  myServo.write(120); // start level
  smoothed = analogRead(irPin);
  delay(500); // let servo settle
  lastTime = millis(); // after delay, not before
}

void loop() {

  int raw = analogRead(irPin);

  smoothed = alpha * raw + (1 - alpha) * smoothed;

  float distance = adcToDistance(smoothed);
  float ballPosition = distance - 15.0;

  float target = 46.0; // target position in cm from sensor — adjust this

  int servoAngle = computeServoAngle(distance, target);
  myServo.write(servoAngle);

  Serial.print("Pos:");
  Serial.print(ballPosition, 1);
  Serial.print(" Err:");
  Serial.print(target - distance, 1);
  Serial.print(" Servo:");
  Serial.println(servoAngle);

  delay(10);
}
