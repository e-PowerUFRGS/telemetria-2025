
#ifndef TELEMETRIA_2025_REAL_TIME_CLOCK_H
#define TELEMETRIA_2025_REAL_TIME_CLOCK_H
#include <RTClib.h>

class Real_time_clock {
    RTC_DS1307 rtc;
    DateTime inicio;
    DateTime atual;
    unsigned long milisegundos_de_inicio;
    unsigned long milisegundos_atuais;
    bool debug = false;
    public:
    Real_time_clock();
    bool pronto();
    unsigned long tempo_que_passou(bool fim_do_loop);
    unsigned long milisegundos();
    DateTime get_inicio();
    DateTime get_atual();
};


#endif //TELEMETRIA_2025_REAL_TIME_CLOCK_H