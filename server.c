#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT	5432
#define MAX_PENDING	5
#define MAX_LINE	256

int main()
{
	struct sockaddr_in sin;
	char buf[MAX_LINE];
	char message[MAX_LINE];
	int len;
	int s, new_s;

	/* build address data structure */
	bzero((char *)&sin, sizeof(sin));	//清零
	sin.sin_family = AF_INET;	//AF_INET:使用TCP/IPv4
	sin.sin_addr.s_addr = INADDR_ANY;	//任何位址
	sin.sin_port = htons(SERVER_PORT);	//host to network shot int:轉換成網路格式

	/* setup passive open */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)	//PF_INET:IPv4
	{
		perror("simplex-talk: socket");
		exit(1);
	}
	if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0)
	{
		perror("simplex-talk: bind");
		exit(1);
	}
	listen(s, MAX_PENDING);

	/* wait for connection, then receive and print text */
	while(1)
	{
		if ((new_s = accept(s, (struct sockaddr *)&sin, &len)) < 0)
		{
			perror("simplex-talk: accept");
			exit(1);
		}

		len = recv(new_s, buf, sizeof(buf), 0);
		fputs(buf, stdout);

		strcpy(message,"Welcome! ");
		strncat(message,buf,len);
		send(new_s, message, strlen(message)+1, 0);

		printf("DONE!\n");

		close(new_s);
	}
}

