// this is for the chasis where the car is, the code is functional it worked with the phones acclerometer and gyrometer, and the glove, dont forget to set the values of the phones accelrometer to f b l r respectively.
#include <SoftwareSerial.h>
SoftwareSerial bt(2, 3); // rx and tx

// Motor pins
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

String cmd = "";

// filtering gesture variables
char lastGesture = 'S';
unsigned long lastGestureTime = 0;
const unsigned long gestureDelay = 300;

// this is orientation direction varaibles ig
enum Orientation { STRAIGHT, LEFT_TURNED, RIGHT_TURNED, DOUBLE_LEFT, DOUBLE_RIGHT };
Orientation currentDir = STRAIGHT;

void setup() {
  bt.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();
}

void loop() {

  while (bt.available()) {
    char c = bt.read();
    Serial.println(c);

    // ts for gesture part from phone and glove
    if (c == 'F' || c == 'B' || c == 'L' || c == 'R' || c == 'S') {

      if (c != lastGesture && millis() - lastGestureTime > gestureDelay) {
        lastGesture = c;
        lastGestureTime = millis();

        if (c == 'F') forward();
        else if (c == 'B') backward();
        else if (c == 'L') left();       // simple turn
        else if (c == 'R') right();      // simple turn
        else if (c == 'S') stopMotors();
      }
    }

    
    else {
      cmd += c;
    }

    delay(2);
  }

  cmd.toLowerCase();

  // some smart code but this 1st cuz its longer
  if (cmd.indexOf("doubleleft") >= 0) {
    doubleleft();
    currentDir = DOUBLE_LEFT;
  }
  else if (cmd.indexOf("doubleright") >= 0) {
    doubleright();
    currentDir = DOUBLE_RIGHT;
  }
  else if (cmd.indexOf("forward") >= 0) {
    forward();
  }
  else if (cmd.indexOf("back") >= 0) {
    backward();
  }
  else if (cmd.indexOf("left") >= 0) {
    handleLeft();
  }
  else if (cmd.indexOf("right") >= 0) {
    handleRight();
  }
  else if (cmd.indexOf("straight") >= 0) {
    straight();
  }
  else if (cmd.indexOf("stop") >= 0) {
    stopMotors();
  }
  else if (cmd.indexOf("360") >= 0) {
    s360();
  }

  cmd = "";
}

// some "smart code" again for straightening and stuff but the right and left logic dont quite work rn

void handleLeft() {
  if (currentDir == LEFT_TURNED) {
    doubleleftq();
  } else {
    left();
  }
  currentDir = LEFT_TURNED;
}

void handleRight() {
  if (currentDir == RIGHT_TURNED) {
    doublerightq();
  } else {
    right();
  }
  currentDir = RIGHT_TURNED;
}

void straight() {
  if (currentDir == LEFT_TURNED) {
    right();
  }
  else if (currentDir == RIGHT_TURNED) {
    left();
  }
  else if (currentDir == DOUBLE_RIGHT){
    doubleleftq();
  }
  else if (currentDir == DOUBLE_LEFT){
    doublerightq();
  }
  currentDir = STRAIGHT;
}

void doubleleft() {
  if (currentDir == DOUBLE_RIGHT){
    doubleleftq();
    doubleleftq();
  }
  else {
    doubleleftq();
  }
  currentDir = DOUBLE_LEFT;
}

void doubleright() {
  if (currentDir == DOUBLE_LEFT){
    doublerightq();
    doublerightq();
  }
  else {
    doublerightq();
  }
  currentDir = DOUBLE_RIGHT;
}
// what the commands actually redirect u to

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(1000);
  stopMotors();
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(1000);
  stopMotors();
}

void left() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(598);      // calibrated to 90° depends on ur motors
  stopMotors();
}

void right() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(618);      // calibrated to 90 deegrees
  stopMotors();
}

void doublerightq() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(1236);      // 180°
  stopMotors();
}

void doubleleftq() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(1192);      // 180°
  stopMotors();
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
void s360() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(1500);    
  stopMotors();

}
