//
// Created by Roy on 09/06/2018.
//

#include "logger.h"
#include "packet.h"

static const int WAIT_FOR_PACKET_TIMEOUT = 3;
static const int NUMBER_OF_FAILURES = 7;

int main(int argc, char *argv[])
{

	// Param Check
	if(argc != 2){
		printf("Bad Parameters");
		return FAIL;
	}

	int port = atoi(argv[1]);

	// socket() - Create socket for sending/receiving datagrams
	int sock =  socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0){
		sysCallErrorAux();
		return FAIL;
	}

	// bind() - Attach socket to the given port
	struct sockaddr_in serverAddress = {0};
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portno);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	int bind_ret = bind(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
	if (bind_ret < 0)
	{
		sysCallErrorAux();
		return FAIL;
	}


	//Program Variables
	uint16_t ackCount=0;
	int sel;
	int packet_type;
	int datalen;
	int fd;
	char buffer[TRANSMISSION_SIZE] = {0};
	bool stop = 0;

	// Client Variables
	struct sockaddr_in clientAddress = {0};
	socklen_t clientAddressLen;

	// Time Variables
	struct timeval timeout;
	timeout.tv_sec = WAIT_FOR_PACKET_TIMEOUT;
	timeout.tv_usec = 0;
	int timeoutExpiredCount = 0;

	// FD Variables
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(sock, &rfds);


	while(true)
	{

		clientAddressLen = sizeof(clientAddress);
		// recvfrom() - Get size of first message from client (supposed to be WRQ)
		datalen = recvfrom(sock, buffer, msg_len, 0, (struct sockaddr *)&clientAddress, &clientAddressLen);
		if (datalen < 0)
		{
			sysCallErrorAux();
			return FAIL;
		}
		fd = recvWRQ(buffer);
		if (fd < 0)
		{
			if (fd == BAD_OPCODE)
			{
				flowErrorAux("Did not receive an expected WRQ packet");
				recFailAux();
				return FAIL;
			}
			continue; //not WRQ
		}

		if (sendAck(sock, &clientAddress, clientAddressLen, ackCount) < 0){ close(fd); close(sock); return FAIL; }
		ackCount++;

		do
		{
			do
			{
				do
				{
					// Wait WAIT_FOR_PACKET_TIMEOUT to see if something appear for us at the socket (we are waiting for DATA)
					sel = select(sock+1, &rfds, NULL, NULL, &timeout);
					if (sel > 0) // if there was something at the socket and we are here not because of a timeout
					{
						// Read the DATA packet from the socket (at least we hope this is a DATA packet)
						datalen = recvfrom(sock, buffer, msg_len, 0, (struct sockaddr *)&clientAddress, &clientAddressLen);
						if (datalen < 0) {
							sysCallErrorAux();
							return FAIL;
						}

						packet_type = recvData(fd, buffer, ackCount, datalen);

						if (packet_type != REG_PACKET)// last packet or bad packet
							stop = 1;
						else {
							if (sendAck(sock, &clientAddress, clientAddressLen, ackCount) < 0){ close(fd); close(sock); return FAIL; }
							ackCount++;
							continue;
						}
					}
					else if (sel == 0) // Time out expired while waiting for data to appear at the socket
					{
						if (sendAck(sock, &clientAddress, clientAddressLen, ackCount-1) < 0){ close(fd); close(sock); return FAIL; }
						timeoutExpiredCount++;
					}
					if (timeoutExpiredCount >= NUMBER_OF_FAILURES)
						stop = 1;

				}while (!stop); // Continue while some socket was ready but recvfrom somehow failed to read the data

				if (packet_type == NOT_DATA) // We got something else but DATA
				{
					flowErrorAux("Expected data, received something else");
					recFailAux();
					break;
				}
				else
				{
					if (packet_type == BAD_PACKET) // The incoming block number is not what we have expected, i.e. this is a DATA pkt but the block number in DATA was wrong (not last ACKs block number + 1)
					{
						flowErrorAux("Incoming block number is not as expected");
						recFailAux();
						break; // FATAL ERROR BAIL OUT
					}
					else //Time out expired while waiting for data to appear at the socket
					{
						if (timeoutExpiredCount >= NUMBER_OF_FAILURES)
						{
							flowErrorAux("Number of failures passed the threshold. Existing Program...");
							recFailAux();
							break;
						}
					}
				}

			}while (0);


			timeoutExpiredCount = 0;
			stop = 0;

			if (packet_type == LAST_PACKET)
			{
				if (sendAck(sock, &clientAddress, clientAddressLen, ackCount) < 0){ close(fd); close(sock); return FAIL; }
				recSuccessAux();
			}

			ackCount = 0;
			close(fd);

		}while(0);
	}

}