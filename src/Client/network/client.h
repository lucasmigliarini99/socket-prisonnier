/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   clientcxnmanager.h
 * Author: aurelio
 *
 */

#ifndef CLIENTCXNMANAGER_H
#define CLIENTCXNMANAGER_H

#define BUFFERSIZE 2048

typedef struct{
    int socketClient;
    pthread_t thread;
} Connection;

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
} Joueur;

void *threadProcess(void * ptr);
int open_connection();
void init_connection();
void send_action(int choix);
void send_pseudo(char *pseudo);
void get_player(Joueur player);

#endif /* CLIENTCXNMANAGER_H */

// define a structure for holding the values in "Section 1".
typedef struct{
    int port;
    const char* adresse_ip;
} section_one;

// define a structure for holding the values in "Section 2".
typedef struct{
    int port;
    const char* adresse_ip;
} section_two;

// define a structure for holding all of the config.
typedef struct
{
    section_one s1;
    section_two s2;
} configuration;

