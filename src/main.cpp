#include <Arduino.h>
#include <Servo.h>


Servo myservo;

// ultrasonik sensör pinleri
const int trigPin = 10;  
const int echoPin = 11; 
const int servoPin = 12; 

// diğer değişkenler
const int delayTime = 15; 
const float soundSpeed = 0.034; 
long duration;
int distance;
int pos = 0;

void scan(int pos) {
  // ultrasonik sensör ile mesafe ölçümü
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // echo süresini ölç
  duration = pulseIn(echoPin, HIGH);

  // mesafeyi hesapla (cm cinsinden)
  distance = duration * soundSpeed / 2;

  // ölçülen mesafe ve açı bilgisini seri porta gönder
  Serial.println(String(distance) + " " + String(pos));
}

void setup() {
  // servo motor pini tanımla
  myservo.attach(servoPin);

  // ultrasonik sensör pinlerini ayarla
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // seri haberleşmeyi başlat
  Serial.begin(9600);
}

void loop() {
 
  for (pos = 10; pos <= 170; pos += 1) {
    myservo.write(pos);
    scan(pos); // Her adımda tarama yap
    delay(delayTime);
  }

  
  for (pos = 170; pos >= 10; pos -= 1) {
    myservo.write(pos);
    scan(pos); 
    delay(delayTime);
  }
}