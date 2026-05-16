#include <Servo.h>
#include <LiquidCrystal.h>

//
// LCD
//
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//
// PINS
//
#define LDR_PIN A0
#define GAS_PIN A1
#define TEMP_PIN A2

#define GREEN_LED A3
#define SERVO_PIN A4

#define TRIG_PIN 6
#define ECHO_PIN 7

#define BUZZER_PIN 8
#define PIR_PIN 9

#define RED_LED 10

#define RELAY_PIN 13

//
// VARIABLES
//
Servo doorServo;

int gasLimit = 400;

//
// FUNCTION : DISTANCE
//
long getDistance()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);

  long distance = duration * 0.034 / 2;

  return distance;
}

void setup()
{
  //
  // SERIAL
  //
  Serial.begin(9600);

  //
  // LCD
  //
  lcd.begin(16, 2);

  //
  // PIN MODES
  //
  pinMode(LDR_PIN, INPUT);
  pinMode(GAS_PIN, INPUT);
  pinMode(TEMP_PIN, INPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(PIR_PIN, INPUT);

  pinMode(RELAY_PIN, OUTPUT);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  //
  // SERVO
  //
  doorServo.attach(SERVO_PIN);

  //
  // START MESSAGE
  //
  lcd.setCursor(0, 0);
  lcd.print("SMART HOME");

  lcd.setCursor(0, 1);
  lcd.print("SYSTEM READY");

  delay(2000);

  lcd.clear();
}

void loop()
{
  //
  // =========================
  // TEMPERATURE
  // =========================
  //

  float temp =
  ((analogRead(TEMP_PIN) * 5.0 / 1024.0) - 0.5) / 0.01;

  //
  // =========================
  // GAS SENSOR
  // =========================
  //

  int gasValue = analogRead(GAS_PIN);

  //
  // GAS ALERT
  //
  if (gasValue > gasLimit)
  {
    tone(BUZZER_PIN, 1000);

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("GAZ PROBLEME!");

    lcd.setCursor(0, 1);
    lcd.print("DANGER !!!");

    Serial.println("GAS DETECTED");

    delay(1000);

    return;
  }
  else
  {
    noTone(BUZZER_PIN);
  }

  //
  // =========================
  // LDR CONTROL
  // =========================
  //

  int lightValue = analogRead(LDR_PIN);

  //
  // DARK => LIGHT ON
  //
  if (lightValue < 500)
  {
    digitalWrite(RELAY_PIN, HIGH);
  }
  else
  {
    digitalWrite(RELAY_PIN, LOW);
  }

  

 //
// =========================
// PIR SENSOR
// =========================
//

int motion = digitalRead(PIR_PIN);

if (motion == HIGH)
{
    Serial.println("Motion Detected");

    //
    // Activer lampe/ventilateur
    //
    digitalWrite(RELAY_PIN, HIGH);

    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("MOUVEMENT");

    lcd.setCursor(0,1);
    lcd.print("DETECTE !");
}
else
{
    digitalWrite(RELAY_PIN, LOW);
}
  //
  // =========================
  // ULTRASONIC SENSOR
  // =========================
  //

  long distance = getDistance();

  //
  // OPEN DOOR
  //
  if (distance < 50)
  {
    doorServo.write(90);

    Serial.println("Door Open");
  }
  else
  {
    doorServo.write(0);

    Serial.println("Door Closed");
  }

  //
  // =========================
  // LCD DISPLAY
  // =========================
  //

  lcd.clear();

  //
  // LINE 1
  //
  lcd.setCursor(0, 0);

  lcd.print("T:");
  lcd.print(temp);

  lcd.print((char)223);

  lcd.print("C");

  //
  // LINE 2
  //
  lcd.setCursor(0, 1);

  lcd.print("D:");
  lcd.print(distance);

  lcd.print("cm");

  //
  // SERIAL MONITOR
  //
  Serial.print("Temp = ");
  Serial.print(temp);

  Serial.print(" | Gas = ");
  Serial.print(gasValue);

  Serial.print(" | Light = ");
  Serial.print(lightValue);

  Serial.print(" | Distance = ");
  Serial.println(distance);

  delay(500);
}