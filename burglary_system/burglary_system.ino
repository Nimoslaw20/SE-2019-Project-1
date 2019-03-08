
//Declarations
const int trigPin = 13;
const int echoPin = 12;

int LedYellow = 10;
int LedRed =  11;
int LedGreen = 9;


const int buttonPin = 2;
const int Led1 = 4;
const int Led2 = 5;

int buttonState = 0;


float duration, distance, meters;


#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR  0x3F
#define En_pin    2
#define Rw_pin    1
#define Rs_pin    0
#define D4_pin    4
#define D5_pin    5
#define D6_pin    6
#define D7_pin    7
#define BACKLIGHT_PIN   3

LiquidCrystal_I2C lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin,
                      D4_pin, D5_pin, D6_pin, D7_pin);



void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  pinMode(LedYellow, OUTPUT);
  pinMode(LedRed, OUTPUT);
  pinMode(LedGreen, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);

  lcd.begin (20, 4);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH ) {
    digitalWrite(Led1, HIGH);
    digitalWrite(Led2, LOW);

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration * .0343) / 2; //distance in centimeters
    //meters = distance/100;       //distance in meters
    //int range = map(meters, sensorMin, sensorMax, 0,2);


    Serial.print("Distance: ");
    Serial.println(distance);
    delay(1000);
    Serial.println("Meters: ");
    Serial.println(meters);
    delay(1000);

    lcd.home();
    lcd.setCursor(0, 0);
    lcd.print("Burglary Sys.");
    lcd.setCursor(0, 1);
    lcd.print("Distance: ");
    lcd.print(distance);
    lcd.print(" cm");

    if (distance < 2500) {
      digitalWrite(LedGreen, HIGH);
      digitalWrite(LedYellow, LOW);
      digitalWrite(LedRed, LOW);
    }

    if (distance <= 500) {
      digitalWrite(LedYellow, HIGH);
      digitalWrite(LedRed, LOW);
    }

    if (distance <= 100) {
      digitalWrite(LedRed, HIGH);
    }
  }

  // put your main code here, to run repeatedly:


}

