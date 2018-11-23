#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "player.h"

#define TAILLE 8

void Afficher_grille (int** grille_bateaux, int **grille_tir, int taille);
void afficher_menu(void);



static void flush_stdin(void)
{
    int c;
    while((c = getchar()) != EOF && c != '\n'){
        ;
    }
}

int main()
{

    t_joueur *joueur1, *joueur2;
    int taille = TAILLE;


    char choix;
    srand(time(NULL));

    while(1){
        afficher_menu ();
        printf("Entrez votre choix\n");
        debut_menu:
        choix = getchar();
        flush_stdin();

        switch (choix)
        {
             case '1' : joueur1 = nouveau_player(taille);
                        joueur2 = nouveau_player(taille);
                        demander_placement_bateaux(joueur1);
                        ia_placement_bateaux(joueur2);
                        tir_switch(joueur1, joueur2);
                        free_player(joueur1);
                        free_player(joueur2);


                        break;
             /*
             case '2' :// Afficher_grille(grille_bateaux, grille_tir, taille);
                        //demande_tir(pjoueur, padvers);
                        //tir_switch(pposs, taille, padvers, pjoueur);
                        break;
             case '3' : //Sauv ();
                        break;
             case '4' : //Charger();
                        break; */
             case 'Q' :
             case 'q' : return 0;
                        break;
             default: printf("Erreur de choix\n");
                      goto debut_menu;

        }
    }
    return 0;
}






void afficher_menu(void)
{
 printf("Bataille Navale\n");
 printf("----------------------------\n");
 printf("1 : Jouer\n");
 printf("q : Quitter\n");
}
