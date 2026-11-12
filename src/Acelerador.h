
#ifndef TELEMETRIA_2025_ACELERADOR_H
#define TELEMETRIA_2025_ACELERADOR_H
#include "Potenciometro.h"

struct Parametros_acelerador {
    double a1;
    double b1;
    double a2;
    double b2;
    bool is_exponential;
    float v_limit;
    float max_error;
    float begining_of_range;
};

class Acelerador {
public:
    Acelerador(int pin_1, int pin_2,  int resolucao, int v_ref, Parametros_acelerador acelerador);
    void setup();
    bool is_working();
    double get_valor();


private:
    double expression(double a, double b, bool is_exponential, double valor);
    double to_volt(double val);
    double valor = -1;
    int resolucao;
    int referencia_V;
    Parametros_acelerador acelerador;
    Potenciometro potenciometro_1;
    Potenciometro potenciometro_2;
};


#endif //TELEMETRIA_2025_ACELERADOR_H