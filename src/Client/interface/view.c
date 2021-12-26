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


int sockfd;

int timer_id = 0;
int elapsed_time = 0;
int compteur = 0;

/**
 * Fermeture de la fenetre
 * cette fonction est invoke par le gestionnaire d'evt
 */
void on_window_main_destroy()
{
    printf("quitting\n ");
    gtk_main_quit();
}

/**
 * Fonction invoque lors du click sur le bouton 'Yes'
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
 * Gestion du bouton toggle_one avec demarrage/arret d'un timer
 * @param widget  le widget qui a emit l'evt
 * @param data des donnees supplementatire
 */
void on_toogle(GtkWidget *widget, gpointer data)
{
    GtkToggleButton *btn = GTK_TOGGLE_BUTTON(widget);
    gchar *name = (gchar *)gtk_widget_get_name(widget);
    gboolean status = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
    printf("toggle %s pressed, status : %d\n", name, status);
    //Activation du timer s'il etait arrete
    if (timer_id == 0)
    {
        timer_id = g_timeout_add(1000, (GSourceFunc)timer_handler, NULL);
    }
    else
    {
        g_source_remove(timer_id);
        timer_id = 0;
    }
}

void on_Cancel()
{
    GtkWidget *message_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
                                                       GTK_MESSAGE_WARNING,
                                                       GTK_BUTTONS_OK_CANCEL,
                                                       "Êtes vous sur de vouloir quitter ?");
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

    //start the timer
    if (timer_id == 0)
    {
        timer_id = g_timeout_add(1000, (GSourceFunc)timer_handler, NULL);
    }
}

void on_Denoncer()
{
    GtkWidget *win2;
    win2 = GTK_WIDGET(gtk_builder_get_object(builder, "Win_Jeux"));

    //get player datas
    joueur = get_player();

    //define text score, score enemy, name, name enemy and party's round
    GtkWidget *tourParty;
    tourParty = GTK_WIDGET(gtk_builder_get_object(builder, "tour_party"));

    GtkWidget *Score;
    Score = GTK_WIDGET(gtk_builder_get_object(builder, "score"));

    GtkWidget *En_Score;
    En_Score = GTK_WIDGET(gtk_builder_get_object(builder, "en_score"));

    GtkWidget *Choix;
    Choix = GTK_WIDGET(gtk_builder_get_object(builder, "choix"));

    GtkWidget *En_Choix;
    En_Choix = GTK_WIDGET(gtk_builder_get_object(builder, "en_choix"));

    //print round of the game
    //gtk_label_set_text(GTK_LABEL(tourParty), compteur);

    //print choice of players
    //gtk_label_set_text(GTK_LABEL(Choix), joueur.choix);

    /*tk_label_set_text(GTK_LABEL (En_Choix), "vous : %d", joueur.choix);  --> besoin de choix adverse*/

    //print score

    sprintf(scoreMe, "%d", joueur.score);
    gtk_label_set_text(GTK_LABEL(Score), (const gchar*)scoreMe);

    

    GtkWidget *Denonce;
    Denonce = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonDenoncer"));

    GtkWidget *Taire;
    Taire = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonTaire"));

    gtk_widget_hide(Denonce);
    gtk_widget_hide(Taire);

    compteur++;
    //return choice
    send_action(2);
}

void on_Taire()
{
    GtkWidget *win2;
    win2 = GTK_WIDGET(gtk_builder_get_object(builder, "Win_Jeux"));


    //get player datas
    joueur = get_player();

    //define text score, score enemy, name, name enemy and party's round
    GtkWidget *tourParty;
    tourParty = GTK_WIDGET(gtk_builder_get_object(builder, "tour_party"));

    GtkWidget *Score;
    Score = GTK_WIDGET(gtk_builder_get_object(builder, "score"));

    GtkWidget *En_Score;
    En_Score = GTK_WIDGET(gtk_builder_get_object(builder, "en_score"));

   

    //print score
    printf("le score de moi %d",joueur.score);
    sprintf(scoreMe, "%d", joueur.score);
    gtk_label_set_text(GTK_LABEL(Score), (const gchar*)scoreMe);

    sprintf(scoreHe, "%d", joueur.score_adverse);
    gtk_label_set_text(GTK_LABEL(En_Score), (const gchar*)scoreHe);


    GtkWidget *Denonce;
    Denonce = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonDenoncer"));

    GtkWidget *Taire;
    Taire = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonTaire"));

    gtk_widget_hide(Denonce);
    gtk_widget_hide(Taire);

     compteur++;
    //return choice
    send_action(1);
   

    
}

int get_round()
{
    return compteur;
}

void AffciherBTN(){
    GtkWidget *Denonce;
    Denonce = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonDenoncer"));

    GtkWidget *Taire;
    Taire = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonTaire"));

    gtk_widget_show(Denonce);
    gtk_widget_show(Taire);
}

void FinPartie()
{
    joueur = get_player();

    GtkWidget *Choix;
    Choix = GTK_WIDGET(gtk_builder_get_object(builder, "choix"));

    GtkWidget *En_Choix;
    En_Choix = GTK_WIDGET(gtk_builder_get_object(builder, "en_choix"));


    //print choice of players
    

    /*tk_label_set_text(GTK_LABEL (En_Choix), "vous : %d", joueur.choix);  --> besoin de choix adverse*/

    if(joueur.score > joueur.score_adverse)
    {
       gtk_label_set_text(GTK_LABEL(Choix), "avez gagné");
    }
    if(joueur.score == joueur.score_adverse){
        gtk_label_set_text(GTK_LABEL(Choix), "égalité");
        gtk_label_set_text(GTK_LABEL (En_Choix), "égalité");
    }
    if(joueur.score < joueur.score_adverse){
        gtk_label_set_text(GTK_LABEL (En_Choix), "a gagné");
    }
}

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

void on_Rejouer()
{
    //initialisation des fenêtres
    GtkWidget *win;
    win = GTK_WIDGET(gtk_builder_get_object(builder, "Win_Bienvenue"));

    GtkWidget *win3;
    win3 = GTK_WIDGET(gtk_builder_get_object(builder, "Win_Score"));

    //fermeture de la fenetre actuelle
    gtk_widget_hide(win3);

    //ouverture de la premiere fenetre : celle du choix du pseudo
    gtk_widget_show(win);
}

/*
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