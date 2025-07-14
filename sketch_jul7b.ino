const int sensorPins[4] = {A0, A1, A2, A3};
int sensorValues[4] = {0, 0, 0, 0};

struct Motor {
  int pinA;
  int pinB;
};

Motor horizontal = {9, 8};
Motor vertical   = {7, 6};

// multiplikator falls ein motor schneller muss
const int multiH = 1;
const int multiV = 1;

// 3 * 60 * 1000 = 180000 -> 3 Minuten
const unsigned long searchTime = 3UL * 60UL * 1000UL;

// 5 * 60 * 1000 = 300000 -> 5 Minuten
const unsigned long sleepTime  = 5UL * 60UL * 1000UL;

// wie viele ms jeder step für einen Motor "nutzt"
const int stepTime = 100;

void setup() {
  Serial.begin(9600);
  pinMode(horizontal.pinA, OUTPUT);
  pinMode(horizontal.pinB, OUTPUT);
  pinMode(vertical.pinA, OUTPUT);
  pinMode(vertical.pinB, OUTPUT);
}

void setMotor(Motor m, int dir) {
  if (dir > 0) {
    digitalWrite(m.pinA, HIGH);
    digitalWrite(m.pinB, LOW);
  } else if (dir < 0) {
    digitalWrite(m.pinA, LOW);
    digitalWrite(m.pinB, HIGH);
  } else {
    digitalWrite(m.pinA, LOW);
    digitalWrite(m.pinB, LOW);
  }
}

void moveMotor(Motor m, int dir, int dauer, int multi) {
  setMotor(m, dir);
  delay(dauer * multi);
  setMotor(m, 0);
}

void readSensors() {
  for (int i = 0; i < 4; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
  }
}

void printSensors() {
  Serial.println("Sensorwerte:");
  Serial.print("A0 (oben links):    "); Serial.println(sensorValues[0]);
  Serial.print("A1 (oben rechts):   "); Serial.println(sensorValues[1]);
  Serial.print("A2 (unten links):   "); Serial.println(sensorValues[2]);
  Serial.print("A3 (unten rechts):  "); Serial.println(sensorValues[3]);
}

void berechneDiff(int &h, int &v) {
  int leftAvg   = (sensorValues[0] + sensorValues[2]) / 2;
  int rightAvg  = (sensorValues[1] + sensorValues[3]) / 2;
  int topAvg    = (sensorValues[0] + sensorValues[1]) / 2;
  int bottomAvg = (sensorValues[2] + sensorValues[3]) / 2;
  h = leftAvg - rightAvg;
  v = topAvg - bottomAvg;
}

void sucheSonne() {
  unsigned long start = millis();
  int tolerance = 3;
  while (millis() - start < searchTime) {
    printSensors();
    int hDiff, vDiff;
    berechneDiff(hDiff, vDiff);

    Serial.println("\nAktion:");

    if (hDiff > tolerance) {
      Serial.print("Drehe NACH LINKS "); Serial.println(hDiff);
      moveMotor(horizontal, 1, stepTime, multiH);
    } else if (hDiff < -tolerance) {
      Serial.print("Drehe NACH RECHTS "); Serial.println(-hDiff);
      moveMotor(horizontal, -1, stepTime, multiH);
    } else {
      Serial.println("✅ Horizontal ausgerichtet");
    }

    if (vDiff > tolerance) {
      Serial.print("⬆ Neige NACH OBEN "); Serial.println(vDiff);
      moveMotor(vertical, 1, stepTime, multiV);
    } else if (vDiff < -tolerance) {
      Serial.print("⬇ Neige NACH UNTEN "); Serial.println(-vDiff);
      moveMotor(vertical, -1, stepTime, multiV);
    } else {
      Serial.println("✅ Vertikal ausgerichtet");
    }

    Serial.println("\n-----------------------------\n");
    delay(500);
  }
}

bool isNight() {
  int nightValue = 10;
  int sumSensors = 0;
  bool night = false;

  for (int i=0; i<4; i++){
    sumSensors += sensorValues[i];
  }

  return (sumSensors / 4) < nightValue;
}

void turnStartPosition() {
  // 1 * 60 * 1000 = 60000 -> 1 Minute
  int returnTime = 1UL * 60UL * 1000UL;
  // 60 * 60 * 1000 = 3600000 -> 60 Minute
  int nightTime = 3600000;
  moveMotor(vertical, -1, returnTime, multiV);
  moveMotor(horizontal, -1, returnTime, multiH);
  Serial.println("Auf Startposition zurückgesetzt");
  delay(nightTime);
}

void loop() {
  readSensors();
  if (isNight){
    turnStartPosition;
  } else {
    sucheSonne();
    setMotor(horizontal, 0);
    setMotor(vertical, 0);
    delay(sleepTime);
  }
}
