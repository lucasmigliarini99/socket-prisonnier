#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../network/server.h"

typedef struct
{
    const char *texte1;
    const char *texte2;
} recup;

int get_party(Jeu games[2]);