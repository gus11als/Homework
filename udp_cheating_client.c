# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>
 
# define PORT 20162
# define BUFFER_SIZE 4096 // 서버에 이용
# define BUFF_SIZE 100 

int main(int argc, char* argv[])
{
    /*
    if(argc != 2)
    {    // argv[0]에 ./client가 들어간다.
        printf("사용법 : %s IPv4-address\n", argv[0]);
        return -1;
    }
    */
 
    int client_socket;
    struct sockaddr_in serverAddress;
    int server_addr_size;
    char sendBuff[BUFF_SIZE];
    char readBuff[BUFF_SIZE];
 
 
    ssize_t receivedBytes = 0;
    ssize_t sentBytes;
 
 
    memset(&serverAddress, 0, sizeof(serverAddress));
 
    serverAddress.sin_family = AF_INET;
    inet_aton("127.0.0.1", (struct in_addr*) &serverAddress.sin_addr.s_addr);
    serverAddress.sin_port = htons(20162);
 
    // 소켓을 생성
    if((client_socket = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("socket 생성 실패\n");
        exit(0);
    }
    
 
    // 닉네임 설정 시 닉네임과 암호키 100을 전송한다.
    printf("닉네임을 설정하세요 : ");
    char nickname[10] = {0};
    fgets(nickname,10,stdin);
    sendto(client_socket, nickname, 100, 0, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
 
    // 채팅유저인지 아닌지 설정한다.
    int canChat = 0;
    
    // 채팅가능 판별을 위해 기다린다.
    while((receivedBytes = recvfrom(client_socket, readBuff, BUFF_SIZE, 0, (struct sockaddr*)&serverAddress, &server_addr_size)) == 0){}
 
    // 채팅 가능하면 메시지 출력
    if(receivedBytes == 100)
    {
        readBuff[receivedBytes] = '\0';
        fputs(readBuff, stdout);
        fflush(stdout);
        canChat = 1;
    }
 
    // 채팅 불가능하면 메시지 출력
    else if(receivedBytes == 99)
    {
        readBuff[receivedBytes] = '\0';
        fputs(readBuff, stdout);
        fflush(stdout);
        canChat = 0;
    }
 
    // 방이 꽉찬경우
    else if(receivedBytes == 98)
    {
        readBuff[receivedBytes] = '\0';
        fputs(readBuff, stdout);
        fflush(stdout);
        return 0;
    }
 
    while(canChat == 1)
    {
        // 채팅 프로그램 제작
        server_addr_size = sizeof(serverAddress);
        
        //클라이언트에서 메세지 전송
        printf("클라이언트에서 보낼 말을 입력하세요 :: ");
    
        char msg[BUFF_SIZE];
        fgets(msg,BUFF_SIZE,stdin);
 
        // exit를 입력하면 98바이트 암호키를 통해 종료한다.
        if(strcmp(msg,"exit\n") == 0)
        {
            sentBytes = sendto(client_socket, "Chatter is Logout\n", 98, 0, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
 
            return 0;
        }
 
        sprintf(sendBuff,"%s",msg);
 
        sentBytes = sendto(client_socket, sendBuff, strlen(sendBuff), 0, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
 
        fflush(stdout);
    }
    while(canChat == 0)
    {
        receivedBytes = recvfrom(client_socket, readBuff, BUFF_SIZE, 0, (struct sockaddr*)&serverAddress, &server_addr_size);
        readBuff[receivedBytes] = '\0';
        fputs(readBuff, stdout);
        fflush(stdout);
        canChat = 0;
        
    }
    
    // 소켓을 close 
    close(client_socket);
    return 0;
}
 

