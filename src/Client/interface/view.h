
/* 
 * File:   view.h
 * Author: Lucas
 *
 */
#include <pthread.h>

typedef struct
{
    int score;
    char pseudo[64];
    int *choix;
    int enjeu;

} Joueurss;

void on_window_main_destroy();
void on_button_click();
int timer_handler();
void on_Cancel();

init_interface(int argc, char **argv, int sockfdd, pthread_t thread);



