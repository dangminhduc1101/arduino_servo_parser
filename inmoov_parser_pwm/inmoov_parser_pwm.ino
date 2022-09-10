/* Libraries */
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


/*Constants*/
#define MODULE_COUNT 3
#define SERVO_COUNT 5
#define SERVO_FREQ 50

String splitString(String text, char separator, int index)
{
  int found = 0;
  int pos[] = {0, -1};

  for (int i = 0; i < text.length() && found <= index; i++) {
    if (text.charAt(i) == separator || i == text.length() - 1) {
      found++;
      pos[0] = pos[1] + 1;
      pos[1] = (i == text.length() - 1) ? i + 1 : i;
    }
  }

  String s = found > index ? text.substring(pos[0], pos[1]) : "";
  s.trim();
  return s;
}

String getModule(String text, int index) {
  return splitString(text, '+', index);
}

String getServo(String text, int index) {
  return splitString(text, '/', index);
}

int getPort(String text) {
  return splitString(text, '.', 0).toInt();
}

int getAttached(String text) {
  return splitString(text, '.', 1).toInt();
}

int getMin(String text) {
  return splitString(text, '.', 2).toInt();
}

int getMax(String text) {
  return splitString(text, '.', 3).toInt();
}

int getPos(String text) {
  return splitString(text, '.', 4).toInt();
}

/* Initialize Servo Shield */
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);

  Serial.begin(115200);
  Serial.setTimeout(50);
}

void loop() {
  if (Serial.available() > 0) {
    String s = Serial.readString();
    for (int i = 0; i < MODULE_COUNT; i++) {
      String module = getModule(s, i);
      if (!module.equals("NA")) {
        for (int j = 0; j < SERVO_COUNT; j++) {
          String servo = getServo(module, j);
          if (getAttached(servo) > 0) {
            pwm.setPWM(getPort(servo), 0, int(float(map(getPos(servo), 0, 180, getMin(servo), getMax(servo))) / 1000000 * SERVO_FREQ * 4096));
          }
        }
      }
    }
  }
}
