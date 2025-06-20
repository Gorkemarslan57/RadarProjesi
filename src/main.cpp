#include <Arduino.h>
#include <Servo.h>

// Servo motor objesi oluştur
Servo myservo;

// Ultrasonik sensör pinleri
const int trigPin = 10;  // Trigger pin
const int echoPin = 11; // Echo pin
const int servoPin = 12; // Servo motor pin

// Diğer değişkenler
const int delayTime = 15; // Servo hareketleri arası gecikme
const float soundSpeed = 0.034; // cm/μs cinsinden ses hızı
long duration;
int distance;
int pos = 0;

void scan(int pos) {
  // Ultrasonik sensör ile mesafe ölçümü
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Echo süresini ölç
  duration = pulseIn(echoPin, HIGH);

  // Mesafeyi hesapla (cm cinsinden)
  distance = duration * soundSpeed / 2;

  // Ölçülen mesafe ve açı bilgisini seri porta gönder
  Serial.println(String(distance) + " " + String(pos));
}

void setup() {
  // Servo motor pini tanımla
  myservo.attach(servoPin);

  // Ultrasonik sensör pinlerini ayarla
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Seri haberleşmeyi başlat
  Serial.begin(9600);
}

void loop() {
  // Servo motoru 10° den 170° ye kadar döndür
  for (pos = 10; pos <= 170; pos += 1) {
    myservo.write(pos);
    scan(pos); // Her adımda tarama yap
    delay(delayTime);
  }

  // Servo motoru 170° den 10° ye kadar döndür
  for (pos = 170; pos >= 10; pos -= 1) {
    myservo.write(pos);
    scan(pos); // Her adımda tarama yap
    delay(delayTime);
  }
}