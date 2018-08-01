//
// Created by Roy on 26/06/2018.
//

#include "packet.h"
#include "logger.h"


int recvWRQ(char *buffer)
{
	uint16_t Opcode = 0;
	char* File_name;
	char* transmission_mode;
	int fd;
	memcpy(&Opcode, buffer, 2);//test WRQ Opcode

	if (ntohs(Opcode) != 2)//not a WRQ message
	{
		return BAD_OPCODE;//fllow error
	}

	File_name = strdup(buffer + 2);
	if (File_name == NULL)
	{
		sysCallErrorAux();
		return FAIL;
	}
	fd = open(File_name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR);//Open file

	if (fd < 0)//couldnt open file
	{
		free(File_name);
		sysCallErrorAux();
		return FAIL;
	}

	transmission_mode = strdup(buffer + 2 + strlen(File_name)+1);
	if (transmission_mode == NULL)
	{
		free(File_name);
		sysCallErrorAux();
		return FAIL;
	}

	recWrqPacketAux(File_name, transmission_mode);
	free(File_name);
	free(transmission_mode);
	return fd;
}


int recvData(int fd, char* buffer, uint16_t ackCount, int datalen)
{
	uint16_t Opcode=0;
	uint16_t block_num=0;
	char* data;
	memcpy(&Opcode, buffer, 2);
	Opcode = ntohs(Opcode);
	if (Opcode != 3)//wrong opcode
	{
		return NOT_DATA;
	}
	memcpy(&block_num, buffer+2, 2);
	block_num = ntohs(block_num);
	printf("IN:DATA, %d, %d\n", block_num, datalen);

	if (block_num != ackCount)//Wrong packet number
	{
		return BAD_PACKET;
	}
	int data_size = datalen - 4;
	data = (char*)malloc(data_size*sizeof(char));
	if (data==NULL)
	{
		perror("TTFTP_ERROR:");
		exit(-1);
	}
	data = memcpy(data, buffer + 4, data_size);
	printf("WRITING: %d\n", datalen - 4);
	if (write(fd, data, data_size*sizeof(char)) < data_size*sizeof(char))
	{
		perror("TTFTP_ERROR:");
		free(data);
		exit(1);
	}
	free(data);

	if (data_size*sizeof(char) < 512)//last packet
	{
		return LAST_PACKET;
	}
	return NORMAL_PACKET;
}


int sendAck(int socketfd, struct sockaddr_in * clientAddress,int clientAddressLen, int ackCount)
{
	ackMsg ack;
	ack.op = htons(4);
	ack.blockNum = htons(ackCount);
	if (sendto(socketfd, (void*)(&ack), sizeof(ack), 0, (struct sockaddr *)clientAddress, clientAddressLen) != sizeof(ack))
	{
		sysCallErrorAux();
		return FAIL;
	}
	sendAckPacketAux(ackCount);
	return SUCCESS;
}
