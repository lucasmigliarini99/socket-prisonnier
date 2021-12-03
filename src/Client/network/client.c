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
int save;

void *threadProcess(void * ptr) {
    char buffer_in[BUFFERSIZE];
    int sockfd = *((int *) ptr);
    int len;

    save = sockfd;

    
    while ((len = read(sockfd, buffer_in, BUFFERSIZE)) != 0 && 1) {
    
        

        
        if (strncmp(buffer_in, "exit", 4) == 0) {
            break;
        }
        if (save != sockfd){
            printf("receive %d chars\n", len);
            printf("%.*s\n", len, buffer_in);
        }

        
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

void init_connection(int argc, char** argv){

    int sockfd;
    int status = 0;
    char *msg = malloc(100);
    pthread_t thread;


    sockfd = open_connection();
    init_interface(argc, argv,sockfd, thread);
    
}

void send_msg(){
    char msg[33];
    printf("Mot: \n");
    scanf("%s", msg);
    send_action(msg);
}

void send_action(char msg[33]){
    //send(cnx.socketClient, &msg, sizeof(msg),0);
    //recv(cnx.socketClient, msg, 32, 0);
    write(cnx.socketClient, msg, strlen(msg));

}
