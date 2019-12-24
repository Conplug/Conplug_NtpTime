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

#ifndef __CONPLUG_NTPTIME__
#define __CONPLUG_NTPTIME__

//
// Library for Time function
//
#include <TimeLib.h>

#include <ESP8266WiFi.h>

//
// UDP header file
//
#include <WiFiUdp.h>

//
// NTP Servers:
//
static const char NTP_SERVER_NAME[] = "us.pool.ntp.org";
//static const char NTP_SERVER_NAME[] = "time.nist.gov";
//static const char NTP_SERVER_NAME[] = "time-a.timefreq.bldrdoc.gov";
//static const char NTP_SERVER_NAME[] = "time-b.timefreq.bldrdoc.gov";
//static const char NTP_SERVER_NAME[] = "time-c.timefreq.bldrdoc.gov";

//
// Default time zone is Taipei
//
const int TIME_ZONE = 8;     // Taipei Time
//const int TIME_ZONE = -5;  // Eastern Standard Time (USA)
//const int TIME_ZONE = -4;  // Eastern Daylight Time (USA)
//const int TIME_ZONE = -8;  // Pacific Standard Time (USA)
//const int TIME_ZONE = -7;  // Pacific Daylight Time (USA)

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message

class Conplug_NtpTime
{
protected:
    WiFiUDP Udp;
    unsigned int LocalPort = 8888;  // local port to listen for UDP packets
    byte PacketBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

    //
    // Function definitions
    //
    void sendNTPpacket(IPAddress &address);

public:
    Conplug_NtpTime();
    int begin();
    time_t getNtpTime();

};

#endif // __CONPLUG_NTPTIME__
