//
// Created by Roy on 09/06/2018.
//

#ifndef EX3_FINAL_LOGGER_H
#define EX3_FINAL_LOGGER_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

void recSuccessAux();
void recFailAux();
void recWrqPacketAux(const char* filename, const char* mode);
void recDataPacketAux(uint16_t block_number, int packet_length);
void sendAckPacketAux(uint16_t block_number);
void writeBlockAux(int size);
void flowErrorAux(const char* description);
void sysCallErrorAux();




#endif //EX3_FINAL_LOGGER_H
