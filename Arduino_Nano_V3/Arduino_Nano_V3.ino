/*****************************************************************************
  Created by: Aditya Pratama. email: adityapratama141198@gmail.com. July 2022
  Energi Meter and Power Factor Correction Version 1.0
  Energy Meter and Correction the power factor for 1 phase
  This is open source code. Please include my name in copies of this code
  Thankyou ...
 *****************************************************************************/

#include "DS3231_Simple.h"
#include <Wire.h>
#include <PZEM004Tv30.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x27, 20, 4);
DS3231_Simple Clock;
PZEM004Tv30 pzem(8, 9);               // PZEM-004T Serial Communication at RX in pin 8 and TX in pin 9

// Initialize Pin Relay
const int relay1 = 4;
const int relay2 = 5;
const int relay3 = 6;
const int relay4 = 7;
const int relay5 = 12;
const int relay6 = 13;
int relayON = LOW;                   //Initialze relay on
int relayOFF = HIGH;                 //Initialze relay off

// Intialize mode
int mode = 1;

// Initialize for date and time
String myDate, myTime;
int Year, Month, Day, Hour, Minute, Second;

// constants won't change. They're used here to set pin numbers:
const int Button1 = 2;                      // the number of the button 1 pin. di PCB 2
const int Button2 = 3;                      // the number of the button 2 pin, di PCB 3
const int led1 = 10;                        // the number of the LED 1 pin, di PCB 10
const int led2 = 11;                        // the number of the LED 2 pin, di PCB 11

void setup() {
  Serial.begin(9600);
  Clock.begin();
  pinMode(led1, OUTPUT);                    // initialize the LED 1 pin as an output:
  pinMode(led2, OUTPUT);                    // initialize the LED 2 pin as an output:
  pinMode(Button1, INPUT_PULLUP);           // initialize the pushbutton 1 pin as an input:
  pinMode(Button2, INPUT_PULLUP);           // initialize the pushbutton 2 pin as an input:
  pinMode(relay1, OUTPUT);                  // initialize the relay 1 pin as an output:
  pinMode(relay2, OUTPUT);                  // initialize the relay 2 pin as an output:
  pinMode(relay3, OUTPUT);                  // initialize the relay 3 pin as an output:
  pinMode(relay4, OUTPUT);                  // initialize the relay 4 pin as an output:
  pinMode(relay5, OUTPUT);                  // initialize the relay 5 pin as an output:
  pinMode(relay6, OUTPUT);                  // initialize the relay 6 pin as an output:
  digitalWrite(relay1, relayOFF);           // initialize relay 1 is actif low
  digitalWrite(relay2, relayOFF);           // initialize relay 2 is actif low
  digitalWrite(relay3, relayOFF);           // initialize relay 3 is actif low
  digitalWrite(relay4, relayOFF);           // initialize relay 4 is actif low
  digitalWrite(relay5, relayOFF);           // initialize relay 5 is actif low
  digitalWrite(relay6, relayOFF);           // initialize relay 6 is actif low

  // Initialze LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(5, 0);
  lcd.print("LCD Ready!");
  delay(1000);

  // Mode selection start display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Silahkan pilih mode:");
  lcd.setCursor(0, 1);
  lcd.print("1.Monitoring Listrik");
  lcd.setCursor(0, 2);
  lcd.print("2.Perbaikan Cosphi");
  lcd.setCursor(0, 3);
  lcd.print("3.Reset");

  // Uncomment if you wanna reset the energy value in PZEM004Tv3.0
  // Serial.println("Reset Energy");
  // pzem.resetEnergy();
}

