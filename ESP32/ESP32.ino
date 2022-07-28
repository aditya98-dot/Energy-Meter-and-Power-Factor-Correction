/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-mysql-database-php/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
-------------------------------------------------------------------------------------------------
  Created by: Aditya Pratama. email: adityapratama141198@gmail.com. July 2022
  Energi Meter and Power Factor Correction Version 1.0
  Energy Meter and Correction the power factor for 1 phase
  This is open source code. Please include my name in copies of this code
  Thankyou ...
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <Wire.h>

// Libraries for SD card
#include "FS.h"
#include "SD.h"
#include "SPI.h"

// Library for RTC DS3231
#include "RTClib.h"

#define SD_CS 5
RTC_DS3231 rtc;
File myFile;
const int LED_RED = 13, LED_GREEN = 12;
int buz = 4;                                  // initialize the buzzer pin 15
String dataIn, dt[8];                         // initialize variable dataIn and dt[] as a String Type
int i;
bool parsing = false;
float V, I, S, PF, P, R, F, E;

// Initiation for date and time
int Year;
byte Month, Day, Hour, Minute, Second;

// Initiation for logSDCard
String myDate, myTime, monitoring, dataMessage;

// Replace with your network credentials
const char* ssid = "Masterpiece";
const char* password = "handstand";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName  = "https://power-meter.smart-monitoring.my.id/db-listrik.php"; //Hostname

// Keep this API Key value to be compatible with the PHP code provided in the project page.
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key
String apiKeyValue = "nsdsuibs7xx278";

const char* rootCACertificate = \
  "-----BEGIN CERTIFICATE-----\n" \
  "MIIFdjCCBF6gAwIBAgISBEnqBGMfvQUfrlSS/iCf9fkrMA0GCSqGSIb3DQEBCwUA\n" \
  "MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD\n" \
  "EwJSMzAeFw0yMjA1MTcwNzEyMDdaFw0yMjA4MTUwNzEyMDZaMDExLzAtBgNVBAMT\n" \
  "Jnd3dy5wb3dlci1tZXRlci5zbWFydC1tb25pdG9yaW5nLm15LmlkMIIBIjANBgkq\n" \
  "hkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAxECyB/BOyNXCQPPQ4qcBl28u4ZGBFt/1\n" \
  "dfmMZuldc53mO0mNBbtd2HtIqOhXlDSpSUuXDrub3hJkVGvlWIu++JhXRxc4u45k\n" \
  "DMsGz+vpmRkfJRRCn1Mbgeemh/svgqhI0fhB/z0BqfNn1phG6ihhrHBquTNsy+3Z\n" \
  "wvJy0mFCa3erILEpNvnz9x/8ht+YAgHmDq0V6SuKW5EnszAw5JPF08b5s46MSP+Z\n" \
  "wvk25exBC8ZfO9+5jtdR/lELiE51I8ooqPgegfBwAdLucYJNH6xP+lOvvqBJ6ain\n" \
  "LJ6+yOsN/UgC23qwpA1TpZvEwDrHNb9qzWBcSUx3aUDo3+oFafuadQIDAQABo4IC\n" \
  "hTCCAoEwDgYDVR0PAQH/BAQDAgWgMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEF\n" \
  "BQcDAjAMBgNVHRMBAf8EAjAAMB0GA1UdDgQWBBQ3tVV9YLe1Yz3gylyFJm9BkrQm\n" \
  "vzAfBgNVHSMEGDAWgBQULrMXt1hWy65QCUDmH6+dixTCxjBVBggrBgEFBQcBAQRJ\n" \
  "MEcwIQYIKwYBBQUHMAGGFWh0dHA6Ly9yMy5vLmxlbmNyLm9yZzAiBggrBgEFBQcw\n" \
  "AoYWaHR0cDovL3IzLmkubGVuY3Iub3JnLzBVBgNVHREETjBMgiJwb3dlci1tZXRl\n" \
  "ci5zbWFydC1tb25pdG9yaW5nLm15LmlkgiZ3d3cucG93ZXItbWV0ZXIuc21hcnQt\n" \
  "bW9uaXRvcmluZy5teS5pZDBMBgNVHSAERTBDMAgGBmeBDAECATA3BgsrBgEEAYLf\n" \
  "EwEBATAoMCYGCCsGAQUFBwIBFhpodHRwOi8vY3BzLmxldHNlbmNyeXB0Lm9yZzCC\n" \
  "AQQGCisGAQQB1nkCBAIEgfUEgfIA8AB2AN+lXqtogk8fbK3uuF9OPlrqzaISpGpe\n" \
  "jjsSwCBEXCpzAAABgNETCVAAAAQDAEcwRQIgNematEjHzUNsZjXaRBNbF1Mb51x2\n" \
  "8OntQ4RFFAF69RECIQCyhCDYXD8dGSMW5p8XkcYgi3/08qOekNUGnEqvQIIQbAB2\n" \
  "AEalVet1+pEgMLWiiWn0830RLEF0vv1JuIWr8vxw/m1HAAABgNETCXMAAAQDAEcw\n" \
  "RQIhAJfbKW1ItOU7aNQOheq9Z9NVOcQ+SKLZTp/mT7xru7ONAiBUXAbBB8RlyoNQ\n" \
  "pAxLNYIG8dJRtdd8yObCNJ9GT/22fDANBgkqhkiG9w0BAQsFAAOCAQEAgaKR2rwM\n" \
  "d9GTJioXehDA309NaW4m1k16Fm2h+Lek2N9RsFLqCkmNeOnxJ4UpD2TgvY/DqGqv\n" \
  "I9CbDbOuGqGqTuklcpXFg+sKbge5jUnjdOXabnU4nWZmMP3WKayuvYK6QCRwyOQM\n" \
  "aHnf+zR0m8k83rQWwcnIfF9KSjnVAOovtzr/XLNFX+7pDO9Ljv2i6g300J6rDIHJ\n" \
  "hR55YKali6u1DZsoaEIopU7XvF48Bfbd0J/qtrGjN53RCwiiON+IXw2tG+r+rwD7\n" \
  "Z2/g+GxjY/bAs/fYBWAK3fFhqOW5QqX5QrloH6PI1PjYg9PbKe+w3nKXq4SJC+lF\n" \
  "EBcFT/2eLLzBSQ==\n" \
  "-----END CERTIFICATE-----\n";

