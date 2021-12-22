void send_player(connection_t *player, char buffer_in[BUFFERSIZE],char buffer_out[BUFFERSIZE]);
void send_attente(connection_t* connections[MAXSIMULTANEOUSCLIENTS],connection_t *player, Joueur buffer_in[BUFFERSIZE],Joueur buffer_out[BUFFERSIZE]);
Jeu jouer(Jeu jeu);