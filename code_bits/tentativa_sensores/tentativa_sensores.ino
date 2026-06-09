#include <mcp_can.h>
#include <SPI.h>
#include <TinyGPS.h>
#include <HX711.h>
#include "BNO055_support.h"
#include <Wire.h>

//definicoes para a can/ft
#define ACC_ID
#define BREAK_ID


//definicoes para o gps
#define GPS_SERIAL Serial1
#define GPS_BAUD 9600

//definicoes para os hx711
#define INNIT_WARP_OUT A8
#define INNIT_WARP_CLK A9
#define WF1 //fator para calibrar o sensor
#define WF2 //fator para calibrar o sensor
#define WF3 //fator para calibrar o sensor
#define WF4 //fator para calibrar o sensor

//definicoes para os potenciometros
#define BP1 //valor do potenciometro com o  carro em descanso
#define BP2 //valor do potenciometro com o  carro em descanso
#define BP3 //valor do potenciometro com o  carro em descanso
#define BP4 //valor do potenciometro com o  carro em descanso
#define BP5 //valor do potenciometro com o  carro em descanso
#define BP6 //valor do potenciometro com o  carro em descanso

//************************** funcoes ******************************** 

//coleta os valores medidos nos potenciometros e escreve em um vetor
void get_poten();

//ESCREVER AO TESTAR, converte o valor encontrado em get_poten para valores em graus
void poten_to_deg();

//coleta os dados de latitude e longitude do gps
void gps_data();

//coleta os dados de deformação dos hx711, ***tenho que calibrar ao testar
void warp_data();

//função geral, reccebe um valor inteiro e uma id de mensagem e manda para a ft pela can
void send_ft(int number, int id);

//****************************************** dados *****************************************

//tipos de dados que guardam parametros dos sensores
TinyGPS gps;
HX711 warp[4];
struct bno055_t bno;
MCP_CAN CAN0(10);

//dados que guardam valores para calibrar os sensores
float warp_factor[4]/* = {WF1, WF2, WF3, WF4}*/;
int base_pot_val[6]/* = {BP1, BP2, BP3, BP4, BP5, BP6}*/;

//********************************** codigo **********************


void setup() {
  
  Serial.begin(115200);

  GPS_SERIAL.begin(GPS_BAUD);
  //inicia comunicação I2C para o bno
  Wire.begin();
  BNO_Init(&bno);

  for(int i = A0; i < A6; i++){
    pinMode(i, INPUT);
  }
  int warp_starter = INNIT_WARP_OUT;
  for(int i = 0; i < 4; i++){
    warp[i].begin(warp_starter, warp_starter + 1);
    warp_starter += 2;
  }
  while(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) 
  Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   
  Serial.println("MCP2515 0 Initialized Successfully!");
}

//guardam os valores coletados pelos sensores em um loop
float latitude, longitude;
int pot_val[6];
float warp_val[4];
struct bno055_accel bno_linear_acc;


void loop() {
  // put your main code here, to run repeatedly:
  potenciometros();
  gps_data();
  warp_data();
  bno055_read_accel_xyz(bno_linear_acc);
}


void potenciometros(){
  int pin = A0;
  for(int i = 0; i < 6; i++){
    pot_val[i] = analogRead(pin);
    pot_val[i] = pot_val[i] - base_pot_val[i];
    pin++;
  }
}

void gps_data(){
  while(GPS_SERIAL.available()){
    if(gps.encode(GPS_SERIAL.read())){
      unsigned long age;
      gps.f_get_position(&latitude, &longitude, &age);
    }
  }
}

void warp_data(){
  for(int i = 0; i < 4; i++){
    warp[i].set_scale(warp_factor[i]);
    warp_val[i] = warp[i].read();
  }
}

void send_ft(int number, int id){
  byte data_drive[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  data_drive[7] = (number >> 24) & 0xFF;
  data_drive[6] = (number >> 16) & 0xFF;
  data_drive[5] = (number >> 20) & 0xFF;
  data_drive[4] = number & 0xFF;
  byte sndStat = CAN0.sendMsgBuf(id | 0x80000000, 1, 8, data_drive);
  if(sndStat == CAN_OK){
    Serial.println("Message Sent Successfully!");
  } else {
    Serial.println("Error Sending Message...");
  }
}

