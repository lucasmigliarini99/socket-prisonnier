/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   clientcxnmanager.h
 * Author: aurelio
 *
 */
#include <pthread.h>

void on_window_main_destroy();
void on_button_click();
int timer_handler();
void on_Cancel();

init_interface(int argc, char **argv, int sockfdd, pthread_t thread);



