
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "server.h"
#include "../game/game.h"


Jeu jouer(Jeu jeu);

connection_t* connections[MAXSIMULTANEOUSCLIENTS];
int players[MAXSIMULTANEOUSCLIENTS];



void init_sockets_array() {
    for (int i = 0; i < MAXSIMULTANEOUSCLIENTS; i++) {
        connections[i] = NULL;
    }
}

void add(connection_t *connection) {
    for (int i = 0; i < MAXSIMULTANEOUSCLIENTS; i++) {
        if (connections[i] == NULL) {
            connections[i] = connection;
            return;
        }
    }
    perror("Too much simultaneous connections");
    exit(-5);
}

void del(connection_t *connection) {
    for (int i = 0; i < MAXSIMULTANEOUSCLIENTS; i++) {
        if (connections[i] == connection) {
            connections[i] = NULL;
            return;
        }
    }
    perror("Connection not in pool ");
    exit(-5);
}

/*
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_lock(&lock);
pthread_mutex_unlock(&lock);
 */

/**
 * Thread allowing server to handle multiple client connections
 * @param ptr connection_t 
 * @return 
 */
void *threadProcess(void *ptr) {
    Joueur buffer_in;
    //Joueur *buffer_out= malloc(sizeof(Joueur));

    

    int len;
    connection_t *connection;

    if (!ptr) pthread_exit(0);
    connection = (connection_t *) ptr;
    printf("New incoming connection \n");

    add(connection);

    buffer_in.id = connection->index;
    //addPlayer(connection->index);

    //Welcome the new client
    printf("Welcomeeeee #%i\n", connection->index);
    printf("Welcome Joueur:%i\n",connection->name);

    //init_jeux();

    while ((len = read(connection->sockfd, &buffer_in, sizeof(Joueur))) > 0) {

        // if (strncmp(buffer_in, "bye", 3) == 0) {
        //     break;
        // }

        printf("DEBUG-----------------------------------------------------------\n");
        printf("Player: %i\n",connection->name);
        printf("len : %i\n", len);
        printf("Buffer : %s\n",buffer_in.pseudo);
        printf("En jeu : %d\n",buffer_in.enjeu);
        printf("----------------------------------------------------------------\n");

        if (buffer_in.enjeu == 0)
        {
            send_wait(connections,connection,buffer_in);
        }

        // sprintf(buffer_in.pseudo, "jesuistest");
        // buffer_in.enjeu = 1;
        // printf("En jeu: %d\n",buffer_in.enjeu);
        // write(connection->sockfd, &buffer_in, sizeof(buffer_in));

        /*for(i = 0; i<sizeof(jeux); i++)
        {
            if(jeux[i].j1.choix != NULL && jeux[i].j2.choix != NULL)
            {
                jeux[i] = jouer(jeux[i]);
                send_player(connections[jeux[i].j1.id],buffer_in, &jeux[i].j1);

                jeux[i].j1.choix = NULL;
                jeux[i].j2.choix = NULL;
            }
        }      */
        

        //clear input buffer
       // memset(buffer_in, '\0', sizeof(Joueur));
    }
    printf("Connection to client %i ended \n", connection->index);
    close(connection->sockfd);
    del(connection);
    free(connection);
    pthread_exit(0);

}




int create_server_socket() {
    int sockfd = -1;
    struct sockaddr_in address;
    int port = 3379;

    /* create socket */
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd <= 0) {
        fprintf(stderr, "%s: error: cannot create socket\n");
        return -3;
    }


    /* bind socket to port */
    address.sin_family = AF_INET;
    //bind to all ip : 
    //address.sin_addr.s_addr = INADDR_ANY;
    //ou 0.0.0.0 
    //Sinon  127.0.0.1
    address.sin_addr.s_addr = inet_addr("0.0.0.0");
    address.sin_port = htons(port);

    /* prevent the 60 secs timeout */
    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*) &reuse, sizeof (reuse));

    /* bind */
    if (bind(sockfd, (struct sockaddr *) &address, sizeof (struct sockaddr_in)) < 0) {
        fprintf(stderr, "error: cannot bind socket to port %d\n", port);
        return -4;
    }

    return sockfd;
}


