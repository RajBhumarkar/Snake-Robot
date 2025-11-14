#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Initialize the Servo Driver board at I2C address 0x40
Adafruit_PWMServoDriver board1 = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN 125  // Minimum pulse length (0 degrees)
#define SERVOMAX 625  // Maximum pulse length (180 degrees)

const int noServos = 5;        // Number of servos

// Yaw servos (horizontal movement) - indices 0, 2, 4
const float yawAmplitude = 40.0;   // Horizontal wave amplitude in degrees
const float yawMidAngle = 90.0;    // Center position for yaw

// Pitch servos (vertical movement) - indices 1, 3
const float pitchAmplitude = 10.0;  // Small vertical lift
const float pitchMidAngle = 90.0;   // Center position for pitch

const float WAVE_SPEED = 0.05;      // Speed of the wave

// Phase shift creates the traveling wave from head to tail
const float PHASE_SHIFT_PER_SEGMENT = 1.2;

const float Pi = 3.14159265359;
float t = 0.0; // Time variable

void setup() {
  Serial.begin(9600);
  Serial.println(F("Snake Robot - Forward Motion"));
  Serial.println(F("Layout: 0=Yaw, 1=Pitch, 2=Yaw, 3=Pitch, 4=Yaw"));
  
  board1.begin();
  board1.setPWMFreq(50); // 50Hz for analog servos
  
  // Initialize all servos to middle position
  for (int i = 0; i < noServos; i++) {
    if (i % 2 == 0) {
      // Yaw servo
      board1.setPWM(i, 0, angleToPulse(yawMidAngle));
      Serial.print(F("Servo "));
      Serial.print(i);
      Serial.println(F(" (YAW) -> 90°"));
    } else {
      // Pitch servo
      board1.setPWM(i, 0, angleToPulse(pitchMidAngle));
      Serial.print(F("Servo "));
      Serial.print(i);
      Serial.println(F(" (PITCH) -> 90°"));
    }
  }
  
  delay(2000);
  Serial.println(F("Starting..."));
}

void loop() {
  for (int i = 0; i < noServos; i++) {
    float angle;
    
    if (i % 2 == 0) {
      // YAW SERVO (horizontal S-curve)
      int segmentIndex = i / 2;
      float phase = 2 * Pi * segmentIndex * PHASE_SHIFT_PER_SEGMENT;
      float waveValue = sin(2 * Pi * t + phase);
      angle = yawMidAngle + yawAmplitude * waveValue;
      
    } else {
      // PITCH SERVO (vertical motion)
      int segmentIndex = i / 2;
      float phase = 2 * Pi * segmentIndex * PHASE_SHIFT_PER_SEGMENT + Pi/2;
      float waveValue = sin(2 * Pi * t + phase);
      angle = pitchMidAngle + pitchAmplitude * waveValue;
    }
    
    // Constrain angle to servo limits
    angle = constrain(angle, 0, 180);
    
    // Write to servo
    board1.setPWM(i, 0, angleToPulse(angle));
  }
  
  // Advance wave in time
  t += WAVE_SPEED;
  if (t >= 1.0) t = 0.0;
  
  delay(30);
}

// Convert angle (0–180) to pulse length
int angleToPulse(float ang) {
  int pulse = map((int)ang, 0, 180, SERVOMIN, SERVOMAX);
  return pulse;
}
