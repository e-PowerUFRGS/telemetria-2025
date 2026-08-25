

#include "Acelerador.h"


Acelerador::Acelerador(int pin_1, int pin_2,  int resolucao, int v_ref, Parametros_acelerador acelerador):
resolucao(resolucao),
referencia_V(v_ref),
acelerador(acelerador),
potenciometro_1(pin_1, resolucao),
potenciometro_2(pin_2, resolucao)
{

}
void Acelerador::setup() {
    potenciometro_1.setup();
    potenciometro_2.setup();
}
bool Acelerador::is_working() {
    int volt_1 = to_volt(potenciometro_1.read());
    int volt_2 = to_volt(potenciometro_2.read());
    if (volt_1 > acelerador.v_limit || volt_2 > acelerador.v_limit)
        return false;
    if (volt_1 < 0 || volt_2 < 0)
        return false;
    double real_1 = acelerador.mult1 * pow(volt_1,acelerador.exp1);
    double real_2 = acelerador.mult2 * pow(volt_2,acelerador.exp2);
    valor = real_1;
    if (real_1 < real_2) {
        double temp = real_1;
        real_1 = real_2;
        real_2 = temp;
    }
    if (real_1 < acelerador.begining_of_range)
        return true;
     if (real_1 * (1-acelerador.max_error) < real_2)
        return true;
    return false;
}

double Acelerador::get_valor() {
    return valor;
}

double Acelerador::to_volt(int val) {
    return val/resolucao * resolucao;
}

