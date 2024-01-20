#include "headers.h"

void iman(char* page)
{
    const char* hostname = "man.he.net";
    char ip[100];

    struct hostent* host;
    struct in_addr** addr_list;
    if((host = gethostbyname(hostname)) == NULL)
        printf("DNS resolution failed!\n");
    addr_list = (struct in_addr**)host->h_addr_list;
    for(int i=0; addr_list[i] != NULL; i++)
    {
        strcpy(ip, inet_ntoa(*addr_list[i]));
    }

    int sock;
    struct sockaddr_in server;
    int received = 0;
    if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        printf("Failed to create socket!\n");
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(80);
    if(connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0)
        printf("Connection Failed!\n");

    char get[512];
    sprintf(get, "GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\nContent-type: text/html\r\n\r\n", page);
    if(send(sock, get, strlen(get), 0) == -1)
        printf("Send Failed!\n");

    char buffer[1024];
    int bytes, check = 0, count = 0;
    while((bytes = recv(sock, buffer, sizeof(buffer), 0)) > 0)
    {
        for(int i=0; i<bytes; i++)
        {
            if(buffer[i] == 'N' && buffer[i+1] == 'A' && buffer[i+2] == 'M' && buffer[i+3] == 'E' && buffer[i+4] == '\n')
                check = 1;
            if(buffer[i] == 'S' && buffer[i+1] == 'E' && buffer[i+2] == 'E' && buffer[i+3] == ' ')
                check = 0;
            if(check == 1)
            {
                putchar(buffer[i]);
                count++;
            }
        }
    }
    if(bytes == -1)
        printf("Receive failed!\n");
    else if(count == 0)
        printf("ERROR\n\tNo such command\n");
    
    close(sock);
}