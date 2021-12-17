

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
    int id;
    char pseudo[256];
    int score;
    int choix;
    int message;
    int enjeu;
} Joueur;




void init_sockets_array();
void add(connection_t *connection);
void del(connection_t *connection);
void *threadProcess(void *ptr);
int create_server_socket() ;
void send_all_player(connection_t *player, char buffer_in[BUFFERSIZE],char buffer_out[BUFFERSIZE]);

#endif /* SERVER_H */

