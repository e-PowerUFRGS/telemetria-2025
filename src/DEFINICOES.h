

#ifndef TELEMETRIA_2025_DEFINICOES_H
#define TELEMETRIA_2025_DEFINICOES_H

#include <Arduino.h>
#include "Acelerador.h"
#include "Cartao_SD.h"
#include "GPS_carro.h"
#include "Real_time_clock.h"
#include "CAN_carro.h"
#include "Acelerometro.h"

/*
 * Funcoes que lidam diretamente com o hardware nao estao descritas aqui, para lidar com interrupcoes veja main
 */


//Serial Monitor
const int SERIAL_BAUD = 9600;


//Potenciometer Configs
const int NUM_POTENCIOMETROS = 4;
const int RES_POTENCIOMETROS = 10;//tamb[em [e importante para o acelerador
const int PIN_POTENCIOMETROS[NUM_POTENCIOMETROS] = {A0,A1,A2,A3};


//Accelerator Configs
const int POTS_ACELERADOR[2] = {A4,A5};
const Parametros_acelerador PAR_ACELERADOR = {10/1/12/2/4.5/0.1/0.5};
const int CASAS_DECIMAIS = 2;
const int REFERENCIA_DE_TENSAO = 5;
const int PIN_RELE = 10;
const int RELE_OFF = HIGH;//mudar se o rele estiver invertido
const int RELE_ON = LOW;


//SD Card Configs
String HEADER = "ACELERADOR,POT1,POT2,POT3,POT4,LATITUDE,LONGITUDE,ANO,MES,DIA,HORA,MINUTO,SEGUNDO,MILISSEGUNDO";
String PASTA_SD = "testes-epower";
String ARQUIVO_SD = "dados";
const int CHIP_SELECT_SD = 53;


//RTC Configs
const unsigned long MAX_LOOP_TIME = 100;


//CAN configs
const int CHIP_SELECT_CAN = CHIP_SELECT_SD;


//Acelerometer configs
const int ACC_ID = 55;
const int ACC_ADRESS = 0x28;


//Timer configs
const long FREQUENCIA_ARDUINO = 16000000;//16MHz
const int TEMPO = 45;//tempo em ms, nado deve passar de 1000, definir de acordo com regras da competicao



//Funcoes Auxiliares Da Main
void timeout_error();
void setup_timer_interrupt();
void check_timeout();
Potenciometro* declarar_pots();
String date_time_to_csv(DateTime quando);


#endif //TELEMETRIA_2025_DEFINICOES_H