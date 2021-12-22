#include "ini.h"
#include "../Server/server.h"
#include <string.h>
#include <stdio.h>

Jeu jeux[2];

void readGame()
{
    if (ini_parse("config/config.ini", handler, &jeux) < 0)
    {
        printf("Can't load 'config.ini'\n");
        return 1;
    }
}

static int handler(void *jeu, const char *section, const char *name,
                   const char *value)
{
    Jeu jeu1, jeu2;
    Joueur j1, j2, j3, j4;
    char texteADecouper1;
    char texteADecouper2;

// define a macro for checking Sections and keys under the sections.
#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

    // fill the values in config struct for Section 1.
    if (MATCH("Config", "game1"))
    {
        char texteADecouper1 = strdup(value);
    }
    else
    {
        return 0;
    }

    int init_size = strlen(texteADecouper1);
    char delim[] = "-";
    char *ptr = strtok(texteADecouper1, delim);

    char *tab1;
    for (int i = 0; i < 2; i++)
    {
        tab1[i] = ptr;
        ptr = strtok(NULL, delim);
    }

    j1.id = tab1[0];
    j2.id = tab1[1];
    jeu1.j1 = j1;
    jeu1.j2 = j2;
    jeux[0] = jeu1;

    // fill the values in config struct for Section 1.
    if (MATCH("Config", "game2"))
    {
        char texteADecouper2 = strdup(value);
    }
    else
    {
        return 0;
    }

    int init_size = strlen(texteADecouper2);
    char delim[] = "-";
    char *ptr = strtok(texteADecouper2, delim);

    char *tab2;
    for (int i = 0; i < 2; i++)
    {
        tab2[i] = ptr;
        ptr = strtok(NULL, delim);
    }

    j3.id = tab2[0];
    j4.id = tab2[1];
    jeu2.j1 = j3;
    jeu2.j2 = j4;
    jeux[1] = jeu2;

    return 1;
}