#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

#include "client.h"


Connection cnx;

void *threadProcess(void * ptr) {
    char buffer_in[BUFFERSIZE];
    int sockfd = *((int *) ptr);
    int len;
    while ((len = read(sockfd, buffer_in, BUFFERSIZE)) != 0) {
        if (strncmp(buffer_in, "exit", 4) == 0) {
            break;
        }

        printf("receive %d chars\n", len);
        printf("%.*s\n", len, buffer_in);
    }
    close(sockfd);
    printf("client pthread ended, len=%d\n", len);
}



int open_connection() {
    int sockfd;

    struct sockaddr_in serverAddr;
    int port = 7799;

    // Create the socket. 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //Configure settings of the server address
    // Address family is Internet 
    serverAddr.sin_family = AF_INET;
    //Set port number, using htons function 
    serverAddr.sin_port = htons(port);
    //Set IP address to localhost
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    //Connect the socket to the server using the address
    if (connect(sockfd, (struct sockaddr *) &serverAddr, sizeof (serverAddr)) != 0) {
        printf("Fail to connect to server");
        exit(-1);
    };

    return sockfd;
}

void init_connection(){

    char msg[100];


    cnx.socketClient = open_connection();
    strcpy(msg, "Hello from Xeon"); //Xeon is the name of the this client
    printf("sending : %s\n", msg);
    write(cnx.socketClient, msg, strlen(msg));

    //Creation d'un pthread de lecture
    pthread_create(&cnx.thread, 0, threadProcess, &cnx.socketClient);
    //write(connection->sock,"Main APP Still running",15);
    pthread_detach(cnx.thread);
}

void send_action(){
    //send(cnx.socketClient, &msg, sizeof(msg),0);
    char msg[33];
    recv(cnx.socketClient, msg, 32, 0);
    printf("%s\n",msg);
    scanf("%s", msg);
    write(cnx.socketClient, msg, strlen(msg));
}
/*
int main(void)
{
    int socketClient = socket(AF_INET, SOCK_STREAM, 0);
    


    char msg[33];
    recv(socketClient, msg, 32, 0);
    printf("%s\n",msg);
    //scanf("%s %d", user.nom, &user.age);
    //send(socketClient, &user, sizeof(user),0);

    close(socketClient);

    return 0;

}*/