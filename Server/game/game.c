

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h> 


#include "../network/server.h"










/*
Message :
0 --> Les deux joueurs coopèrent
1 --> Les deux joueurs trahissent
2 --> Le joueur a été trahi
3 --> Le joueur a trahi l'autre joueur
*/

Jeu jouer(Jeu jeu)
{
    //joueur 1 et joueur 2 se taisent
    if(jeu.j1.choix == 1 && jeu.j2.choix == 1)
    {
        jeu.j1.score += 2;
        jeu.j2.score += 2;
        jeu.j1.message = 0;
        jeu.j2.message = 0;
    }
    else
    {
        //joueur 1 se tait et joueur 2 dénonce
        if(jeu.j1.choix == 1 && jeu.j2.choix == 2)
        {
            jeu.j1.score -= 1;
            jeu.j2.score += 3;
            jeu.j1.message = 2;
            jeu.j2.message = 3;
        }
        
        else
        {
            //joueur 1 dénonce et joueur 2 se tait
            if (jeu.j1.choix == 2 && jeu.j2.choix == 1)
            {
                jeu.j1.score += 3;
                jeu.j2.score -= 1;
                jeu.j1.message = 3;
                jeu.j2.message = 2;
            }
            //joueur 1 et 2 dénoncent, pas de points
            else
            {
                jeu.j1.message = 1;
                jeu.j2.message = 1;
            }
        }
    }
}