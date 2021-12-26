

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
0 --> Both players cooperate
1 --> Both players betray
2 --> The player is betrayed
3 --> The player betrays
*/
/**
 * @brief Make the game between player 1 and player 2
 * 
 * @param j1 
 * @param j2 
 * @return Jeu 
 */
Jeu jouer(Joueur j1, Joueur j2)
{
    Jeu jeu;
    //Both players cooperate
    if(j1.choix == 1 && j2.choix == 1)
    {
        j1.score += 2;
        j2.score += 2;
        j1.message = 0;
        j2.message = 0;
    }
    else
    {
        //Player 1 cooperates, player 2 betrays
        if(j1.choix == 1 && j2.choix == 2)
        {
            j1.score -= 1;
            j2.score += 3;
            j1.message = 2;
            j2.message = 3;
        }

        else
        {
            //player 1 betrays, player 2 cooperates
            if (j1.choix == 2 && j2.choix == 1)
            {
                j1.score += 3;
                j2.score -= 1;
                j1.message = 3;
                j2.message = 2;
            }
            //both players betray, no points
            else
            {
                j1.message = 1;
                j2.message = 1;
            }
        }
    }
    j1.score_adverse = j2.score;
    j2.score_adverse = j1.score;
    jeu.j1 = j1;
    jeu.j2 = j2;
    return jeu;
}