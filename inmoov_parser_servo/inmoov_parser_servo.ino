/* Libraries */
#include <Servo.h>

/*Constants*/
#define MODULE_COUNT 3
#define SERVO_COUNT 5

Servo* control[MODULE_COUNT * SERVO_COUNT];

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

void setServo(Servo servo, int port, int pos, int maxi, int mini, int att) {
  if (att > 0) {
    if (!servo.attached()) {
      servo.attach(port);
    }
    servo.writeMicroseconds(int(float(map(pos, 0, 180, mini, maxi))));
  }
  else if (servo.attached()) {
    servo.detach();
  }
}

void setup() {
  for (int i = 0; i < MODULE_COUNT * SERVO_COUNT; i++) {
    control[i] = new Servo();
  }
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
          setServo(*control[i * j], getPort(servo), getPos(servo), getMax(servo), getMin(servo), getAttached(servo));
        }
      }
    }
  }
}
