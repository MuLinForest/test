#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT	5432
#define MAX_LINE	256

int main(int argc, char *argv[])
{
	FILE *fp;
	struct hostent *hp;
	struct sockaddr_in sin;
	char *host;
	char buf[MAX_LINE];
	char buf_get[MAX_LINE];
	int s;
	int len;

	if (argc==2)
	{
		host = argv[1];
	}
	else
	{
		fprintf(stderr, "usage: simplex-talk host\n");
		exit(1);
	}

	/* translate host name into peer's IP address */
	hp = gethostbyname(host);
	if (!hp)
	{
		fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
		exit(1);
	}

	/* build address data structure */
	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);	//h_addr複製到sin.sin_addr
	sin.sin_port = htons(SERVER_PORT);

	/* active open */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("simplex-talk: socket");
		exit(1);
	}
	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		perror("simplex-talk: connect");
		close(s);
		exit(1);
	}

    printf("Please Enter your name:");
    fgets(buf, sizeof(buf), stdin);

    len = strlen(buf) +1 ;
    send(s, buf, len, 0);


    while(len = recv(s, buf_get, sizeof(buf_get), 0))
    {
			printf("\n%s",buf_get);
    }

}
