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

int main(int argc, char *argv[])
{
    char readBuff[BUFFER_SIZE];
    char sendBuff[BUFFER_SIZE];
    struct sockaddr_in serverAddress, clientAddress;
    int server_fd, client_fd;
    int client_addr_size;
    ssize_t receivedBytes;
    ssize_t sentBytes;
 
    /*
    if (argc != 2)
    {
        printf("사용법 : ./filename 포트번호 \n");
        exit(0);
    }
    */
 
    socklen_t clientAddressLength =0;
 
    memset(&serverAddress, 0, sizeof(serverAddress));
    memset(&clientAddress, 0, sizeof(clientAddress));
 
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(20162);
 
 
    // 서버 소켓 생성 및 서버 주소와 bind
    
    // 서버 소켓 생성(UDP니 SOCK_DGRAM이용)
    if((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) // SOCK_DGRAM : UDP
    {
        printf("Sever : can not Open Socket\n");
        exit(0);
    }
 
    // bind 과정
    if(bind(server_fd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        printf("Server : can not bind local address");
        exit(0);
    }
    
 
    printf("Server: waiting connection request.\n");
 
    char nickname[5][10];
    int chatter = 0;
    int portArr[5] = {-1,-1,-1,-1,-1};
    int pNum = 0;
    int i = 0;
 
    while(1)
    {
        
    
    //채팅 프로그램 제작
    client_addr_size = sizeof(clientAddress);
 
    receivedBytes = recvfrom(server_fd, readBuff, BUFF_SIZE, 0, (struct sockaddr*)&clientAddress, &client_addr_size);
 
    // 닉네임을 닉네임 배열에 저장해준다.
    readBuff[receivedBytes] = '\0';
    sprintf(nickname[pNum],"%s",readBuff);
    fflush(stdout);
 
        // 닉네임 수신 할 때 100바이트를 수신한다.
        if(receivedBytes == 100)
        {
            // 채팅 유저가 없으면 100바이트라는 임시 암호를 보낸다.
            if(chatter == 0)
            {
                sentBytes = sendto(server_fd, "You can 'send Message'\n", 100, 0, (struct sockaddr*)&clientAddress, sizeof(clientAddress));
                chatter = 1;                
 
                int pt = clientAddress.sin_port;
                portArr[0] = pt;
                pNum = 0;
                printf("Login Succeed ! \n Client : %s port[%d]\n\n",nickname[pNum], pt);
                pNum++;
    
            }
    
            // 채팅 유저가 있으면 99바이트라는 임시 암호를 보낸다.
            else if(chatter == 1)
            {
                
                
                int pt = clientAddress.sin_port;
 
                for(i = 1;; i++)
                {
                    if(i <= 3 && portArr[i] == -1)
                    {
                        portArr[i] = pt;
                        printf("Login Succeed ! \n Client :: %s port :: %d\n",nickname[i], portArr[i]);
                        pNum++; // 닉네임 넘버를 위해 넣어둔다.
                        break;
                    }
                    // 채팅방이 꽉 찼으면 거절한다.
                    else if(i > 4)
                    {
                        sentBytes = sendto(server_fd, "This chatting room is full\n", 98, 0, (struct sockaddr*)&clientAddress, sizeof(clientAddress));
                        break;
                    }
                }
 
                sentBytes = sendto(server_fd, "You can't send Message 'Only hear'\n", 99, 0, (struct sockaddr*)&clientAddress, sizeof(clientAddress));
            }
        }
 
        // Chatter이 접속을 종료하면 배열 및 변수를 초기화해준다.
        else if(receivedBytes == 98)
        {
            chatter = 0;
            portArr[0] = -1;
            nickname[0][0] = '\0';
 
            readBuff[receivedBytes] = '\0';
            fputs(readBuff, stdout);
            fflush(stdout);
 
            sprintf(sendBuff,"%s",readBuff);
            
            // 다른 클라이언트에게도 종료를 알려준다.
            for(i = 1; portArr[i] != -1; i++)
            {
                clientAddress.sin_port = portArr[i];
                sentBytes = sendto(server_fd, sendBuff, strlen(sendBuff), 0, (struct sockaddr*)&clientAddress, sizeof(clientAddress));
            }
        }
        
        // 그 외에는 문자를 수신받는다.
        else
        {
            printf("%lu bytes read\n", receivedBytes);
            readBuff[receivedBytes] = '\0';
            fputs(readBuff, stdout);
            fflush(stdout);
    
            sprintf(sendBuff,"%s",readBuff);
 
            // 현재 로컬 주소인 127.0.0.1에서 행동중이니 고유 포트 번호를 변경해주며 이용한다.
            for(i = 1; portArr[i] != -1; i++)
            {
                clientAddress.sin_port = portArr[i];
                sentBytes = sendto(server_fd, sendBuff, strlen(sendBuff), 0, (struct sockaddr*)&clientAddress, sizeof(clientAddress));
            }
            
        }
    }
 
    // 서버 소켓 close 
    close(server_fd);
 
    return 0;
}
