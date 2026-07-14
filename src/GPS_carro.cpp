
#include "GPS_carro.h"

GPS_carro::GPS_carro(float *lat, float *lon): latitude(lat), longitude(lon) {
}

void GPS_carro::setup() {
    GPS_SERIAL.begin(GPS_BAUD);
}

void GPS_carro::ler_gps() {
    valid = false;
    while (GPS_SERIAL.available()) {
        if(gps.encode(GPS_SERIAL.read())){
            unsigned long age;
            gps.f_get_position(latitude, longitude, &age);
            break;
        }
    }
    valid = true;
}

float GPS_carro::get_latitude() {
    if(!valid) {
        return 0;
    }
    return *latitude;
}

float GPS_carro::get_longitude() {
    if(!valid) {
        return 0;
    }
    return *longitude;
}