// Not sure if WiFiClientSecure checks the validity date of the certificate.
// Setting clock just to be sure...
void setClock() {
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print(F("Waiting for NTP time sync: "));
  time_t nowSecs = time(nullptr);
  while (nowSecs < 8 * 3600 * 2) {
    delay(500);
    Serial.print(F("."));
    yield();
    nowSecs = time(nullptr);
  }
  Serial.println();
  struct tm timeinfo;
  gmtime_r(&nowSecs, &timeinfo);
  Serial.print(F("Current time: "));
  Serial.print(asctime(&timeinfo));
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  // Clear value variable dataIn for parsing data
  dataIn = "";
  pinMode (buz, OUTPUT);            // Create variable buz as a OUTPUT
  pinMode (LED_RED, OUTPUT);        // Create variable LED_MERAH as a OUTPUT
  pinMode (LED_GREEN, OUTPUT);      // Create variable LED_HIJAU as a OUTPUT

  // Checking WiFi connection
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(buz, HIGH);
    delay(500);
    digitalWrite(buz, LOW);
    delay(500);
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  setClock();

  // Initialize RTC DS3231
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
  // uncomment for set RTC time first
  //rtc.adjust(DateTime(2022, 2, 15, 14, 49, 0));

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2022, 2, 12, 7, 21, 0));
  }

  // Initialize SD card
  SD.begin(SD_CS);
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed!");
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  } Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("ERROR - SD card initialization failed!");
    return;    // init failed
  }
  // If the backup.txt file doesn't exist
  // Create a file on the SD card and write the data labels
  File file = SD.open("/backup.txt");
  if (!file) {
    Serial.println("File doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/backup.txt", "DD/MM/YY, Hr:Min:Sec, Voltage, Current, Apparent Power, Power Factor, Active Power, Reaktif Power, Frekuensi, Energi \r\n");
    delay(500);
  }
  else {
    Serial.println("File already exists");
  }
  file.close();
}

void loop() {
// parsing data
    while (Serial2.available() > 0) {
      char inChar = (char)Serial2.read();
      dataIn += inChar;
      if (inChar == '\n') {
        parsing = true;
      }
    }

    if (parsing) {
      parsingData();
      parsing = false;
      dataIn = "";
    }
}

void parsingData() {
  int j = 0;

  // Send the previously received data and then display it on the serial monitor
  Serial.print("data masuk : ");
  Serial.println(dataIn);
  dt[j] = "";              //initialize the variable, (reset variable contents)

  //Data parsing process
  for (i = 1; i < dataIn.length(); i++) {
    //Checking each character with characters (#) and (,)
    if ((dataIn[i] == '#') || (dataIn[i] == ','))
    {
      //increment variable j, used to change the index of the container array
      j++;
      dt[j] = "";                   //inisialisasi variabel array dt[j]
    }
    else {
      dt[j] = dt[j] + dataIn[i];    // The process of storing data when character checking is complete 
    }
  }

  // Store data into variable
  V += dt[0].toInt() / 100.0;
  I += dt[1].toInt() / 100.0;
  S += dt[2].toInt() / 100.0;
  PF += dt[3].toInt() / 100.0;
  P += dt[4].toInt() / 100.0;
  R += dt[5].toInt() / 100.0;
  F += dt[6].toInt() / 100.0;
  E += dt[7].toInt() / 100.0;
  delay(500);
  sendData_to_server();           // Call fungtion for sending data into database
}

