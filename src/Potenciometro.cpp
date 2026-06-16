
#include "Potenciometro.h"

Potenciometro::Potenciometro(int pin_n, int res): pin(pin_n), resolucao(res){}


void Potenciometro::setup() {
    pinMode(pin, INPUT_PULLUP);
}

int Potenciometro::read() {
    return analogRead(pin);
}

