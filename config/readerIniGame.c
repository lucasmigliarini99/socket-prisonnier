#include "ini.h"
#include "readerIniGame.h"
#include <string.h>
#include <stdio.h>

void readGame()
{
    game partie1;

    if (ini_parse("config/config.ini", handler, &partie1) < 0)
    {
        printf("Can't load 'config.ini'\n");
        return 1;
    }
}

static int handler(void *Partie, const char *section, const char *name,
                   const char *value)
{
    // config instance for filling in the values.
    games *pPartie = (games *)Partie;
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

    char *tab;
    for (int i = 0; i < 2; i++)
    {
        tab[i] = ptr;
        ptr = strtok(NULL, delim);
    }

    pPartie->partie1.joueur1 = tab[0];
    pPartie->partie1.joueur2 = tab[1];
    print("Les joueurs qui jouent sont joueur %i et %i.", pPartie->partie1.joueur1, pPartie->partie1.joueur2);

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

    char *tab;
    for (int i = 0; i < 2; i++)
    {
        tab[i] = ptr;
        ptr = strtok(NULL, delim);
    }

    pPartie->partie2.joueur1 = tab[0];
    pPartie->partie2.joueur2 = tab[1];
    print("Les joueurs qui jouent sont joueur %i et %i.", pPartie->partie2.joueur1, pPartie->partie2.joueur2);

    return 1;
}