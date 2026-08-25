# README
*Este diretório contém os arquivos para o código do arduino para telemetria da E-power além de referências usadas durante seu desenvolvimento.*

# Introdução

## Referencias
[Site Arduino](https://www.arduino.cc).
[Site platformIO](https://platformio.org).
[Partes de códigos antigos usados em testes](/code_bits).
[Exemplos de RTC e cartão SD](/ex_datalogger).

## Pré requisitos

* Arduino mega 2560 (para usar outro arduino deverão haver mmodificações significativas).
* Demais placas usadas no projeto.
* [PlatformIO IDE](https://platformio.org) ou IDE com plugin do platformIO.
* Alguma instalação de [Python](https://www.python.org/downloads/).

## Organização 

Atualizar este aqruivo conforme mudanças no código forem feitas,\ ele deve servir tanto como um guia de uso quanto como um guia para mudanças futuras,\ principalmente se os desenvolvedores mudarem.

## Visão geral do código

O código usa o conceito de [classes do c++](https://en.cppreference.com/cpp/language/classes), pode ser necessário ler sobre elas.
O arquivo `main.cpp` contém as interações diretas com o hardware,\ buffer de escrita no CSV além da estrutura padrão de um código para arduino.\ Para lidar com a interrupcao de timer para o controle de erros de apps veja [ISR](#isr).
o arquivo `DEFINICOES.h` contém todas as configurações como pinagens,\ formato das linhas para o CSV e tempo. Contém também os includes para os outros arquivos e\as funções auxiliares da main.

# [main](/src/main.cpp)

* `void setup()` lida com o setup de cada função.
* `void loop()` rotina principal, lida com o buffer de escrita.
* `void timeout_error()` alcançado via [ISR](#isr), ocorre quando há falhas no acelerador.
* `void setup_timer_interrupt()` lida com o baixo nível para usar os relógios de hardware para interrupção.
* `Sring date_time_to_csv(DateTime when)` transforma os dados do RTC em uma String CSV.
* `Potenciometro* declarar_pots()` caso existam potenciometros separados do acelerador.

## ISR

O conceito de uma ISR é uma subrotina usada para lidar com algum processo critico,\no caso ela checa caso o acelerador foi declarado como em funcionamento pelo corpo do código.
```
ISR(TIMER4_COMPA_vect) {//interrompe apos o tempo indicado em Definicoes-Timer configs- Tempo
    if (acelerador_passou) {
        acelerador_passou = false;
    }else {
        timeout_error();
    }
}
```  
O tempo do relógio está definido em [src/DEFINICOES](src/DEFINICOES.h).
O timeout error tranca a execução do código até que haja uma verificação do funcionamento\ do acelerador, enquanto isso mantém o rele desligado.
```
void timeout_error() {//enquanto o acelerador estiver errado envia sinal para o rele não passar
    digitalWrite(PIN_RELE, RELE_OFF);
    while (!acelerador_passou) {
    }
    acelerador_passou = true;
    digitalWrite(PIN_RELE, RELE_ON);
}
```


# [DEFINICOES](/src/DEFINICOES.h)

Está separado por comentários indicando diferentes tipos de configuração.
>[!IMPORTANT]
> Configurações de tempo, pinagem, header e id's são propensas a erro\em caso de problemas no funcionamento verificar primeiro se estão de acordo com\a parte física do projeto
Ex:
```
//Accelerator Configs
const int POTS_ACELERADOR[2] = {A4,A5};
const Parametros_acelerador PAR_ACELERADOR = {10/1/12/2/4.5/0.1/0.5};
const int CASAS_DECIMAIS = 2;
const int REFERENCIA_DE_TENSAO = 5;
const int PIN_RELE = 10;
const int RELE_OFF = HIGH;//mudar se o rele estiver invertido
const int RELE_ON = LOW;
```

# [Acelerador](/src/Acelerador.h)

Contém as configurações e funções relacionadas ao apps, usa dois [Potenciometros](#potenciometro)\para realizar os testes, é inicializado\pelos pinos conectados, a resolução usada,\a referencia de tensão e a aproximação exponencial para as funções dos apps.
`bool is_working()` simultaneamente checa se não há um erro e coleta uma amostrr,\a amostra tem que ser acessada via `double get_valor`, que retorna\o valor aproximado entre 0 e 1 indicando o quanto o acelerador está pressionado.  

# [Acelerometro](/src/Acelerometro.h)

Contém as funções relacionadas ao acelerometro BNO055, recebe na declaração\um objeto BNO055 realiza o setup e pode ser acessado\com `void get_liner_acceleration(float *x, float *y, float *z);`\para resgatar valores de aceleração linear usando ponteiros.

# CAN_carro

>[!WARNING]
>Ainda não implementado, temos que encontrar as mensagens dos dispositivos\ou encontrar uma forma genérica que o arduino suporte.

# [Cartao_SD](/src/Cartao_SD.h)

Contém as funcionalidades do cartão SD, foi pensado para ser usado junto com\o [Dashboard da E-power](https://github.com/e-PowerUFRGS/dashboard), embora qualquer leitor de CSV\deve dar conta de ler esse arquivo.
É declarado junto do nome do arquivo, nome da pasta e pino CS, que devem ser\escolhidos dentro do arquivo [src/DEFINICOES](src/DEFINICOES.h).

```
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
```
Primeiro garante que o arquivo está aberto então se não for header,\escreve a linha normalmente,\se for header e o arquivo está vazio,\escreve o header,\caso tenha um header anterior e informações do arquivo,\não escreve o novo header.

# GPS_carro

Reestrutura um código antigo usado para testes para ser uma classe.
Contém definições do próprio GPS além de um objeto tinyGPS e funções para acesso.
`void ler_gps()` deve ser usado antes dos getters de latitude e longitude.

# Potenciometro

Classe de suporte só esconde trechos de código repetitivos para leitura,\como não existem potenciometros fora os contidos pelo acelerador,\não há declaração deles fora do acelerador.

# Real_time_clock

Implementa o RTC e lida com as funções dele usando a RTClib.
mantém controle de certos parâmetros em relação ao tempo passado,\lida com a obtenção de tempos maiores do que um segundo, visto que o arduino\mantém controle por hardware da passagem do tempo em milissegundos,\realiza os calculos necessários para isso.
