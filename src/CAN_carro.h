//
// Created by Jordi on 8/4/2026.
//

#ifndef TELEMETRIA_2025_CAN_CARRO_H
#define TELEMETRIA_2025_CAN_CARRO_H
#include <SPI.h>


class CAN_carro {
    //MCP2515 mcp2515;
    char* buffer = nullptr;
    int buffer_size;
    public:
    CAN_carro(int cs_pin);
    void setup();
    void write(char* linha, int tamanho);
    String read();
};


#endif //TELEMETRIA_2025_CAN_CARRO_H