#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>

Adafruit_MPU6050 mpu;

// HC-05 bluetooth module integrate
SoftwareSerial bt(2, 3); // RX, TX

// Timing control (0.3 sec delay for each and every command, can be repeated)
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 300;

// Thresholds for detecting
const float forwardThreshold = -6.5;
const float backwardThreshold = 6.5;
const float leftThreshold = -6.5;
const float rightThreshold = 6.5;

char lastCommand = 'S';

void setup() {
  Serial.begin(9600);
  bt.begin(9600);

  Wire.begin();
  // this part checks if ur mpu6050 is working or nah
  if (!mpu.begin()) {
    Serial.println("MPU6050 not found!");
    while (1);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("Glove ready!");
  
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  char command = 'S';

  float x = a.acceleration.x;
  float y = a.acceleration.y;

  // logic for sending commands
  if (y < forwardThreshold) {
    command = 'F';
  }
  else if (y > backwardThreshold) {
    command = 'B';
  }
  else if (x < leftThreshold) {
    command = 'L';
  }
  else if (x > rightThreshold) {
    command = 'R';
  }
  else {
    command = 'S';
  }

  
  if (millis() - lastSendTime >= sendInterval && command != lastCommand) {
    bt.write(command);
    Serial.print("Sent: ");
    Serial.println(command);

    lastCommand = command;
    lastSendTime = millis();
  }
}
