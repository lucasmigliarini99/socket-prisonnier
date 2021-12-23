#include "ini.h"
#include "readerIniGame.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "../network/server.h"

Jeu jeux[2];
Joueur j1, j2, j3, j4;
Jeu jeu1, jeu2;

static int handler(void *recuperationJoueurs, const char *section, const char *name, const char *value)
{

    recup *precuperationJoueurs = (recup *)recuperationJoueurs;

// define a macro for checking Sections and keys under the sections.
#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

    // fill the values in config struct for Section 1.
    if (MATCH("Config", "game1"))
    {
        precuperationJoueurs->texte1 = strdup(value);
    }
    else if (MATCH("Config", "game2"))
    {
        precuperationJoueurs->texte2 = strdup(value);
    }
    else
    {
        return 0;
    }

    return 1;
}


Jeu get_party(Jeu games[2])
{
    recup recuperationJoueurs;
    if (ini_parse("config/config.ini", handler, &recuperationJoueurs) < 0)
    {
        printf("Can't load 'config.ini'\n");
    }
    printf("texte1= %s || texte2= %s\n", recuperationJoueurs.texte1, recuperationJoueurs.texte2);
    

    char *ptr = strtok(recuperationJoueurs.texte1, "-");
    char *joueur1 = ptr;
    ptr = strtok(NULL, "-");
    char *joueur2 = ptr;
    int id1 = atoi(joueur1);
    int id2 = atoi(joueur2);
    j1.id = id1;
    j2.id = id2;
    jeu1.j1 = j1;
    jeu1.j2 = j2;
    games[0] = jeu1;

    //---------------------------------------

    char *ptr2 = strtok(recuperationJoueurs.texte2, "-");
    char *joueur3 = ptr2;
    ptr = strtok(NULL, "-");
    char *joueur4 = ptr2;
    int id3 = atoi(joueur3);
    int id4 = atoi(joueur4);
    j3.id = id3;
    j4.id = id4;
    jeu2.j1 = j3;
    jeu2.j2 = j4;
    games[1] = jeu2;

   printf("Les id des joueurs sont: Joueur 1 du jeu#1= %d, Joueur 2 du jeu#1= %d, Joueur 1 du jeu#2 =%d, Joueur 2 du jeu#2= %d", games[0].j1.id, games[0].j2.id, games[1].j1.id, games[1].j2.id);
}
