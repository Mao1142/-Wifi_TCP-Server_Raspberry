#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string>

#define MAXLINE 4096
#define port 8888
#define dataSize 8192
using namespace std;

int  listenfd, connfd;
int cmd_switch(char data[]);

int main(int argc, char** argv) {
	
	struct sockaddr_in  servaddr;
	
	int  n;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		return 0;
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
		printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
		return 0;
	}

	if (listen(listenfd, 10) == -1) {
		printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
		return 0;
	}
	while (1) {
		printf("======waiting for client's request======\n");
		if ((connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1) {
			printf("accept socket error: %s(errno: %d)", strerror(errno), errno);
			continue;
		}
		int count = 0;
		while (1) {
			char  buff[4096];
			n = recv(connfd, buff, MAXLINE, 0);
			if (n>0) 
			{
				if (buff[n - 1] == '\n') {
					buff[n - 1] = '\0';
				}
				else {
					buff[n] = '\0';
				}
				printf("recv msg from client:%d %s\n", n, buff);

				if (cmd_switch(buff) == 1) {
					break;
				}
			}
			else {
				printf("Connection closing...\n");
				break;
			}
		}
		close(connfd);
	}
	close(listenfd);
	return 0;
}

int cmd_switch(char buff[])
{
	if (buff[0] == '0' && buff[1] == 'x' && buff[4] == '\0')
	{
		buff[2] = toupper(buff[2]);
		buff[3] = toupper(buff[3]);
		string data(buff);

		//0x00
		if (data == "0x00")
		{
			printf("continue");
		}
		//0x01
		else if (data == "0x01")
		{
			const char* sendline = "OP-00000000\n";
			send(connfd, sendline, strlen(sendline), 0);
			printf("Do Some Things 0x01\n");
		}
		//0x02
		else if (data == "0x02")
		{
			char buffer[dataSize + 1];
			for (int i = 0; i < dataSize; i++)
			{
				buffer[i] = 'H';
			}
			buffer[dataSize] = '\n';
			send(connfd, buffer, sizeof(buffer), 0);
			printf("Do Some Things 0x02 \n");
		}
		//0xFF
		else if (data == "0xFF")
		{
			printf("End Connect\n");
			return 1;
		}
		else
		{
			const char* success = "Unknown Command\n";
			send(connfd, success, strlen(success), 0);
		}
	}
	else
	{
		const char* fail = "Not Command\n";
		send(connfd, fail, strlen(fail), 0);
	}
	return 0;
}


