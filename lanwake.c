#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>

int main()
{
	int i;
	unsigned char toSend[102], mac[6];
	struct sockaddr_in udpClient, udpServer;
	int broadcast = 1;

	int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);

	if (setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST,
				&broadcast, sizeof broadcast) == -1)
	{
		perror("setsockopt (SO_BROADCAST)");
		exit(EXIT_FAILURE);
	}
	udpClient.sin_family = AF_INET;
	udpClient.sin_addr.s_addr = INADDR_ANY;
	udpClient.sin_port = 0;

	bind(udpSocket, (struct sockaddr*)&udpClient, sizeof(udpClient));

	for (i = 0; i < 6; i++)
		toSend[i] = 0xFF;

	mac[0] = 0xab;
	mac[1] = 0xcd;
	mac[2] = 0xef;
	mac[3] = 0x01;
	mac[4] = 0x23;
	mac[5] = 0x45;

	for (i = 1; i <= 16; i++)
		memcpy(&toSend[i * 6], &mac, 6 * sizeof(unsigned char));

	udpServer.sin_family = AF_INET;
	udpServer.sin_addr.s_addr = inet_addr("10.89.255.255");
	udpServer.sin_port = htons(9);

	sendto(udpSocket, &toSend, sizeof(unsigned char) * 102, 0,
			(struct sockaddr*)&udpServer, sizeof(udpServer));
	return 0;
}
