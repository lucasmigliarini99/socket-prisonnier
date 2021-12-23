#include "../network/server.h"




void send_player(connection_t *player, char buffer_in[BUFFERSIZE],char buffer_out[BUFFERSIZE]);
void send_wait(connection_t* connections[MAXSIMULTANEOUSCLIENTS],connection_t *player, Joueur buffer_in);
Jeu jouer(Jeu jeu);