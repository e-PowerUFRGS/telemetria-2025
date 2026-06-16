

#ifndef TELEMETRIA_2025_GPS_CARRO_H
#define TELEMETRIA_2025_GPS_CARRO_H

#include <TinyGPS.h>

#define GPS_SERIAL Serial1
#define GPS_BAUD 9600


class GPS_carro {
 TinyGPS gps;
 float latitude;
 float longitude;
 bool valid = false;
public:
 void setup();
 void ler_gps();
 float get_latitude();
 float get_longitude();
};


#endif //TELEMETRIA_2025_GPS_CARRO_H