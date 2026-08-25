// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <CAN_carro.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("CAN Sender");
  
  // start the CAN bus at 500 kbps
  if (!CAN_carro.begin(100E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
}

void loop() {
  // send packet: id is 11 bits, packet can contain up to 8 bytes of data
  /*Serial.print("Sending packet ... ");

  CAN.beginPacket(0x12);
  CAN.write('h');
  CAN.write('e');
  CAN.write('l');
  CAN.write('l');
  CAN.write('o');
  CAN.endPacket();

  Serial.println("done");

  delay(1000);*/

  // send extended packet: id is 29 bits, packet can contain up to 8 bytes of data
  Serial.print("Sending extended packet ... ");

  CAN_carro.beginExtendedPacket(0x2001100);
  CAN_carro.write('T');
  CAN_carro.write('E');
  CAN_carro.write('S');
  CAN_carro.write('T');
  CAN_carro.write('E');
  CAN_carro.write('!');
  CAN_carro.endPacket();

  Serial.println("done");

  delay(1000);
}