//
// Created by Jordi on 7/14/2026.
//

#ifndef TELEMETRIA_2025_CARTAO_SD_H
#define TELEMETRIA_2025_CARTAO_SD_H
#include <SPI.h>
#include <SD.h>


class Cartao_SD {
    File arquivo_arduino;
    String caminho_pasta;
    String caminho_arquivo;
    bool debug = false;
    public:
    Cartao_SD(String nome_arqivo, String nome_pasta, int chip_select);
    bool pronto();
    void escreve_linha(String linha, bool header);
};


#endif //TELEMETRIA_2025_CARTAO_SD_H