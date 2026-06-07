/*
 * ARDUINO BASED AUTOMATED GARBAGE DISPOSAL SYSTEM
 * 
 * Features:
 * - Ultrasonic sensor (HC-SR04) detects garbage level inside bin
 * - IR sensor detects hand/object near lid for automatic opening
 * - Servo motor opens/closes lid automatically
 * - LCD (16x2 I2C) displays bin status and fill level
 * - Buzzer alerts when bin is full
 * - LED indicators: Green (empty), Yellow (half), Red (full)
 * 
 * Circuit Connections:
 * Component          | Arduino Pin
 * -------------------|------------
 * Ultrasonic TRIG    | 9
 * Ultrasonic ECHO    | 10
 * IR Sensor OUT      | 8
 * Servo Signal       | 11
 * Buzzer             | 12
 * Green LED          | 5
 * Yellow LED         | 6
 * Red LED            | 7
 * LCD SDA            | A4 (I2C)
 * LCD SCL            | A5 (I2C)
 */

#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// Pin Definitions
#define TRIG_PIN  9
#define ECHO_PIN  10
#define IR_PIN    8
#define SERVO_PIN 11
#define BUZZER_PIN 12
#define LED_GREEN 5
#define LED_YELLOW 6
#define LED_RED   7

// Thresholds (in cm)
#define BIN_DEPTH_CM      30      // Total depth of bin
#define FULL_THRESHOLD_CM  5      // Fill level considered full (distance from top)
#define HALF_THRESHOLD_CM  15     // Half full threshold
#define OPEN_DURATION_MS   5000   // Lid stays open (ms)
#define SCAN_INTERVAL_MS   500    // Sensor read interval (ms)

// Objects
Servo lidServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// State variables
unsigned long lastScan = 0;
unsigned long lidOpenTime = 0;
bool lidOpen = false;
bool binFullAlerted = false;
int currentFillPercent = 0;

void setup() {
  // Initialize pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(IR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  // Initialize servo
  lidServo.attach(SERVO_PIN);
  lidServo.write(0);  // Lid closed
  delay(500);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Garbage Disposal");
  lcd.setCursor(0, 1);
  lcd.print("System Ready...");
  delay(2000);
  lcd.clear();

  // Startup beep
  tone(BUZZER_PIN, 1000, 200);
  delay(200);
  noTone(BUZZER_PIN);

  Serial.begin(9600);
  Serial.println("Automated Garbage Disposal System initialized.");
}

void loop() {
  unsigned long now = millis();

  if (now - lastScan >= SCAN_INTERVAL_MS) {
    lastScan = now;

    // Read sensors
    int distance = readUltrasonic();
    int objectDetected = digitalRead(IR_PIN);  // HIGH = object near

    // Calculate fill percentage (0% = empty bin, 100% = full)
    currentFillPercent = calculateFillPercent(distance);
    
    // Auto-open lid when hand/object detected near opening
    if (objectDetected == HIGH && !lidOpen) {
      openLid();
    }

    // Auto-dose lid after timeout
    if (lidOpen && (now - lidOpenTime >= OPEN_DURATION_MS)) {
      closeLid();
    }

    // Update indicators
    updateIndicators(currentFillPercent, distance);
    
    // Update LCD
    updateDisplay(currentFillPercent, distance, objectDetected);

    // Debug serial output
    Serial.print("Distance: "); Serial.print(distance);
    Serial.print(" cm | Fill: "); Serial.print(currentFillPercent);
    Serial.print("% | IR: "); Serial.print(objectDetected);
    Serial.print(" | Lid: "); Serial.println(lidOpen ? "OPEN" : "CLOSED");
  }
}

/**
 * Reads distance from HC-SR04 ultrasonic sensor.
 * Returns distance in centimeters, or -1 on error.
 */
int readUltrasonic() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);  // Timeout at ~5m
  if (duration == 0) return -1;  // No echo

  int distance = duration * 0.034 / 2;
  return constrain(distance, 0, 100);
}

/**
 * Calculates bin fill percentage from distance reading.
 * 0% = empty (distance == BIN_DEPTH_CM), 100% = full (distance ~ 0)
 */
int calculateFillPercent(int distance) {
  if (distance <= 0) return 100;
  if (distance >= BIN_DEPTH_CM) return 0;
  return map(distance, BIN_DEPTH_CM, 0, 0, 100);
}

/**
 * Opens the lid by rotating the servo to 90 degrees.
 */
void openLid() {
  lidServo.write(90);
  lidOpen = true;
  lidOpenTime = millis();
  Serial.println("Lid OPENED");
}

/**
 * Closes the lid by rotating the servo back to 0 degrees.
 */
void closeLid() {
  lidServo.write(0);
  lidOpen = false;
  Serial.println("Lid CLOSED");
}

/**
 * Updates LED indicators and buzzer based on fill level.
 */
void updateIndicators(int fillPercent, int distance) {
  if (fillPercent >= 90) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, HIGH);
    if (!binFullAlerted) {
      // Alert pattern: beep 3 times
      for (int i = 0; i < 3; i++) {
        tone(BUZZER_PIN, 2000, 150);
        delay(200);
        noTone(BUZZER_PIN);
        delay(150);
      }
      binFullAlerted = true;
    }
  } else if (fillPercent >= 50) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, LOW);
    binFullAlerted = false;
  } else {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);
    binFullAlerted = false;
  }
}

/**
 * Updates the LCD display with current status and fill level.
 */
void updateDisplay(int fillPercent, int distance, int objectDetected) {
  lcd.clear();
  
  // Line 1: Fill level bar + percentage
  lcd.setCursor(0, 0);
  lcd.print("Fill: ");
  lcd.print(fillPercent);
  lcd.print("% ");
  lcd.print(getFillBar(fillPercent));
  
  // Line 2: Status
  lcd.setCursor(0, 1);
  if (fillPercent >= 90) {
    lcd.print("! BIN FULL !");
  } else if (fillPercent >= 50) {
    lcd.print("Half Full     ");
  } else if (lidOpen) {
    lcd.print("Lid Open      ");
  } else if (distance < 0) {
    lcd.print("Sensor Error  ");
  } else {
    lcd.print("Ready         ");
  }
}

/**
 * Returns a visual bar representation of the fill level.
 */
String getFillBar(int percent) {
  int bars = map(percent, 0, 100, 0, 8);
  String bar = "";
  for (int i = 0; i < 8; i++) {
    bar += (i < bars) ? (char)255 : ' ';  // Block character
  }
  return bar;
}
