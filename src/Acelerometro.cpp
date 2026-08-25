
#include "Acelerometro.h"

Acelerometro::Acelerometro(Adafruit_BNO055 bno) {
    this->bno = bno;
}

void Acelerometro::setup() {
    while (!bno.begin()) {
        delay(100);
    }
}

void Acelerometro::get_liner_acceleration(float *x, float *y, float *z) {
    sensors_event_t linear_accel_data;
    bno.getEvent(&linear_accel_data, Adafruit_BNO055::VECTOR_LINEARACCEL);
    *x = linear_accel_data.data[0];
    *y = linear_accel_data.data[1];
    *z = linear_accel_data.data[2];
}
