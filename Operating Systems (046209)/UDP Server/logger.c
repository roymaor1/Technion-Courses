//
// Created by Roy on 26/06/2018.
//

#include "logger.h"

//
// Created by Roy on 09/06/2018.
//
#include "logger.h"

static const char* recSuccess = "RECVOK\n";
static const char* recFail = "RECVFAIL\n";
static const char* recWrqPacket = "IN:WRQ,%s,%s\n";
static const char* recDataPacket = "IN:DATA, %d,%d\n";
static const char* sendAckPacket = "OUT:ACK,%d\n";
static const char* writeBlock = "WRITING: %d\n";
static const char* flowError = "FLOWERROR:%s\n";
static const char* sysCallError = "TTFTP_ERROR:\n";

void recSuccessAux(){
	printf("%s",recSuccess);
}

void recFailAux(){
	printf("%s",recFail);
}

void recWrqPacketAux(const char* filename, const char* mode){
	printf(recWrqPacket,filename,mode);
}

void recDataPacketAux(uint16_t block_number, int packet_length){
	printf(recDataPacket,block_number,packet_length);
}

void sendAckPacketAux(uint16_t block_number){
	printf(sendAckPacket,block_number);
}

void writeBlockAux(int size){
	printf(writeBlock,size);
}

void flowErrorAux(const char* description){
	printf(flowError, description);
}

void sysCallErrorAux(){
	perror(sysCallError);
}