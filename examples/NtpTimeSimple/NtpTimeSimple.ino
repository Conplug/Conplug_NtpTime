//
// Copyright (c) 2019 Conplug (https://conplug.com.tw)
// Author: Hartman Hsieh
//
// Description :
//   None
//
// Connections :
//
// Required Library :
//   https://github.com/PaulStoffregen/Time
//

#include <ESP8266WiFi.h>
#include "Conplug_NtpTime.h"

#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK  "your-password"
#endif

//
// NTP time sync, default time zone is Taipei time.
//
Conplug_NtpTime NtpTime;

//
// NTP time provider function
//
time_t getNtpTime()
{
  return NtpTime.getNtpTime();
}

void setup() {
  //
  // Initialize serial
  //
  Serial.begin(9600);

  //
  // Initialize WIFI function
  //
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  //
  // Initialize NTP time object
  //
  NtpTime.begin();
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime); // set the external time provider
  setSyncInterval(36000); // set the number of seconds between re-sync 
}

void loop() {
  
  Serial.print(year());
  Serial.print("-");   
  Serial.print(month());
  Serial.print("-");   
  Serial.print(day());

  Serial.print(" ");
  
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");    
  Serial.print(second());

  Serial.println();
  
  delay(1000);
}
