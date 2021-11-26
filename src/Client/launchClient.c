/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: aurelio
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

#include "client.h"



int timer_id = 0;
int elapsed_time = 0;

/*
 * 
 */
int main(int argc, char** argv) {

    
    init_connection();
    
    do {
        send_action();
    } while (true);


    return (EXIT_SUCCESS);
}

