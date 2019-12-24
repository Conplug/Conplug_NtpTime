//
// Copyright (c) 2019 Conplug (https://conplug.com.tw)
// Author: Hartman Hsieh
//
// Description :
//   Get NTP time and set the external time provider.
//
// Connections :
//   None
//
// Required Library :
//   https://github.com/PaulStoffregen/Time
//


#include <Arduino.h>


#include "Conplug_NtpTime.h"


Conplug_NtpTime::Conplug_NtpTime()
{

}

int Conplug_NtpTime::begin()
{
  //
  // NTP function initialize
  //
  Serial.println("Starting UDP");
  Udp.begin(LocalPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  return 0; // Success
}

time_t Conplug_NtpTime::getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  for (int i = 0; i < 6; i++) { // Retry 6 times if "No NTP Response"
    while (Udp.parsePacket() > 0) ; // discard any previously received packets
    Serial.println("Transmit NTP Request");
    // get a random server from the pool
    WiFi.hostByName(NTP_SERVER_NAME, ntpServerIP);
    Serial.print(NTP_SERVER_NAME);
    Serial.print(": ");
    Serial.println(ntpServerIP);
    sendNTPpacket(ntpServerIP);
    uint32_t beginWait = millis();
    while (millis() - beginWait < 1500) {
      int size = Udp.parsePacket();
      if (size >= NTP_PACKET_SIZE) {
        Serial.println("Receive NTP Response");
        Udp.read(PacketBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
        unsigned long secsSince1900;
        // convert four bytes starting at location 40 to a long integer
        secsSince1900 =  (unsigned long)PacketBuffer[40] << 24;
        secsSince1900 |= (unsigned long)PacketBuffer[41] << 16;
        secsSince1900 |= (unsigned long)PacketBuffer[42] << 8;
        secsSince1900 |= (unsigned long)PacketBuffer[43];
        return secsSince1900 - 2208988800UL + TIME_ZONE * SECS_PER_HOUR;
      }
    }
    Serial.println("No NTP Response :-(");
    delay(500);
  }
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void Conplug_NtpTime::sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(PacketBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  PacketBuffer[0] = 0b11100011;   // LI, Version, Mode
  PacketBuffer[1] = 0;     // Stratum, or type of clock
  PacketBuffer[2] = 6;     // Polling Interval
  PacketBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  PacketBuffer[12] = 49;
  PacketBuffer[13] = 0x4E;
  PacketBuffer[14] = 49;
  PacketBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(PacketBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
