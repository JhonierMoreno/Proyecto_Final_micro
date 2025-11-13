#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Objetos
Servo servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pines
const int VRx = 34;
const int SW = 32;
const int servoPin = 13;

// Variables
int angle = 90;  // posición inicial
int lastState = HIGH;
unsigned long lastMove = 0;
const int delayMove = 300;  // retardo entre movimientos

void setup() {
  Serial.begin(115200);
  servo.attach(servoPin);
  servo.write(angle);

  pinMode(VRx, INPUT);
  pinMode(SW, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Servo listo");
  delay(1000);
  lcd.clear();
}

void loop() {
  int xValue = analogRead(VRx);
  int button = digitalRead(SW);

  // Evita movimientos seguidos
  if (millis() - lastMove > delayMove) {
    // Mover a la derecha
    if (xValue > 3000 && angle < 180) {
      angle += 90;
      if (angle > 180) angle = 180;
      servo.write(angle);
      mostrarLCD("Derecha", angle);
      lastMove = millis();
    }
    // Mover a la izquierda
    else if (xValue < 1000 && angle > 0) {
      angle -= 90;
      if (angle < 0) angle = 0;
      servo.write(angle);
      mostrarLCD("Izquierda", angle);
      lastMove = millis();
    }
  }

  // Botón central (reset posición)
  if (button == LOW && lastState == HIGH) {
    angle = 90;
    servo.write(angle);
    mostrarLCD("Centro", angle);
    delay(500);
  }
  lastState = button;
}

void mostrarLCD(String direccion, int grados) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Servo giró:");
  lcd.setCursor(0, 1);
  lcd.print(direccion + " ");
  lcd.print(grados);
  lcd.print((char)223);
}