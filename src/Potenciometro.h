
#ifndef TELEMETRIA_2025_POTENCIOMETRO_H
#define TELEMETRIA_2025_POTENCIOMETRO_H
#include <Arduino.h>

class Potenciometro {
    int pin;
    int resolucao;

public:
    Potenciometro(int pin_n, int res);
    void setup();
    int read();
};


#endif //TELEMETRIA_2025_POTENCIOMETRO_H