#include "Adafruit_FONA.h"
#define FONA_RX 7
#define FONA_TX 8
#define FONA_RST 6

char replybuffer[255];

#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(8, 7);
SoftwareSerial *fonaSerial = &fonaSS;

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);
uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

uint8_t type;

#include <Servo.h> 
int servoPin = 3; 
Servo Servo1; 



#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C  lcd(0x27, 2, 1, 0, 4, 5, 6, 7);


const int trigPin = 11;
const int echoPin = 10;
long duration;
int distance;


int LedRed =  13;
int LedGreen = 9;
const int buzzer = 5;


bool gprs = false;



//////////////////////////////SETTINGS
void settings() {
  pinMode(buzzer, OUTPUT);
  pinMode(LedRed, OUTPUT);
  pinMode(LedGreen, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}




////////////////////////////////SETUP//////////////////////////////
void setup() {
  // put your setup code here, to run once:
  lcd.begin (16, 2); // for 16 x 2 LCD module
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);

  Servo1.attach(servoPin);

  settings();

  while (!Serial);

  Serial.begin(115200);
  Serial.println(F("FONA basic test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));

  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  type = fona.type();
  Serial.println(F("FONA is OK"));
  Serial.print(F("Found "));
  switch (type) {
    case FONA800L:
      Serial.println(F("FONA 800L")); break;
    case FONA800H:
      Serial.println(F("FONA 800H")); break;
    case FONA808_V1:
      Serial.println(F("FONA 808 (v1)")); break;
    case FONA808_V2:
      Serial.println(F("FONA 808 (v2)")); break;
    case FONA3G_A:
      Serial.println(F("FONA 3G (American)")); break;
    case FONA3G_E:
      Serial.println(F("FONA 3G (European)")); break;
    default:
      Serial.println(F("???")); break;
  }

  // Print module IMEI number.
  char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("Module IMEI: "); Serial.println(imei);
  }
  delay(1000);

  fona.setGPRSNetworkSettings(F("Internet"));
  delay(3000);
}




/////////////////////////LOOPS////////////////////////////////////////////

void lcdup () {
  lcd.clear();
  lcd.home (); // set cursor to 0,0
  lcd.print(" Burglar Sys.");
  lcd.setCursor (0, 1);
  lcd.print("Intruder detected!");
  lcd.setBacklight(LOW);      // Backlight off
  delay(250);
  lcd.setBacklight(HIGH);     // Backlight on
  delay(1000);
}

void lcddp () {
  lcd.clear();
  lcd.home (); // set cursor to 0,0
  lcd.print(" Burglar Sys.");
  lcd.setCursor (0, 2);
  lcd.print("No intruder!");
  lcd.setBacklight(LOW);      // Backlight off
  delay(250);
  lcd.setBacklight(HIGH);     // Backlight on
  delay(1000);
}

void motor(){
   Servo1.write(180); 
}




void loop() {
  uint8_t n = fona.getNetworkStatus();
  Serial.print(F("Network status "));
  Serial.print(n);
  Serial.print(F(": "));
  if (n == 0) Serial.println(F("Not registered"));
  if (n == 1) {
    Serial.println(F("Registered (home)"));
    if (!gprs) {
      if (!fona.enableGPRS(true)) {
        Serial.println("Failed to activate GPRS");
      }
      else {
        Serial.println("GPRS activated");
        gprs = true;
      }
    }
  }
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  
  if (distance < 15) {

    digitalWrite(LedRed, HIGH);
    delay(500);
    digitalWrite(LedGreen, LOW);
    lcdup();
    motor();
    tone(buzzer, 1000); // Send 1KHz sound signal...
    delay(1000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(1000);        // ...for 1sec
    if (n == 1) {
      SMS(distance);
      delay(1000);
    }
    }
    else {
      digitalWrite(LedGreen, HIGH);
      digitalWrite(LedRed, LOW);
      lcddp();
    }
  }



///////////////////////Alert Notification///////////////////////////////////
  void SMS(int input) {
    char sendto[21] = "+233247145596";
    String message = "Intruder detected!";
    char newMessage[141];
    message.toCharArray(newMessage, sizeof(newMessage));

    if (!fona.sendSMS(sendto, newMessage)) {
      Serial.println(F("Emergency SMS Sent!"));
    } else {
      Serial.println(F("Failed to send emergency SMS"));
    }
  }