void sendData_to_server() {
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure *client = new WiFiClientSecure;
    if (client) {
      client -> setCACert(rootCACertificate);
      {
        HTTPClient https;

        // Your Domain name with URL path or IP address with path
        https.begin(serverName);

        // Specify content-type header
        https.addHeader("Content-Type", "application/x-www-form-urlencoded");

        // Prepare your HTTP POST request data
        String httpRequestData = "api_key=" + apiKeyValue + "&Tegangan=" + String(V) + "&Arus=" + String(I)
                                 + "&Daya_semu=" + String(S) + "&Faktor_daya=" + String(PF) + "&Daya_aktif=" + String(P)
                                 + "&Daya_reaktif=" + String(R) + "&Frekuensi=" + String(F) + "&Energi=" + String(E) + "";
        Serial.print("httpRequestData: ");
        Serial.println(httpRequestData);

        // You can comment the httpRequestData variable above
        // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
        //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";// Send HTTP POST request
        int httpResponseCode = https.POST(httpRequestData);

        // If you need an HTTP request with a content type: text/plain
        // http.addHeader("Content-Type", "text/plain");
        // int httpResponseCode = http.POST("Hello, World!");

        // If you need an HTTP request with a content type: application/json, use the following:
        // http.addHeader("Content-Type", "application/json");
        // int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");

        if (httpResponseCode > 0) {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          Serial.print("\n");
          // Green LED on
          digitalWrite(LED_GREEN, HIGH);
          delay(200);
          digitalWrite(LED_GREEN, LOW);
          delay(200);
          digitalWrite(LED_GREEN, HIGH);
          delay(200);
          digitalWrite(LED_GREEN, LOW);
          delay(200);
        }
        else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
          Serial.print("\n");
          //Red LED on
          digitalWrite(LED_RED, HIGH);
          delay(200);
          digitalWrite(LED_RED, LOW);
          delay(200);
          digitalWrite(LED_RED, HIGH);
          delay(200);
          digitalWrite(LED_RED, LOW);
          delay(200);
        }
        // Free the resources
        https.end();
      }
    } else {
      Serial.println("Unable to create client");
    }
  } else if (WiFi.status() != WL_CONNECTED) {
    // if you didn't get a connection to the server:
    Serial.println("Connection failed!, Please check the network previously");
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    
    digitalWrite(buz, HIGH);
    digitalWrite(LED_RED, HIGH);
    delay(500);
    digitalWrite(buz, LOW);
    digitalWrite(LED_RED, LOW);
    delay(500);
    digitalWrite(buz, HIGH);
    digitalWrite(LED_RED, HIGH);
    delay(500);
    digitalWrite(buz, LOW);
    digitalWrite(LED_RED, LOW);
    delay(500);
    Serial.println ("Send data to mikro SD card");
    DateTime waktu = rtc.now() + TimeSpan(0, 0, 0, 0);

    // Separate data between date and time
    // Store data to date variable
    Year = waktu.year();
    Month = waktu.month();
    Day = waktu.day();
    myDate = String(Day) + "/" + String(Month) + "/" + String(Year) + ", ";
    // Store data to time2 variable
    Hour = waktu.hour();
    Minute = waktu.minute();
    Second = waktu.second();
    myTime = String(Hour) + ":" + String(Minute) + ":" + String(Second) + ", ";

    // Store data to monitoring variable
    monitoring = String(V) + " V, " + String(I) + " A, " + String(S) + " VA, " + String(PF) + ", " + String(P) + " W, " + String(R) + " Var, " + String(F) + " Hz, " + String(E) + " kWh";
    logSDCard();
  } else {
    Serial.println("");
    Serial.println("WiFi connected again!");
    setClock();
  }
  setData();        // After storing data into server dan database, we have empty the variable
}

// Set data as a 0 value
void setData() {
  V = 0;
  I = 0;
  S = 0;
  PF = 0;
  P = 0;
  R = 0;
  F = 0;
  E = 0;
}

// Write data into SD card
void logSDCard() {
  dataMessage = myDate + myTime + monitoring + "\r\n";
  Serial.print("Save data: ");
  Serial.println(dataMessage);
  appendFile(SD, "/backup.txt", dataMessage.c_str());
}

// Write to the SD card (DON'T MODIFY THIS FUNCTION)
void writeFile(fs::FS & fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
    Serial.print("\n");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// Append data to the SD card (DON'T MODIFY THIS FUNCTION)
void appendFile(fs::FS & fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
    Serial.print("\n");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}
