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
/* #include "../network/ini.h" */

#include <gtk/gtk.h>

GtkBuilder *builder = NULL;

Joueur joueur;

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
    if(compteur != 0){
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
    //initialisation des fenêtres
    GtkWidget *win;
    win = GTK_WIDGET(gtk_builder_get_object(builder, "Win_Bienvenue"));
    gtk_builder_connect_signals(builder, NULL);

    GtkWidget *win2;
    win2 = GTK_WIDGET(gtk_builder_get_object(builder, "Win_Jeux"));

    //initialisation des bouttons denoncer et se taire
    GtkWidget *Denonce;
    Denonce = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonDenoncer"));

    GtkWidget *Taire;
    Taire = GTK_WIDGET(gtk_builder_get_object(builder, "ButtonTaire"));

    // recuperation du pseudo.
    printf("bouton 'Confirmer' clicked\n");
    GtkEntry *texte = GTK_ENTRY(gtk_builder_get_object(builder, "texte"));
    GtkEntry *data = (gchar *)gtk_entry_get_text(texte);
    send_pseudo(data);

    //fermeture de la fenetre actuelle
    gtk_widget_hide(win);

    //ouverture de la fenêtre suivante
    gtk_widget_show(win2);

    

    //demmarage du timer
    if (timer_id == 0)
    {
        timer_id = g_timeout_add(1000, (GSourceFunc)timer_handler, NULL);
    }
}

void on_Denoncer()
{
    //initialisation des fenêtres
    GtkWidget *win2;
    win2 = GTK_WIDGET(gtk_builder_get_object(builder, "Win_Jeux"));

    GtkWidget *win3;
    win3 = GTK_WIDGET(gtk_builder_get_object(builder, "Win_Score"));

    

    //incrementation du compteur de round
    
    send_action(2);
}

void on_Taire()
{
    GtkWidget *win2;
    win2 = GTK_WIDGET(gtk_builder_get_object(builder, "Win_Jeux"));

    GtkWidget *win3;
    win3 = GTK_WIDGET(gtk_builder_get_object(builder, "Win_Score"));

   
    send_action(1);
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