void sensor_PZEM_004T_reading() {
  // Reading data from PZEM-004T
  float V = pzem.voltage();
  float I = pzem.current();
  float S = V * I;
  float PF = pzem.pf() - 0.02;
  float P = pzem.power();
  float phi = acos(PF);
  float R = S * sin(phi);
  float F = pzem.frequency();
  float E = pzem.energy();

  float V_send = V * 100;
  float I_send = I * 100;
  float S_send = S * 100;
  float PF_send = PF * 100;
  float P_send = P * 100;
  float R_send = R * 100;
  float F_send = F * 100;
  float E_send = E * 100;
  // Collect data into variable (String) str
  String str = "*" +  String(V_send) + "," + String(I_send) + "," + String(S_send) + "," +  String(PF_send) + "," + String(P_send) + "," + String(R_send) + "," + String(F_send) + "," + String(E_send) + '#';
  
  // Sending data into ESP32 using serial communication
  Serial.println(str);
  delay(2000);

  // LCD display parameter
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(V, 1);
  lcd.print("V");
  lcd.setCursor(1, 1);
  lcd.print(I);
  lcd.print(" A");
  lcd.setCursor(1, 2);
  lcd.print(S, 1);
  lcd.print("VA");
  lcd.setCursor(1, 3);
  lcd.print(F, 1);
  lcd.print(" Hz");
  lcd.setCursor(11, 0);
  lcd.print("PF:");
  lcd.print(PF);
  lcd.setCursor(11, 1);
  lcd.print(P, 1);
  lcd.print("  W");
  lcd.setCursor(11, 2);
  lcd.print(R, 1);
  lcd.print("Var");
  lcd.setCursor(11, 3);
  lcd.print(E);
  lcd.print("kWh");
  delay(2000);
}

void RTC_time() {
  DateTime waktu;
  waktu = Clock.read();
  // Separate data between date and time
  Year = waktu.Year;
  Month = waktu.Month;
  Day = waktu.Day;
  Hour = waktu.Hour;
  Minute = waktu.Minute;
  Second = waktu.Second;
  myDate = String(Day) + "/" + String(Month) + "/" + String(Year);
  myTime = String(Hour) + ":" + String(Minute) + ":" + String(Second);
  lcd.setCursor(6, 2);
  lcd.print(myDate);
  lcd.setCursor(6, 3);
  lcd.print(myTime);
}

void loop () {
  switch (mode) {
    case 1:
      if (digitalRead(Button1) == LOW) {
        lcd.clear();
        lcd.setCursor(8, 0);              
        lcd.print("Mode");
        lcd.setCursor(1, 1);
        lcd.print("Monitoring Listrik");
        RTC_time();
        delay(1000);
        mode = 2;
      }
      else if (digitalRead(Button2) == LOW) {
        lcd.clear();
        lcd.setCursor(3, 0);              
        lcd.print("Mode Perbaikan");
        lcd.setCursor(4, 1);
        lcd.print("Nilai CosPhi");
        RTC_time();
        delay(1000);
        mode = 3;
      }
      break;

    case 2:
      digitalWrite(led1, HIGH);         // LED1 turn on
      digitalWrite(led2, LOW);          // LED2 turn off
      sensor_PZEM_004T_reading();
      break;

    case 3:
      digitalWrite(led1, LOW);          // LED1 turn off
      digitalWrite(led2, HIGH);         // LED2 turn on
      perbaikan_faktor_daya();
      break;
  }
}

