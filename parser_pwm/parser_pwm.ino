/* Libraries */
#include <Adafruit_PWMServoDriver.h>

/* Constants */
#define SERVO_FREQ 50
#define INPUT_MAX_LENGTH 50
#define SERVO_MAX_COUNT 16

/* Variables */
char s[INPUT_MAX_LENGTH];
bool ipt_read = false;

int pin;
int enabled;
int ms_min;
int ms_max;
int pos;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

/* Helper Functions */
void readInput() {
  static int i = 0;
  char c = Serial.read();

  if (c > 0 && ipt_read == false) {
    switch (c) {
      case '\r':
        break;
      case '\n':
        s[i] = '\0';
        i = 0;
        ipt_read = true;
        break;
      default:
        s[i] = c;
        i++;
    }
  }
}

const char* parseInput(const char* input, char delimiter, int index) {
  int idx_delimtr = 0;
  int idx_first = 0;
  int idx_last = -1;
  int ipt_length = strlen(input);

  for (int i = 0; i < ipt_length && idx_delimtr <= index; i++) {
    if (input[i] == delimiter || i == ipt_length - 1) {
      idx_delimtr++;
      idx_first = idx_last + 1;
      idx_last = (i == ipt_length - 1) ? i + 1 : i;
    }
  } // find the indexes of the delimiter argument, and the first and last character of the string.

  if (idx_delimtr <= index) {
    return "N/A";
  } // if index argument too high

  char* ipt_substr = malloc(idx_last - idx_first + 1);
  strncpy(ipt_substr, &input[idx_first], idx_last - idx_first);
  ipt_substr[idx_last - idx_first] = '\0';
  return ipt_substr;
}

int convertInput(const char* input, int index) {
  char* i = parseInput(input, '.', index);
  if (strcmp(i, "N/A") == 0) {
    return -2;
  } // if arguument index too high, use for exception of inappropriate string length

  int ipt_converted = atoi(i);
  char str_parsed[INPUT_MAX_LENGTH];
  sprintf(str_parsed, "%d", ipt_converted);
  if (strcmp(i, str_parsed) != 0) {
    free(i);
    return -1;
  } // exception handling for atoi()
  free(i);
  return ipt_converted;
}

int parsePin(const char* input) {
  int p = convertInput(input, 0);
  return (p < 0 || p > 50) ? -1 : p;
}

int parseEnabled(const char* input) {
  int e = convertInput(input, 1);
  return (e == 0 || e == 1) ? e : -1;
}

int parseMinMax(const char* input, bool minimum) {
  int mn = convertInput(input, 2);
  int mx = convertInput(input, 3);
  if (mn >= mx || mn < 0 || mn > 3000 || mx < 0 || mx > 3000) {
    return -1;
  }
  return minimum ? mn : mx;
}

int parsePos(const char* input) {
  int p = convertInput(input, 4);
  return (p < 0 || p > 180) ? -1 : p;
}

bool excessInput(const char* input) {
  int p = convertInput(input, 5);
  return p != -2;
}

void updateOutput() {
  if (ipt_read == true) {
    pin = parsePin(s);
    enabled = parseEnabled(s);
    ms_min = parseMinMax(s, true);
    ms_max = parseMinMax(s, false);
    pos = parsePos(s);
    Serial.println(pin);
    Serial.println(enabled);
    Serial.println(ms_min);
    Serial.println(ms_max);
    Serial.println(pos);        // use only for debugging, else put as comment, since Serial.println() adds unread strings into the buffer and slows down the parser
    if (pin == -1 || excessInput(s)) {
      /*
        Serial.println("Invalid input");      // use only for debugging, else put as comment
      */
      for (int i = 0; i < SERVO_MAX_COUNT; i++) {
        pwm.setPWM(i, 0, 4096);
      }
    }
    else if (enabled == -1 || ms_min == -1 || ms_max == -1 || pos == -1) {
      /*
        Serial.println("Invalid input");      // use only for debugging, else put as comment
      */
      pwm.setPWM(pin, 0, 4096);
    }
    else if (enabled == 0) {
      /*
        Serial.println("Disabled"); // use only for debugging, else put as comment
      */
      pwm.setPWM(pin, 0, 4096);
    }
    else {
      pwm.setPWM(pin, 0, int(float(map(pos, 0, 180, ms_min, ms_max)) / 1000000 * SERVO_FREQ * 4096));
    }
    ipt_read = false;
  }
}

/* Main functions */
void setup() {
  Serial.begin(115200);

  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
}

void loop() {
  readInput();
  updateOutput();
}
