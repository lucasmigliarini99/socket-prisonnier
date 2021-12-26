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
#include "view.h"

#include "../network/client.h"

#include <gtk/gtk.h>

GtkBuilder *builder = NULL;

Joueur joueur;
char scoreMe[100];
char scoreHe[100];
char TourParty[100];

int sockfd;

int timer_id = 0;
int elapsed_time = 0;
int compteur = 0;

int temptour;

/**
 * @brief Close the window
 * 
 */
void on_window_main_destroy()
{
    printf("quitting\n ");
    gtk_main_quit();
}

/**
 * @brief Called when the button 'Yes' is clicked
 * 
 */
void on_button_click()
{
    printf("bouton 'Yes' clicked\n");
    GtkEntry *texte = GTK_ENTRY(gtk_builder_get_object(builder, "texte"));
    gchar *data = (gchar *)gtk_entry_get_text(texte);
    GtkEntry *echo = GTK_ENTRY(gtk_builder_get_object(builder, "echo"));
    gtk_entry_set_text(echo, data);
}

int timer_handler()
{
    elapsed_time++;
    char txt[100];
    //printf("timer running, time : %d\n", elapsed_time);
    if (compteur != 0)
    {
        GtkLabel *timelabel = GTK_LABEL(gtk_builder_get_object(builder, "time_display"));
        snprintf(txt, 100, "%04i", elapsed_time);
        gtk_label_set_text(timelabel, txt);
        return 1;
    }
}

/**
 * @brief Called when 'cancel' button is clicked
 * 
 */
void on_Cancel()
{
    GtkWidget *message_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
                                                       GTK_MESSAGE_WARNING,
                                                       GTK_BUTTONS_OK_CANCEL,
                                                       "Êtes vous sur de vouloir Quitter ?");
    //cf : http://refspecs.linuxbase.org/gtk/2.6/gtk/GtkMessageDialog.html
    //https://developer.gnome.org/gnome-devel-demos/stable/messagedialog.c.html.en
    unsigned int pressed = gtk_dialog_run(GTK_DIALOG(message_dialog));
    if (pressed == GTK_RESPONSE_OK)
    {
        printf("OK Pressed \n");
        printf("quitting\n ");
        gtk_widget_destroy(message_dialog);
        gtk_main_quit();
    }
    else
    {
        printf("CANCEL Pressed \n");
        gtk_widget_destroy(message_dialog);
    }
}

/**
 * @brief Called when the player enters his username and start
 * initialize the window
 * intialize the buttons
 * 
 */
void on_ConfirmationPseudo()
{
    //initialize the windows
    GtkWidget *win;
    win = GTK_WIDGET(gtk_builder_get_object(builder, "Win_Bienvenue"));
    gtk_builder_connect_signals(builder, NULL);

    GtkWidget *win2;
    win2 = GTK_WIDGET(gtk_builder_get_object(builder, "Win_Jeux"));

    //initialize the button Denonce and Taire
    GtkWidget *Denonce;
    Denonce = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonDenoncer"));

    GtkWidget *Taire;
    Taire = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonTaire"));

    //Initialize button QuitterJ et Rejouer
    GtkWidget *Rejouer;
    Rejouer = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonRejouer"));

    GtkWidget *QuitterJ;
    QuitterJ = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonQuitterjeu"));

    // recuperation du pseudo.
    printf("bouton 'Confirmer' clicked\n");
    GtkEntry *texte = GTK_ENTRY(gtk_builder_get_object(builder, "texte"));
    GtkEntry *data = (gchar *)gtk_entry_get_text(texte);
    send_pseudo(data);

    //close the current windows
    gtk_widget_hide(win);

    //open the next windows
    gtk_widget_show(win2);

    //hide the button
    gtk_widget_hide(Denonce);
    gtk_widget_hide(Taire);
    gtk_widget_hide(Rejouer);
    gtk_widget_hide(QuitterJ);
}

/**
 * @brief Called when the player betrays
 * 
 */
void on_Denoncer()
{
    GtkWidget *win2;
    win2 = GTK_WIDGET(gtk_builder_get_object(builder, "Win_Jeux"));


    GtkWidget *Denonce;
    Denonce = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonDenoncer"));

    GtkWidget *Taire;
    Taire = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonTaire"));

    gtk_widget_hide(Denonce);
    gtk_widget_hide(Taire);

    //stop the timer and return value in temptour
    temptour = elapsed_time;
    g_source_remove(timer_id);
    timer_id = 0;
    elapsed_time=0;

    compteur++;
    //return choice
    send_action(2);
}

/**
 * @brief Called when the player cooperates
 * 
 */
void on_Taire()
{
    GtkWidget *win2;
    win2 = GTK_WIDGET(gtk_builder_get_object(builder, "Win_Jeux"));



    GtkWidget *Denonce;
    Denonce = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonDenoncer"));

    GtkWidget *Taire;
    Taire = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonTaire"));

    gtk_widget_hide(Denonce);
    gtk_widget_hide(Taire);

    //stop the timer and return value in temptour
    temptour = elapsed_time;
    g_source_remove(timer_id);
    timer_id = 0;
    elapsed_time=0;

    compteur++;
    //return choice
    send_action(1);
}

/**
 * @brief Return the actual round
 * 
 */
