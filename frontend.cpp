#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<pthread.h>
#include<string.h>

using namespace std;

struct sendstruct
{
    string name;
    int balance;
};

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd1,sockfd2,sockfd3,portno,n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }

    //Connecting to server1____________________________________________________________________________________________
    portno = atoi(argv[2]);
    sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd1 < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    serv_addr.sin_port = htons(portno);
    //connecting to server1
    if (connect(sockfd1,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);

    n = write(sockfd1,buffer,strlen(buffer));
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer,256);

    n = read(sockfd1,buffer,255);
    if (n < 0)
        error("ERROR reading from socket");
    printf("%s\n",buffer);

    //___________________________________________________________________________________
    //connecting to server2

    sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd1 < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);


    serv_addr.sin_port = htons(atoi(argv[3]));
    if (connect(sockfd2,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    n = write(sockfd2,buffer,strlen(buffer));
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer,256);

    n = read(sockfd2,buffer,255);
    if (n < 0)
        error("ERROR reading from socket");
    printf("%s\n",buffer);


    //____________________________________________________________________________________
    //connecting to server3

    sockfd3 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd1 < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);

    serv_addr.sin_port = htons(atoi(argv[4]));
    if (connect(sockfd3,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    n = write(sockfd3,buffer,strlen(buffer));
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer,256);

    n = read(sockfd3,buffer,255);
    if (n < 0)
        error("ERROR reading from socket");
    printf("%s\n",buffer);


    close(sockfd1);
    close(sockfd2);
    close(sockfd3);
    return 0;
}
