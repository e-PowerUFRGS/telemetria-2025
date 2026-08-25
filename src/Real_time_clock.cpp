//
// Created by Jordi on 7/14/2026.
//

#include "Real_time_clock.h"

Real_time_clock::Real_time_clock() {
    if (!rtc.begin()) {
        Serial.println("RTC nao encontrado");
        return;
    }

    if (!rtc.isrunning()) {
        Serial.println("RTC nao funcionando, ajustando...");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    debug = true;
    atual = inicio = rtc.now();
    milisegundos_atuais = milisegundos_de_inicio = millis();
}

DateTime Real_time_clock::get_inicio() {
    return inicio;
}

DateTime Real_time_clock::get_atual() {
    return atual;
}

unsigned long Real_time_clock::milisegundos() {
    return milisegundos_atuais;
}

unsigned long Real_time_clock::tempo_que_passou(bool fim_do_loop) {
    unsigned long milisegundos_novos = millis();
    DateTime agora = rtc.now();
    uint32_t diferenca_segundos = agora.unixtime() - atual.unixtime();
    if (fim_do_loop) {
        atual = agora;
    }
    unsigned long diferenca_milisegundos = diferenca_segundos*1000 + milisegundos_novos - milisegundos_atuais;
    milisegundos_atuais = milisegundos_novos;

    return diferenca_milisegundos;
}