int get_round()
{
    return compteur;
}

/**
 * @brief Return the actual timer
 * 
 */
int get_time()
{
    return temptour;
}

/**
 * @brief Make the buttons visible
 * 
 */
void AfficherBTN(){

    //get player datas
    joueur = get_player();


    //define text score, score enemy and party's round
    GtkWidget *tourParty;
    tourParty = GTK_WIDGET(gtk_builder_get_object(builder, "tour_party"));

    GtkWidget *Score;
    Score = GTK_WIDGET(gtk_builder_get_object(builder, "score"));

    GtkWidget *En_Score;
    En_Score = GTK_WIDGET(gtk_builder_get_object(builder, "en_score"));

    sprintf(scoreMe, "%d", joueur.score);
    gtk_label_set_text(GTK_LABEL(Score), (const gchar*)scoreMe);

    sprintf(scoreHe, "%d", joueur.score_adverse);
    gtk_label_set_text(GTK_LABEL(En_Score), (const gchar*)scoreHe);

    sprintf(TourParty, "Partie %d/5", compteur+1);
    gtk_label_set_text(GTK_LABEL(tourParty), (const gchar*)TourParty);

    GtkWidget *Denonce;
    Denonce = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonDenoncer"));

    GtkWidget *Taire;
    Taire = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonTaire"));

    gtk_widget_show(Denonce);
    gtk_widget_show(Taire);

    //start the timer
    if (timer_id == 0)
    {
        timer_id = g_timeout_add(1000, (GSourceFunc)timer_handler, NULL);
    }
}

/**
 * @brief Called at the end of the game to reset everything
 * 
 */
void FinPartie()
{
    joueur = get_player();

    //actualisation affichage score
    GtkWidget *Score;
    Score = GTK_WIDGET(gtk_builder_get_object(builder, "score"));

    GtkWidget *En_Score;
    En_Score = GTK_WIDGET(gtk_builder_get_object(builder, "en_score"));

    sprintf(scoreMe, "%d", joueur.score);
    gtk_label_set_text(GTK_LABEL(Score), (const gchar*)scoreMe);

    sprintf(scoreHe, "%d", joueur.score_adverse);
    gtk_label_set_text(GTK_LABEL(En_Score), (const gchar*)scoreHe);

    GtkWidget *Resultat;
    Resultat = GTK_WIDGET(gtk_builder_get_object(builder, "resulta"));

    //Initialize button QuitterJ et Rejouer
    GtkWidget *Rejouer;
    Rejouer = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonRejouer"));

    GtkWidget *QuitterJ;
    QuitterJ = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonQuitterjeu"));

    //show the button
    gtk_widget_show(Rejouer);
    gtk_widget_show(QuitterJ);

    if (joueur.score > joueur.score_adverse)
    {
        gtk_label_set_text(GTK_LABEL(Resultat), "vous avez gagné");
    }
    if (joueur.score == joueur.score_adverse)
    {
        gtk_label_set_text(GTK_LABEL(Resultat), "égalité");
    }
    if (joueur.score < joueur.score_adverse)
    {
        gtk_label_set_text(GTK_LABEL(Resultat), "vous avez perdu");
    }
}

/**
 * @brief Called at the beginning of a game
 * allows the player to play
 * 
 */
void debutPartie()
{
    //initialize the button Denonce and Taire
    GtkWidget *Denonce;
    Denonce = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonDenoncer"));

    GtkWidget *Taire;
    Taire = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonTaire"));

    //show the button
    gtk_widget_show(Denonce);
    gtk_widget_show(Taire);

    //define variable attente
    GtkWidget *Attente;
    Attente = GTK_WIDGET(gtk_builder_get_object(builder, "attente"));

    //hide "en attend joueur"
    gtk_widget_hide(Attente);
}

/**
 * @brief Called when the player click on "Rejouer"
 * 
 */
void on_Rejouer()
{
    //initialisation des fenêtres
    GtkWidget *win;
    win = GTK_WIDGET(gtk_builder_get_object(builder, "Win_Bienvenue"));

    GtkWidget *win2;
    win2 = GTK_WIDGET(gtk_builder_get_object(builder, "Win_Jeux"));

    //fermeture de la fenetre actuelle
    gtk_widget_hide(win2);

    //get player datas
    joueur = get_player();

    //ouverture de la premiere fenetre : celle du choix du pseudo
    gtk_widget_show(win);
    compteur = 0;
    sprintf(scoreMe, "%d", 0);
    sprintf(scoreHe, "%d", 0);

    GtkWidget *Resultat;
    Resultat = GTK_WIDGET(gtk_builder_get_object(builder, "resulta"));

    gtk_label_set_text(GTK_LABEL(Resultat), "");
}

/**
 * @brief Initialize the window
 * 
 */
int init_interface(int argc, char **argv, int sockfdd, pthread_t thread)
{
    GtkWidget *win;
    sockfd = sockfdd;

    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file("src/Client/interface/Interface.glade");
    win = GTK_WIDGET(gtk_builder_get_object(builder, "Win_Bienvenue"));
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show(win);

    pthread_create(&thread, 0, threadProcess, &sockfd);
    pthread_detach(thread);

    gtk_main();
    return (EXIT_SUCCESS);
}