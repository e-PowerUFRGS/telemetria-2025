#include "Acelerometro.h"
#include  "DEFINICOES.h"


const int FREQUENCIA_DESEJADA = 1/TEMPO;
const long DIVISOR = 256*FREQUENCIA_DESEJADA;
const int CONTADOR = (FREQUENCIA_ARDUINO/DIVISOR) - 1;
volatile bool acelerador_passou = true;

//CAN_carro can(CHIP_SELECT_CAN);
Acelerometro acelerometro(Adafruit_BNO055 bno = Adafruit_BNO055(ACC_ID, ACC_ADRESS));
Potenciometro *potenciometros = declarar_pots();
Acelerador sensor_acelerador(POTS_ACELERADOR[0], POTS_ACELERADOR[1], RES_POTENCIOMETROS,REFERENCIA_DE_TENSAO, PAR_ACELERADOR);
Cartao_SD sd(ARQUIVO_SD,PASTA_SD,CHIP_SELECT_SD);
GPS_carro gps;
Real_time_clock rtc;


void setup() {
    cli();//desabilita interrupcoes
    Serial.begin(SERIAL_BAUD);
    sensor_acelerador.setup();
    for (int n = 0; n < NUM_POTENCIOMETROS; n++) {
        potenciometros[n].setup();
    }
    gps.setup();
    setup_timer_interrupt();
    pinMode(PIN_RELE, OUTPUT);
    sei();//habilita interrupcoes
}



void loop() {
    digitalWrite(PIN_RELE, RELE_ON);
    unsigned long tempo = rtc.tempo_que_passou(true);
    String buffer;
    if (!sensor_acelerador.is_working()) {
        timeout_error();
    }
    acelerador_passou = true;
    buffer += String(sensor_acelerador.get_valor(), CASAS_DECIMAIS);
    buffer += ",";


    for (int n = 0; n < NUM_POTENCIOMETROS; n++) {
        buffer += String(potenciometros[n].read());
        buffer += ",";
    }


    buffer += String(gps.get_latitude(), CASAS_DECIMAIS);
    buffer += ",";
    buffer += String(gps.get_longitude(), CASAS_DECIMAIS);
    buffer += ",";


    DateTime quando = rtc.get_atual();
    buffer += date_time_to_csv(quando);
    buffer += String(rtc.tempo_que_passou(false));
    buffer += "\n";

    sd.escreve_linha(buffer, false);
}



void timeout_error() {//enquanto o acelerador estiver errado envia sinal para o rele não passar
    digitalWrite(PIN_RELE, RELE_OFF);
    while (!acelerador_passou) {
    }
    acelerador_passou = true;
    digitalWrite(PIN_RELE, RELE_ON);
}

void setup_timer_interrupt() {//Seta os registradores para permitir interrupcoes em a nivel de hardware
    TCCR4A = 0;
    TCCR4B = 0;
    TCNT4 = 0;
    OCR4A = CONTADOR;//valor calculado a partir do tempo
    TCCR4B |= (1<< WGM12);//ativa CTC mode, faz as interrupcoes de tempo
    TCCR4B |= (1<<CS12);
    TIMSK4 |= (1<<OCIE4A);
}

ISR(TIMER4_COMPA_vect) {//interrompe apos o tempo indicado em Definicoes-Timer configs- Tempo
    if (acelerador_passou) {
        acelerador_passou = false;
    }else {
        timeout_error();
    }
}

String date_time_to_csv(DateTime when) {
    String csv = "";
    csv += String(when.year());
    csv += ",";
    csv += String(when.month());
    csv += ",";
    csv += String(when.day());
    csv += ",";
    csv += String(when.hour());
    csv += ",";
    csv += String(when.minute());
    csv += ",";
    csv += String(when.second());
    csv += ",";
    return csv;
}

Potenciometro* declarar_pots() {
    auto potenciometros= (Potenciometro *) malloc(sizeof(Potenciometro)*NUM_POTENCIOMETROS);
    for (int n = 0; n < NUM_POTENCIOMETROS; n++) {
        Potenciometro novo(PIN_POTENCIOMETROS[n], RES_POTENCIOMETROS);
        potenciometros[n] = novo;
    }
    return potenciometros;
}


