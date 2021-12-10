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
    int id;
    char pseudo[256];
    int score;
    int message;
} Joueur;

void *threadProcess(void * ptr);
int open_connection();
void init_connection();
void send_action();
void send_msg();

#endif /* CLIENTCXNMANAGER_H */

