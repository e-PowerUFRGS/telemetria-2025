

#ifndef TELEMETRIA_2025_ACELEROMETRO_H
#define TELEMETRIA_2025_ACELEROMETRO_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>



class Acelerometro {
    Adafruit_BNO055 bno;
    public:
    Acelerometro(Adafruit_BNO055 bno);
    void setup();
    void get_liner_acceleration(float *x, float *y, float *z);
};


#endif //TELEMETRIA_2025_ACELEROMETRO_H