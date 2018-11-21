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

    int **grille_bateaux, **grille_tir;
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
             case '1' : demander_placement_bateaux(pjoueur);
                        ia_placement_bateaux(ia_joueur);
                        break;
             case '2' : Afficher_grille(grille_bateaux, grille_tir, taille);
                        demande_tir(pjoueur, padvers);
                        tir_switch(pposs, taille, padvers, pjoueur);
                        break;
             case '3' : Sauv ();
                        break;
             case '4' : Charger();
                        break;
             case 'Q' :
             case 'q' : return 0;
                        break;
             default: printf("Erreur de choix\n");
                      goto debut_menu;

        }
    }
    return 0;
}



void Afficher_grille (int **grille_bateaux,int **grille_tir, int taille)
{
    int i, j;

        printf("    ");
    for(i = 0; i < taille; ++i){
        printf(" %d ", i);
    }
    printf("  |  ");
    printf("    ");
    for(i = 0; i < taille; ++i){
        printf(" %d ", i);
    }
    putchar('\n');
    printf("    ");
    for(i = 0; i<taille; ++i)
        printf("---");
    printf("  |  ");
     printf("    ");
    for(i = 0; i<taille; ++i)
        printf("---");
    putchar('\n');

    for(i=0; i<taille; i++) {
        printf("%c | ", 'A'+ i);
        for (j=0; j<taille; j++){
            printf(" %c ", grille_bateaux[i][j]);
        }
        printf("  |  ");
        printf("%c | ", 'A'+ i);
        for (j=0; j<taille; j++){
            printf(" %c ", grille_tir[i][j]);
        }
        putchar('\n');
    }
}


void afficher_menu(void)
{
 printf("Bataille Navale\n");
 printf("----------------------------\n");
 printf("1 : Placer les bateaux \n");
 printf("2 : Jouer\n");
 printf("3 : Sauvegarder la partie\n");
 printf("4 : Charger la partie\n");
 printf("q : Quitter\n");
}
