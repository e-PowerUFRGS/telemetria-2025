
#include <mcp_can.h>
#include <SPI.h>

MCP_CAN CAN0(10); 

//MCP_CAN CAN1(9);     

void setup()
{
  
  Serial.begin(115200);

  //incicialização das CAN meu mano veio, uma em 500Kbps e outra em 250Kbps
  while(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) 
  Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   
  Serial.println("MCP2515 0 Initialized Successfully!");

  /*while(CAN1.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) != CAN_OK) 
  Serial.println("Error Initializing MCP2515...");

 CAN1.setMode(MCP_NORMAL);   
  Serial.println("MCP2515 1 Initialized Successfully!");*/
}
//aqui tu pode adicionar as mensagens que tu quer enviar. 
byte data_drive0[8] = {0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
byte data_drive1[8] = {0x00, 0x23, 0x45, 0x00, 0x01, 0x00, 0x00, 0x00};
byte data_drive2[8] = {0x65, 0x65, 0x65, 0x65, 0x00, 0x00, 0x00, 0x00};
//aqui as variaveis que armazenam o ultimo dado recebido da can, id, tamanho, e mensagem (buffer)
unsigned long rxId = 0;
unsigned char len;
unsigned char rxBuf[8];
unsigned long rxId1 = 0;
unsigned char len1;
unsigned char rxBuf1[8];
void loop()
{
// aqui é como tu usa, chama um if que verfifica se a can leu algo, e que automaticamente armazena nas varivais)
if (CAN0.readMsgBuf(&rxId, &len, rxBuf) == CAN_OK) {
  // aqui é pra checar se recebeu id especifico 
  if (rxId == 0x17D)   {
    // e aqui mensagem especifica no byte especifico 
    if (rxBuf[2] == 0x42){
    }
  }
} 
// aqui a mesma coisa só que pra can de 250Kbps                   
/*if (CAN1.readMsgBuf(&rxId1, &len1, rxBuf1) == CAN_OK) {
  // da uma olhada que aqui o valor é extended frame pra esss casos é preciso somar 8 ao primeiro valor do Id, por exemplo
  // o id certo é 18FF50E5, mas na hora de ler eu tenho que somar 8 no inicio então fica 98FF50E5
  if (rxId1 == 0x98FF50E5) {  
    if (rxBuf1[4] == 0x00) {
                   
    }
  }
}*/
// aqui é como manda as mensagens na rede can, o caso abaixo é como mandar extended frame, 
// pra envio não precisa somar o 8 lá, só colocar o id que tu quer na esquerda e deixar do jeito que ta abaixo 
// no caso o id sendo enviado é 00000601 o tamanho é 8 e a mensagme é drive0 
//CAN1.sendMsgBuf(0x00000601 | 0x80000000, 1, 8, data_drive0);
// aqui é envio do id 626 standard 8 bytes drive 1 
//CAN1.sendMsgBuf(0x626, 8, data_drive1);
// aqui mais um exemplo de extended id 1806E5F4 8 bytes drive2 
 byte sndStat = CAN0.sendMsgBuf(0x0FFF0300, 1, 8, data_drive1);
  if(sndStat == CAN_OK){
    Serial.println("Message Sent Successfully!");
  } else {
    Serial.println("Error Sending Message...");
  }
  delay(100);   // send data per 100ms


}
