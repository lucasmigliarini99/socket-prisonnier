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

Jeu get_party(Jeu games[2]);