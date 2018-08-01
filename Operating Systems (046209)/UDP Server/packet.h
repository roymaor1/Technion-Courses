//
// Created by Roy on 26/06/2018.
//

#ifndef EX3_FINAL_PACKET_H
#define EX3_FINAL_PACKET_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>


#define SUCCESS 0
#define FAIL -1


#define TRANSMISSION_SIZE 516

//Packet Types
#define REG_PACKET 0
#define LAST_PACKET 1
#define BAD_PACKET 2

//Errors
#define NOT_DATA 3
#define BAD_OPCODE 4


typedef struct ackMsg{
	uint16_t op;
	uint16_t blockNum;
} __attribute__((packed));

// recvWRQ - receive the WRQ packet
// in: buffer - the held packet
// out: if successful, file descriptor to destination file. else, FAIL.
int recvWRQ(char *buffer);

// recvData - receive the given data, check the validity of it, and write data to file
// in: fd - file descriptor of the destination file, buffer - the held packet,
// ackCount - number of the ack to send, datalen - size of data
// out: status of the data received - REG_PACKET/LAST_PACKET/BAD_PACKET/NOT_DATA
int recvData(int fd, char* buffer, uint16_t ackCount, int datalen);


// sendAck - send an ACK message
// in: socketfd - file descriptor of the socket, clientAddress - address of the client,
// clientAddressLen - length of client address, ackCount - number of the ack to send
// int: SUCCESS if sent, FAIL if failed
int sendAck(int socketfd, struct sockaddr_in * clientAddress, int clientAddressLen, uint16_t ackCount);


#endif //EX3_FINAL_PACKET_H