void perbaikan_faktor_daya() {
  sensor_PZEM_004T_reading();
  float amp = pzem.current();       // Read current value
  float teg = pzem.voltage();       // Read voltage value
  float watt = pzem.power();        // Read active power value
  float PF = pzem.pf() - 0.02;      // Read power factor value
  if (PF <= 0.85 && PF >= 0.70) {   // Set power factor range, before correction using capasitor
    // Calculate Q1
    float rad1 = acos(PF);
    float Sin = sin(rad1);
    float Q1 = teg * amp * Sin;
    Serial.print("Q1 = ");
    Serial.println(Q1);

    // Calculate Arccos2
    float cosphi2 = 0.85;
    float rad2 = acos(cosphi2);

    // Calculate Q
    float Tan = tan(rad2);
    float Q = watt * Tan;
    Serial.print("Q = ");
    Serial.println(Q);

    // Calculate Q2
    float Q2 = Q1 - Q;
    Serial.print("Q2 = ");
    Serial.println(Q2);

    // Calculate Z
    float Z = pow(teg, 2) / Q2;
    Serial.print("Z = ");
    Serial.println(Z);

    // Get capasitor value
    float c = (1 / (2 * 3.14 * 50 * Z)) * 1000000;
    Serial.print("C: ");
    Serial.println(c);
    delay(1000);

    // Switch relay for input capasitor according to the calculated value
    // This prototype using capasitor 1.2uF, 2uF, 4uF, 8uF, 16uF
    if (c > 0 && c <= 1.2) {             // For capasitor value from 0 until 2 micro farad
      digitalWrite(relay1, relayOFF);    // Relay 1 OFF   (2uF)
      digitalWrite(relay2, relayOFF);    // Relay 2 OFF   (4uF)
      digitalWrite(relay3, relayOFF);    // Relay 3 OFF   (8uF)
      digitalWrite(relay4, relayOFF);    // Relay 4 OFF   (16uF)
      digitalWrite(relay5, relayON);     // Relay 5 ON    (1.2uF)
      digitalWrite(relay6, relayON);     // Relay 6 ON    (N)
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("C: ");
      lcd.print(c);
      delay(2000);
    }

    else if (c > 1.2 && c <= 2) {        // For capasitor value from 2 until 4 micro farad
      digitalWrite(relay1, relayON);     // Relay 1 ON    (2uF)
      digitalWrite(relay2, relayOFF);    // Relay 2 OFF   (4uF)
      digitalWrite(relay3, relayOFF);    // Relay 3 OFF   (8uF)
      digitalWrite(relay4, relayOFF);    // Relay 4 OFF   (16uF)
      digitalWrite(relay5, relayOFF);    // Relay 5 OFF   (1.2uF)
      digitalWrite(relay6, relayON);     // Relay 6 ON    (N)
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("C: ");
      lcd.print(c);
      delay(2000);
    }

    else if (c > 2 && c <= 4) {          // For capasitor value from 2 until 4 micro farad
      digitalWrite(relay1, relayOFF);    // Relay 1 OFF   (2uF)
      digitalWrite(relay2, relayON);     // Relay 2 ON    (4uF)
      digitalWrite(relay3, relayOFF);    // Relay 3 OFF   (8uF)
      digitalWrite(relay4, relayOFF);    // Relay 4 OFF   (16uF)
      digitalWrite(relay5, relayOFF);    // Relay 5 OFF   (1.2uF)
      digitalWrite(relay6, relayON);     // Relay 6 ON    (N)
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("C: ");
      lcd.print(c);
      delay(2000);
    }

    else if (c > 4 && c <= 6) {          // For capasitor value from 4 until 6 micro farad
      digitalWrite(relay1, relayON);     // Relay 1 ON    (2uF)
      digitalWrite(relay2, relayON);     // Relay 2 ON    (4uF)
      digitalWrite(relay3, relayOFF);    // Relay 3 OFF   (8uF)
      digitalWrite(relay4, relayOFF);    // Relay 4 OFF   (16uF)
      digitalWrite(relay5, relayOFF);    // Relay 5 OFF   (1.2uF)
      digitalWrite(relay6, relayON);     // Relay 6 ON    (N)
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("C: ");
      lcd.print(c);
      delay(2000);
    }

    else if (c > 6 && c <= 8) {          // For capasitor value from 6 until 8 micro farad
      digitalWrite(relay1, relayOFF);    // Relay 1 OFF   (2uF)
      digitalWrite(relay2, relayOFF);    // Relay 2 OFF   (4uF)
      digitalWrite(relay3, relayON);     // Relay 3 ON    (8uF)
      digitalWrite(relay4, relayOFF);    // Relay 4 OFF   (16uF)
      digitalWrite(relay5, relayOFF);    // Relay 5 OFF   (1.2uF)
      digitalWrite(relay6, relayON);     // Relay 6 ON    (N)
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("C: ");
      lcd.print(c);
      delay(2000);
    }

    else if (c > 8 && c <= 10) {         // For capasitor value from 8 until 10 micro farad
      digitalWrite(relay1, relayON);     // Relay 1 ON    (2uF)
      digitalWrite(relay2, relayOFF);    // Relay 2 OFF   (4uF)
      digitalWrite(relay3, relayON);     // Relay 3 ON    (8uF)
      digitalWrite(relay4, relayOFF);    // Relay 4 OFF   (16uF)
      digitalWrite(relay5, relayOFF);    // Relay 5 OFF   (1.2uF)
      digitalWrite(relay6, relayON);     // Relay 6 ON    (N)
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("C: ");
      lcd.print(c);
      delay(2000);
    }

    else if (c > 10 && c <= 12) {        // For capasitor value from 10 until 12 micro farad
      digitalWrite(relay1, relayOFF);    // Relay 1 OFF   (2uF)
      digitalWrite(relay2, relayON);     // Relay 2 ON    (4uF)
      digitalWrite(relay3, relayON);     // Relay 3 ON    (8uF)
      digitalWrite(relay4, relayOFF);    // Relay 4 OFF   (16uF)
      digitalWrite(relay5, relayOFF);    // Relay 5 OFF   (1.2uF)
      digitalWrite(relay6, relayON);     // Relay 6 ON    (N)
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("C: ");
      lcd.print(c);
      delay(2000);
    }

    else if (c > 12 && c <= 14) {        // For capasitor value from 12 until 14 micro farad
      digitalWrite(relay1, relayON);     // Relay 1 ON    (2uF)
      digitalWrite(relay2, relayON);     // Relay 2 ON    (4uF)
      digitalWrite(relay3, relayON);     // Relay 3 ON    (8uF)
      digitalWrite(relay4, relayOFF);    // Relay 4 OFF   (16uF)
      digitalWrite(relay5, relayOFF);    // Relay 5 OFF   (1.2uF)
      digitalWrite(relay6, relayON);     // Relay 6 ON    (N)
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("C: ");
      lcd.print(c);
      delay(2000);
    }

    else if (c > 14 && c <= 16) {        // For capasitor value from 14 until 16 micro farad
      digitalWrite(relay1, relayOFF);    // Relay 1 OFF   (2uF)
      digitalWrite(relay2, relayOFF);    // Relay 2 OFF   (4uF)
      digitalWrite(relay3, relayOFF);    // Relay 3 OFF   (8uF)
      digitalWrite(relay4, relayON);     // Relay 4 ON    (16uF)
      digitalWrite(relay5, relayOFF);    // Relay 5 OFF   (1.2uF)
      digitalWrite(relay6, relayON);     // Relay 6 ON    (N)
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("C: ");
      lcd.print(c);
      delay(2000);
    }

    else if (c > 16 && c <= 18) {        // For capasitor value from 16 until 18 micro farad
      digitalWrite(relay1, relayON);     // Relay 1 ON    (2uF)
      digitalWrite(relay2, relayOFF);    // Relay 2 OFF   (4uF)
      digitalWrite(relay3, relayOFF);    // Relay 3 OFF   (8uF)
      digitalWrite(relay4, relayON);     // Relay 4 ON    (16uF)
      digitalWrite(relay5, relayOFF);    // Relay 5 OFF   (1.2uF)
      digitalWrite(relay6, relayON);     // Relay 6 ON    (N)
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("C: ");
      lcd.print(c);
      delay(2000);
    }

    else if (c > 18 && c <= 20) {        // For capasitor value from 18 until 20 micro farad
      digitalWrite(relay1, relayOFF);    // Relay 1 OFF   (2uF)
      digitalWrite(relay2, relayON);     // Relay 2 ON    (4uF)
      digitalWrite(relay3, relayOFF);    // Relay 3 OFF   (8uF)
      digitalWrite(relay4, relayON);     // Relay 4 ON    (16uF)
      digitalWrite(relay5, relayOFF);    // Relay 5 OFF   (1.2uF)
      digitalWrite(relay6, relayON);     // Relay 6 ON    (N)
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("C: ");
      lcd.print(c);
      delay(2000);
    }

    else if (c > 20 && c <= 22) {        // For capasitor value from 20 until 24 micro farad
      digitalWrite(relay1, relayON);     // Relay 1 ON    (2uF)
      digitalWrite(relay2, relayON);     // Relay 2 ON    (4uF)
      digitalWrite(relay3, relayOFF);    // Relay 3 OFF   (8uF)
      digitalWrite(relay4, relayON);     // Relay 4 ON    (16uF)
      digitalWrite(relay5, relayOFF);    // Relay 5 OFF   (1.2uF)
      digitalWrite(relay6, relayON);     // Relay 6 ON    (N)
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("C: ");
      lcd.print(c);
      delay(2000);
    }

    else if (c > 22 && c <= 24) {        // For capasitor value from 24 until 26 micro farad
      digitalWrite(relay1, relayOFF);    // Relay 1 OFF   (2uF)
      digitalWrite(relay2, relayOFF);    // Relay 2 OFF   (4uF)
      digitalWrite(relay3, relayON);     // Relay 3 ON    (8uF)
      digitalWrite(relay4, relayON);     // Relay 4 ON    (16uF)
      digitalWrite(relay5, relayOFF);    // Relay 5 OFF   (1.2uF)
      digitalWrite(relay6, relayON);     // Relay 6 ON    (N)
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("C: ");
      lcd.print(c);
      delay(2000);
    }

    else if (c > 24 && c <= 26) {        // For capasitor value from 24 until 26 micro farad
      digitalWrite(relay1, relayON);     // Relay 1 ON    (2uF)
      digitalWrite(relay2, relayOFF);    // Relay 2 OFF   (4uF)
      digitalWrite(relay3, relayON);     // Relay 3 ON    (8uF)
      digitalWrite(relay4, relayON);     // Relay 4 ON    (16uF)
      digitalWrite(relay5, relayOFF);    // Relay 5 OFF   (1.2uF)
      digitalWrite(relay6, relayON);     // Relay 6 ON    (N)
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("C: ");
      lcd.print(c);
      delay(2000);
    }

    else if (c > 26 && c <= 28) {        // For capasitor value from 24 until 26 micro farad
      digitalWrite(relay1, relayOFF);    // Relay 1 OFF   (2uF)
      digitalWrite(relay2, relayON);     // Relay 2 ON    (4uF)
      digitalWrite(relay3, relayON);     // Relay 3 ON    (8uF)
      digitalWrite(relay4, relayON);     // Relay 4 ON    (16uF)
      digitalWrite(relay5, relayOFF);    // Relay 5 OFF   (1.2uF)
      digitalWrite(relay6, relayON);     // Relay 6 ON    (N)
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("C: ");
      lcd.print(c);
      delay(2000);
    }

    else if (c > 28 && c <= 30) {        // For capasitor value from 24 until 26 micro farad
      digitalWrite(relay1, relayON);     // Relay 1 ON    (2uF)
      digitalWrite(relay2, relayON);     // Relay 2 ON    (4uF)
      digitalWrite(relay3, relayON);     // Relay 3 ON    (8uF)
      digitalWrite(relay4, relayON);     // Relay 4 ON    (16uF)
      digitalWrite(relay5, relayOFF);    // Relay 5 OFF   (1.2uF)
      digitalWrite(relay6, relayON);     // Relay 6 ON    (N)
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("C: ");
      lcd.print(c);
      delay(2000);
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nilai Kapasitor");
      lcd.setCursor(0, 1);
      lcd.print("Tidak Memenuhi Untuk");
      lcd.setCursor(0, 2);
      lcd.print("Perbaikan PF");
      delay(2000);
    }

  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Perbaikan cosphi");
    lcd.setCursor(0, 1);
    lcd.print("diluar range");
    delay(2000);
    sensor_PZEM_004T_reading();
  }
}
