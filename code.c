// ---------- ULTRASONIC ----------
const int trigPin1 = A1;
const int echoPin1 = A0;
const int trigPin2 = A2;
const int echoPin2 = A3;

// ---------- OTHERS ----------
const int buzzerPin = 4;   // BUZZER
const int rainPin   = A4;

// ---------- SIREN ----------
void sirenSound() {
  for (int f = 900; f <= 2800; f += 25) {
    tone(buzzerPin, f);
    delay(4);
  }
  for (int f = 2800; f >= 900; f -= 25) {
    tone(buzzerPin, f);
    delay(4);
  }
  noTone(buzzerPin);
}

void setup() {
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(buzzerPin, OUTPUT);
  pinMode(rainPin, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("System Ready (No GSM / No GPS)");
}

// ---------- ULTRASONIC READ ----------
int readUltrasonic(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH, 25000);
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}

void loop() {

  int d1 = readUltrasonic(trigPin1, echoPin1);
  int d2 = readUltrasonic(trigPin2, echoPin2);

  // ---- SERIAL DISTANCE DISPLAY ----
  Serial.print("US1: ");
  Serial.print(d1);
  Serial.print(" cm  |  US2: ");
  Serial.print(d2);
  Serial.println(" cm");

  // ---- RAIN ALERT (CONTINUOUS) ----
  if (digitalRead(rainPin) == LOW) {
    Serial.println("Rain Alert");
    tone(buzzerPin, 3000);   // constant sound
    return;                 // skip other sounds
  } 
  else {
    noTone(buzzerPin);
  }

  // ---- BUZZER LOGIC (OBSTACLE) ----
  if (d1 <= 15 || d2 <= 15) {
    sirenSound();
  }
  else if (d1 <= 20 || d2 <= 20) {
    tone(buzzerPin, 2500);
    delay(300);
    noTone(buzzerPin);
    delay(300);
  }
  else {
    noTone(buzzerPin);
  }

  delay(200);
}
