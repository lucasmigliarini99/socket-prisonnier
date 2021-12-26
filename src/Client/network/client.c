#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

#include "../interface/view.h"
#include "client.h"
#include "../config/ini.h"

Connection cnx;
Joueur j;
int save;

static int handler(void* config, const char* section, const char* name,
                   const char* value)
{
    // config instance for filling in the values.
    configuration* pconfig = (configuration*)config;

    // define a macro for checking Sections and keys under the sections.
    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

    // fill the values in config struct for Section 1.
    if(MATCH("Config 1", "adresse_ip")){ 
        pconfig->s1.adresse_ip = strdup(value);
    }else if(MATCH("Config 1", "port")){
        pconfig->s1.port = atoi(value);
    }
    // fill the values in config struct for Section 2.
    else if(MATCH("Config 2", "adresse_ip")){
        pconfig->s2.adresse_ip = strdup(value);
    }else if(MATCH("Config 2", "port")){
        pconfig->s2.port = atoi(value);
    }else{
        return 0;
    }

    return 1;
}

void *threadProcess(void * ptr) {
    Joueur buffer_in;
    int sockfd = *((int *) ptr);
    int len;
    int compteur = 0;
    int round;
    
    while ((len = read(sockfd, &buffer_in, sizeof(Joueur))) > 0) {
       
        j = buffer_in;
        printf("%s\n", j.pseudo);
        printf("score %d\n", j.score);
        if (j.enjeu == 1 && compteur == 0)
        {
            compteur = 1;
            debutPartie();
        }
        if(j.enjeu == 1 && compteur == 1){
            round = get_round();
            if(round >= 0 && round < 6 )
            {
                sleep (2);
                AffciherBTN();
            }
            if (round == 6){
                sleep(1);
                FinPartie();
                j.enjeu = 0;
                compteur = 0;
                j.score = 0;
                j.score_adverse = 0;
                round = -1;
            }
        }
        
    }
    close(sockfd);
    printf("client pthread ended, len=%d\n", len);
}

Joueur get_player()
{
    return j;
}

int open_connection() {

    configuration config;
    
    if (ini_parse("src/Client/config/config.ini", handler, &config) < 0) {
        printf("Can't load 'config.ini'\n");
        return 1;
    }

    int sockfd;

    struct sockaddr_in serverAddr;

    // Create the socket. 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //Configure settings of the server address
    // Address family is Internet 
    serverAddr.sin_family = AF_INET;
    //Set port number, using htons function 
    serverAddr.sin_port = htons(config.s1.port);
    //Set IP address to localhost
    serverAddr.sin_addr.s_addr = inet_addr(config.s1.adresse_ip);

    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    //Connect the socket to the server using the address
    if (connect(sockfd, (struct sockaddr *) &serverAddr, sizeof (serverAddr)) != 0) {
        printf("Fail to connect to server");
        exit(-1);
    };

    return sockfd;
} 

void init_connection(int argc, char** argv){

    int sockfd;
    int status = 0;
    char *msg = malloc(100);
    pthread_t thread;
    j.id = 0;
    


    cnx.socketClient = open_connection();
    init_interface(argc, argv,cnx.socketClient, thread);
    
}

void send_pseudo(char *pseudo){
    sprintf(j.pseudo, pseudo);
    
    printf("Send pseudo \n");
    write(cnx.socketClient, &j, sizeof(j));
}

void send_action(int choix){
    
    j.choix = choix;
    write(cnx.socketClient, &j, sizeof(j));

}
