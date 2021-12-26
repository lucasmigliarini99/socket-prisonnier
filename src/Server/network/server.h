

/* 
 * File:   srvcxnmanager.h
 * Author: aurelio
 *
 */

#ifndef SERVER_H
#define SERVER_H

#define BUFFERSIZE 2048
#define MAXSIMULTANEOUSCLIENTS 100

typedef struct {
    int sockfd;
    struct sockaddr address;
    int addr_len;
    int index;
    int name;
} connection_t;


typedef struct Joueur
{
    int sockfd;
    int id;
    char pseudo[256];
    int score;
    int score_adverse;
    int choix;
    int message;
    int enjeu;
    int timeRound[5];
} Joueur;

typedef struct Jeu
{
    Joueur j1;
    Joueur j2;
} Jeu;




void init_sockets_array();
void add(connection_t *connection);
void del(connection_t *connection);
void *threadProcess_Server(void *ptr);
int create_server_socket() ;

#endif /* SERVER_H */

