//
// Created by Jordi on 7/14/2026.
//

#include "Cartao_SD.h"

Cartao_SD::Cartao_SD(String nome_arqivo, String nome_pasta, int chip_select): caminho_arquivo(nome_arqivo), caminho_pasta(nome_pasta) {
    if (!SD.begin(chip_select)) {
        Serial.println("SD falhou");
        return;
    }

    SD.mkdir(caminho_pasta);
    debug = true;
}

bool Cartao_SD::pronto() {
    return debug;
}

void Cartao_SD::escreve_linha(String linha, bool header) {
    String caminho_aux = caminho_pasta + "/" + caminho_arquivo;
    File arquivo = SD.open(caminho_aux.c_str(), FILE_WRITE);
    if (!arquivo) {
        Serial.println("Erro no arquivo");
        return;
    }
    if (!header) {
        arquivo.println(linha);
        return;
    }
    if (arquivo.size() == 0) {
        arquivo.println(linha);
    }
}

