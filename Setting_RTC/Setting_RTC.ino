#include "DS3231_Simple.h"
DS3231_Simple Clock;

void setup() {
  Serial.begin(9600);
  Clock.begin();
}

void loop() { 
  DateTime waktu;
  waktu.Day    = 27;       //atur tanggal
  waktu.Month  = 07;      //atur bulan
  waktu.Year   = 22;      //atur tahun
  waktu.Hour   = 18;      //atur jam
  waktu.Minute = 37;      //atur menit
  waktu.Second = 00;      //atur detik
  Clock.write(waktu);
  
  Serial.print("Waktu telah di atur ke: ");
  Clock.printTo(Serial);
  Serial.println();
  
  Serial.print("Program berakhir (RESET untuk menjalakan lagi)");
  while(1);
}
