int sensorPins[4] = {A0, A1, A2, A3};
int sensorValues[4] = {0, 0, 0, 0};

void setup() {
  Serial.begin(9600);
}

void loop() {
  // sensoren werden hier auslesen
  for (int i = 0; i < 4; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
  }

  // sensorwerte werden hier ausgegebn
  Serial.println("Sensorwerte:");
  Serial.print("A0 (oben links):    "); Serial.println(sensorValues[0]);
  Serial.print("A1 (oben rechts):   "); Serial.println(sensorValues[1]);
  Serial.print("A2 (unten links):   "); Serial.println(sensorValues[2]);
  Serial.print("A3 (unten rechts):  "); Serial.println(sensorValues[3]);

  // mittelwerte berechnen werden hier berechnet
  int leftAvg   = (sensorValues[0] + sensorValues[2]) / 2;
  int rightAvg  = (sensorValues[1] + sensorValues[3]) / 2;
  int topAvg    = (sensorValues[0] + sensorValues[1]) / 2;
  int bottomAvg = (sensorValues[2] + sensorValues[3]) / 2;

  // differenzen
  int horizontalDiff = leftAvg - rightAvg;
  int verticalDiff   = topAvg - bottomAvg;

  // print der Mittelwerte
  Serial.println("\nMittelwerte:");
  Serial.print("Links:  "); Serial.println(leftAvg);
  Serial.print("Rechts: "); Serial.println(rightAvg);
  Serial.print("Oben:   "); Serial.println(topAvg);
  Serial.print("Unten:  "); Serial.println(bottomAvg);

  // print der Differenzen
  Serial.println("\nDifferenzen:");
  Serial.print("Horizontal (L-R): "); Serial.println(horizontalDiff);
  Serial.print("Vertikal (O-U):   "); Serial.println(verticalDiff);

  // toleranz halt
  int tolerance = 5;

  Serial.println("\nAktion:");

  // horizontal schauen, ob eine Bewegung nötig ist
  if (horizontalDiff > tolerance) {
    Serial.print("Drehe NACH LINKS ");
    Serial.print(horizontalDiff);
    Serial.println(" Einheiten");
  } else if (horizontalDiff < -tolerance) {
    Serial.print("Drehe NACH RECHTS ");
    Serial.print(-horizontalDiff);
    Serial.println(" Einheiten");
  } else {
    Serial.println("✅ Horizontal ausgerichtet");
  }

  // vertikal schauen, ob eine Drehung benötigt wird
  if (verticalDiff > tolerance) {
    Serial.print("⬆ Neige NACH OBEN ");
    Serial.print(verticalDiff);
    Serial.println(" Einheiten");
  } else if (verticalDiff < -tolerance) {
    Serial.print("⬇ Neige NACH UNTEN ");
    Serial.print(-verticalDiff);
    Serial.println(" Einheiten");
  } else {
    Serial.println("✅ Vertikal ausgerichtet");
  }

  Serial.println("\n-----------------------------\n");

  delay(5000);
}
