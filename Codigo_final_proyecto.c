#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Servo servo;
Servo servoExtra;  
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int VRx = 34;
const int SW = 32;
const int servoPin = 13;
const int servoExtraPin = 14;  

int angle = 90;  
int lastState = HIGH;
int currentState = HIGH;
unsigned long lastMove = 0;
const int delayMove = 300;  

void setup() {
  Serial.begin(115200);
  servo.attach(servoPin);
  servoExtra.attach(servoExtraPin);  
  servo.write(angle);
  servoExtra.write(0); 

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
  currentState = digitalRead(SW);

  if (currentState == LOW && lastState == HIGH) {
    servoExtra.write(90);
    mostrarLCD("Btn Presionado", 90, true);
    delay(200);  
  }
  else if (currentState == HIGH && lastState == LOW) {
    servoExtra.write(0);
    mostrarLCD("Btn Liberado", 0, true);
  }
  if (millis() - lastMove > delayMove) {
    if (xValue > 3000 && angle < 180) {
      angle += 90;
      if (angle > 180) angle = 180;
      servo.write(angle);
      mostrarLCD("Derecha", angle, false);
      lastMove = millis();
    }
    // Mover a la izquierda
    else if (xValue < 1000 && angle > 0) {
      angle -= 90;
      if (angle < 0) angle = 0;
      servo.write(angle);
      mostrarLCD("Izquierda", angle, false);
      lastMove = millis();
    }
  }
  if (currentState == LOW && millis() - lastMove > delayMove) {
    angle = 90;
    servo.write(angle);
    mostrarLCD("Centro", angle, false);
    lastMove = millis();
    delay(500);
  }
  
  lastState = currentState;
}

void mostrarLCD(String direccion, int grados, bool esServoExtra) {
  lcd.clear();
  lcd.setCursor(0, 0);
  
  if (esServoExtra) {
    lcd.print("Servo Extra:");
    lcd.setCursor(0, 1);
    lcd.print(direccion);
  } else {
    lcd.print("Servo Principal:");
    lcd.setCursor(0, 1);
    lcd.print(direccion + " ");
    lcd.print(grados);
    lcd.print((char)223);
  }
}