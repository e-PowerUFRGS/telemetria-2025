
#ifndef TELEMETRIA_2025_ACELERADOR_H
#define TELEMETRIA_2025_ACELERADOR_H
#include "Potenciometro.h"

union Parametros_acelerador {
    double mult1;
    double exp1;
    double mult2;
    double exp2;
    float v_limit;
    float max_error;
    float begining_of_range;
};

class Acelerador {
public:
    Acelerador(int pin_1, int pin_2,  int resolucao, int v_ref, Parametros_acelerador acelerador);
    void setup();
    bool is_working();

private:
    double to_volt(int val);
    int resolucao;
    int referencia_V;
    Parametros_acelerador acelerador;
    Potenciometro potenciometro_1;
    Potenciometro potenciometro_2;
};


#endif //TELEMETRIA_2025_ACELERADOR_H