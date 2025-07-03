#include <SPI.h>
#include <SD.h>
#include "Wire.h"
#include "RTClib.h"

//TODO: melhorar a identificação dos tipos
#define TYPE_FLOAT sizeof(float)
#define TYPE_INT sizeof(int)
#define TYPE_STRING -1 

/*
Este código apresenta uma implementação de exemplo das funções desenvolvidas para facilitar a escrita de dados em cartão SD em conjunto com o RTC.
O objetivo é torná-las o mais genéricas possível, permitindo sua reutilização em diferentes contextos além deste projeto.
Coloque o cartão antes de executar o código
O nome do arquivo é definido e é criado diretamente
Para escrever em uma pasta basta escrever "<caminho>/<nomedoarquivo>" mas a pasta já deve estar criada, para criar ela basta usar
SD.mkdir
Se quisermos aumentar a velocidade, teremos que pensar em formas de escrever em buffers
A biblioteca RTClib.h deve ser baixada e instalada separadamente
*/

// Variaveis de tempo para aumentar a precisao em milissegundos
DateTime startTime;
unsigned long startMillis;

// Classe do RTC
RTC_DS1307 rtc;


File myFile;
const int chipSelect = 53;  // Pino CS no Arduino Mega
unsigned long lastWrite = 0;
const unsigned long interval = 2;  // Intervalo de tempo, estamos escrevendo a cada 50ms, deveria ser menos, mas no momento é OK

union Values{
  int i;
  float f;
  const char *s;
};

/*
  Struct para colocar o dado e o tamanho dele, pode ser simplificado futuramente removendo as informações de tamanho
*/
struct ValuesToWrite { 
  Values data;
  int type;  // sizeof(int) ou sizeof(float)
};

/*
  Função para abrir o SD e configurá-lo, retorna true caso tenha sido feito com sucesso ou false caso não tenha funcionado
*/
static bool initSD(int chipSelect);

/*
  Função para escrever os cabeçalhos quando o arquivo for recem criado
*/
void writeCSVHeaderToSD(const char* filename, const char* headers[], int numColumns);
/*
  Função que escreve uma única linha de dados no formato .csv
*/
void writeCSVLineToSD(const char* filename, ValuesToWrite* list, int vector_size);

/*
  Função para configurar o RTC e configurar as variaveis de timing 
    DateTime startTime;
    unsigned long startMillis;
*/
bool initRTC();
/*
  Retorna a timestamp atual no formato "YYYY-MM-DD HH:MM:SS.mmm"
*/
void getTime(char* timestampBuffer);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Aguarda conexão serial
  }

  while(!initSD(chipSelect)){
    Serial.println("Erro ao acessar SD!");
  }


  while (!initRTC()) {
    Serial.println("Erro ao inicializar RTC!");
  } 

  const char* headers[] = {"Dado 1", "Dado 2", "Dado 3", "Dado 4"};


  SD.mkdir("pasta");
  writeCSVHeaderToSD("pasta/teste.txt", headers, 4);
  Serial.println("SD configurado e inicializado com sucesso");



}

void loop() {
  if (millis() - lastWrite >= interval) {
    lastWrite = millis();

    char timestamp[30];
    getTime(timestamp);

    ValuesToWrite dados[] = {
      {.data = {.s = timestamp}, .type = TYPE_STRING},
      {.data = {.i = 42}, .type = TYPE_INT},
      {.data = {.f = 3.14}, .type = TYPE_FLOAT},
      {.data = {.i = -7}, .type = TYPE_INT}
    };

    writeCSVLineToSD("pasta/teste.txt", dados, 4);

    Serial.print("Linha registrada: ");
    Serial.println(timestamp);
  }
}

static bool initSD(int chipSelect) {
  Serial.println("Inicializando cartão SD... ");
  if (!SD.begin(chipSelect)) {
    return false;
  }
  return true;
}

void writeCSVHeaderToSD(const char* filename, const char* headers[], int numColumns) {
  File f = SD.open(filename, FILE_WRITE);  // use FILE_WRITE para adicionar caso já exista
  if (f) {
    // Verifica se o arquivo está vazio (posição == 0), importante!
    if (f.size() == 0) {
         for (int i = 0; i < numColumns; i++) {
        f.print(headers[i]);
        if (i < numColumns - 1) f.print(", ");
      }
      f.println();
    }
    f.close();
  } else {
    Serial.println("Erro ao abrir o arquivo para cabeçalho");
  }
}

void getTime(char* timestampBuffer) {
  unsigned long elapsedMillis = millis() - startMillis;
  DateTime now = startTime + TimeSpan(0, 0, 0, elapsedMillis / 1000);
  int ms_part = elapsedMillis % 1000;
  
  sprintf(timestampBuffer, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
          now.year(), now.month(), now.day(),
          now.hour(), now.minute(), now.second(),
          ms_part);
}

bool initRTC() {
  if (!rtc.begin()) {
    Serial.println("DS1307 não encontrado.");
    return false;
  }

  if (!rtc.isrunning()) {
    Serial.println("DS1307 não está rodando, ajustando...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  startTime = rtc.now();
  startMillis = millis();

  return true;
}

void writeCSVLineToSD(const char* filename, ValuesToWrite* list, int vector_size) {
  File f = SD.open(filename, FILE_WRITE);
  if (f) {
    for (int i = 0; i < vector_size; i++) {
      if (list[i].type == TYPE_INT) {
        f.print(list[i].data.i);
      } else if (list[i].type == TYPE_FLOAT) {
        f.print(list[i].data.f, 2);
      } else if (list[i].type == TYPE_STRING) {
        f.print(list[i].data.s);
      }

      if (i < vector_size - 1)
        f.print(", ");
    }

    f.println();
    f.close();
  } else {
    Serial.println("Erro ao abrir o arquivo.");
  }
}
