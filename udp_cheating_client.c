#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE  511 
#define BLOCK 255 

struct sockaddr_in servaddr;
int addrlen = sizeof(servaddr); 


void sendMessage(int s, char* buf) {
    if((sendto(s, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, addrlen)) < 0) {
        perror("sendto fail");
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    int s; 
    int nbyte;
    char buf[MAXLINE+1], buf2[MAXLINE+1];
    FILE *stream;

    
    if(argc != 4) {
        printf("usage: %s ip_address port_number filename\n", argv[0]);
        exit(0);
    }

    
    if((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket fail");
        exit(0);
    }
    
    
    memset(&servaddr, 0, addrlen); 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr(argv[1]); 
    servaddr.sin_port = htons(atoi(argv[2])); 

	if((stream = fopen(argv[3], "r")) == NULL) { 
		printf("Error");
	 	exit(1);
	}

    
	while(!feof(stream)) {
		buf[0] = '\0';
		fgets(buf, BLOCK, stream); 

		printf("Send : %s\n", buf); 
        
   		sendMessage(s, buf);
	}
	fclose(stream);

    sendMessage(s, "end of file"); 

    
	if((stream = fopen(argv[3], "r")) == NULL) {
		printf("Error not File");
		exit(1);
	}
    
	while(!feof(stream))
	{
		buf2[0] = '\0'; 
		fgets(buf2, BLOCK, stream); 
		puts("get Server : waiting request.");
        sendMessage(s, buf);
		if((nbyte = recvfrom(s, buf, MAXLINE, 0, 
				(struct sockaddr *)&servaddr, &addrlen)) < 0) {
			perror("recvfrom fail");
			exit(1);
		}
		buf[nbyte] = 0;

		if(strncmp(buf, buf2, BLOCK)) { 
			printf("Not Match buf : %s\nbuf2 : %s", buf, buf2);
			fclose(stream);
			exit(0);
		} else {
			printf("Match buf : %s\nbuf2 : %s", buf, buf2);
		}
		
		puts("sendto complete");
	}
    
	fclose(stream); 
    close(s); 
    return 0;
}