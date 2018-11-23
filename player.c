#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include <string.h>
#include <ctype.h>

static int **nouvelle_grille (int taille);
static void placement_bateaux(int **grille, int taille,  unsigned int x, unsigned int y, char direction, int longueur, char caractere);
static int verification_placement(int **grille, int taille, unsigned int x, unsigned int y, char direction, int longueur);
static void marquage_tir(int **grille_tir, int **grille_bateau_adv,unsigned int x, unsigned int y, t_bateau bateaux_adv[4]);
static int verification_tir(int **grille_tir, int taille, unsigned int x, unsigned int y);
static void demande_tir(t_joueur *pjoueur,t_joueur *padvers);

static void flush_stdin(void)
{
    int c;
    while((c = getchar()) != EOF && c != '\n'){
        ;
    }
}

int **nouvelle_grille (int taille)
{
    int **grille;
    grille = malloc(taille*sizeof(*grille));
    for (int i=0; i<taille; i++) {
        grille[i] = malloc(taille*sizeof(**grille));
        for (int j=0; j<taille; j++) {
            grille[i][j] = '0';
        }
    }
    return grille;
}

static void free_grille(int **grille, int taille)
{
    int i;
    for(i=0; i<taille; ++i){
        free(grille[i]);
    }
    free(grille);
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

t_joueur *nouveau_player(int taille)
{
     t_joueur *joueur;
     joueur = malloc(sizeof(*joueur));

     joueur->taille = taille;

     joueur->grille_bateaux = nouvelle_grille(taille);
     joueur->grille_tir = nouvelle_grille(taille);

     /* Porte avion */

     joueur->bateaux[0].longueur= 5;
     joueur->bateaux[0].nbtouche=0;
     strncpy(joueur->bateaux[0].nom, "Porte Avion", NOMBAT_LONG);
     joueur->bateaux[1].longueur= 4;
     joueur->bateaux[1].nbtouche=0;
     strncpy(joueur->bateaux[1].nom, "Cuirasse", NOMBAT_LONG);
     joueur->bateaux[2].longueur= 3;
     joueur->bateaux[2].nbtouche=0;
     strncpy(joueur->bateaux[2].nom, "Sous Marin", NOMBAT_LONG);
     joueur->bateaux[3].longueur= 2;
     joueur->bateaux[3].nbtouche=0;
     strncpy(joueur->bateaux[3].nom, "Fregate", NOMBAT_LONG);

    return joueur;
}

void free_player(t_joueur *pjoueur)
{
    free_grille(pjoueur->grille_bateaux, pjoueur->taille);
    free_grille(pjoueur->grille_tir, pjoueur->taille);
    free(pjoueur);


}

void demander_placement_bateaux(t_joueur *pjoueur)
{
    unsigned int x , y, j;
    int status;
    char direction, ylettre;

    for (j = 0; j < 4; ++j){
        do{
            debut_placement:
            printf("Place le %s ! x y direction\n", pjoueur->bateaux[j].nom);
            status = scanf("%u %c %c", &x, &ylettre, &direction);
            flush_stdin();
            if (!isalpha(ylettre)){
                status = -1;
            }
            y = toupper(ylettre) - 'A';
            direction = toupper(direction);
        }while(status != 3 || x >= pjoueur->taille || y >= pjoueur->taille || ( direction != 'N' && direction != 'S' && direction != 'E' && direction != 'O') );

        if ( verification_placement(pjoueur->grille_bateaux, pjoueur->taille, x, y, direction, ((pjoueur->bateaux)[j]).longueur) == 1){
            pjoueur->bateaux[j].x = x;
            pjoueur->bateaux[j].y = y;
            pjoueur->bateaux[j].sens = direction;
            placement_bateaux(pjoueur->grille_bateaux, pjoueur->taille, x, y, direction, pjoueur->bateaux[j].longueur, pjoueur->bateaux[j].nom[0]);
        }else{
            goto debut_placement;
        }

    }


}

int verification_placement(int **grille, int taille, unsigned int x, unsigned int y, char direction, int longueur)
{
    int i;
    int occupe = 0;


    switch (direction)
    {
    case 'N':
        if (y - longueur + 1 >= taille)
            return 0;
        break;
    case 'S':
        if (y + longueur - 1 >= taille)
            return 0;
        break;
    case 'E':
        if (x + longueur - 1 >= taille)
            return 0;
        break;
    case 'O':
        if (x - longueur + 1 < 0)
            return 0;
        break;
    default: printf("Mauvaise direction !\n");
        return 0;
    }

    switch(direction)
    {
    case 'N':
        for (i= 0; i< longueur && !occupe ; ++i)
            occupe = grille[y - i][x] == '0' ? 0 : 1;
        break;
    case 'S':
        for (i= 0; i< longueur && !occupe ; ++i)
            occupe = grille[y + i][x] == '0' ? 0 : 1;
        break;
    case 'O':
        for (i= 0; i< longueur && !occupe ; ++i)
            occupe = grille[y][x - i] == '0' ? 0 : 1;
        break;
    case 'E':
         for (i= 0; i< longueur && !occupe ; ++i)
            occupe = grille[y][x + i] == '0' ? 0 : 1;
        break;
    default: printf("Mauvaise direction !\n");
        return 0;
    }
    return !occupe;

}

void placement_bateaux(int **grille, int taille,  unsigned int x, unsigned int y, char direction, int longueur, char caractere)
{
    int i;

    switch(direction)
    {
    case 'N':
        for (i= 0; i< longueur; ++i)
            grille[y - i][x] = caractere;
        break;
    case 'S':
        for (i= 0; i< longueur; ++i)
            grille[y + i][x] = caractere;
        break;
    case 'O':
        for (i= 0; i< longueur; ++i)
            grille[y][x - i] = caractere;
        break;
    case 'E':
         for (i= 0; i< longueur; ++i)
            grille[y][x + i] = caractere;
        break;
    default: printf("Mauvaise direction !\n");
    }
}

void demande_tir(t_joueur *pjoueur,t_joueur *padvers)
{
    unsigned int x , y;
    char yletter;
    int status;

    do{
        debut_tir:
        printf("Vas y tire ! x y\n");
        status = scanf("%u %c", &x, &yletter);
         flush_stdin();
        if (!isalpha(yletter)){
            status = -1;
        }
        y = toupper(yletter) - 'A';

    }while(status != 2 || x >= pjoueur->taille || y >= pjoueur->taille);
    if (verification_tir(pjoueur->grille_tir, pjoueur->taille, x, y)){
        marquage_tir(pjoueur->grille_tir, padvers->grille_bateaux, x, y, padvers->bateaux);
    }else{
        goto debut_tir;
    }
}

int verification_tir(int **grille_tir, int taille, unsigned int x, unsigned int y)
{
        return grille_tir[y][x] == '0';
}

static int cherche_bateau(t_bateau bateaux[4], char lettre)
{
    int i;
    for(i=0; i<4; ++i){
        if(lettre == bateaux[i].nom[0]){
            return i;
        }
    }
    return -1;
}

static void marque_mort(int **grille_tir, int **grille_adv, t_bateau *pbateau)
{
    int i;
    unsigned x, y;
    x = pbateau->x;
    y = pbateau->y;
    switch(pbateau->sens)
    {
    case 'N':
        for (i=0; i<pbateau->longueur; ++i)
            grille_tir[y - i][x] = grille_adv[y-i][x] = 'D';
        break;
    case 'S':
        for (i=0; i<pbateau->longueur; ++i)
            grille_tir[y + i][x] = grille_adv[y+i][x] = 'D';
        break;
    case 'O':
        for (i=0; i<pbateau->longueur; ++i)
            grille_tir[y][x - i] = grille_adv[y][x-i] = 'D';
        break;
    case 'E':
         for (i=0; i<pbateau->longueur; ++i)
            grille_tir[y][x + i] = grille_adv[y][x+i] = 'D';
        break;
    default: printf("Mauvaise direction !\n");
    }
}

void marquage_tir(int **grille_tir, int **grille_bateau_adv,unsigned int x, unsigned int y, t_bateau bateaux_adv[4])
{
    int i;
    if (grille_bateau_adv[y][x] == '0'){
        grille_tir[y][x] = ' ';
        printf("C'est Plouf\n");
    }else{
        i = cherche_bateau(bateaux_adv, grille_bateau_adv[y][x]);
        // TODO check i value

        bateaux_adv[i].nbtouche++;
        if (bateaux_adv[i].nbtouche == bateaux_adv[i].longueur){
            marque_mort(grille_tir, grille_bateau_adv, &(bateaux_adv[i]));
            printf("Le %s est coule\n", bateaux_adv[i].nom);

        }else{
        grille_tir[y][x] = 'T';
        grille_bateau_adv[y][x] = 'T';
        printf("c'est touche\n");
        }
    }
}





typedef struct s_liste{
    struct{ int x;
            int y;
            char direction;
             } valeur;
    struct s_liste *next;
 } t_liste;

static t_liste *prepend(int x, int y, char direction, t_liste *liste)
{
    t_liste *new_element = malloc(sizeof(*new_element));
    new_element->valeur.x = x;
    new_element->valeur.y = y;
    new_element->valeur.direction = direction;
    new_element->next = liste;

    return new_element;
}

static void free_liste(t_liste *liste)
{

    t_liste *courant, *suivant;

    courant = liste;
    while (courant != NULL){
        suivant = courant->next;
        free(courant);
        courant = suivant;

    }
}

static t_liste *get_element(t_liste *liste, int i)
{
   unsigned int index = 0;
   t_liste *courant;

   courant = liste;
   while(index != i && courant != NULL ){
        courant = courant->next;
        ++index;

   }
   return courant;
}

static unsigned int nb_element(t_liste *liste)
{
    unsigned int i = 0;
    t_liste *courant;

    courant = liste;
    while (courant){
        courant = courant->next;
        ++i;

    }
    return i;
}

static t_liste *scan_positions(int **grille_bateaux, int taille, unsigned int longueur)
{
    t_liste *liste;
    int x, y;

    liste = NULL;
    for(x=0; x<taille; ++x){
        for(y=0; y<taille; ++y){
            if (verification_placement(grille_bateaux, taille, x, y, 'N', longueur) == 1){
                liste = prepend(x, y, 'N', liste);
            }
            if (verification_placement(grille_bateaux, taille, x, y, 'S', longueur) == 1){
                liste = prepend(x, y, 'S', liste);
            }
            if (verification_placement(grille_bateaux, taille, x, y, 'E', longueur) == 1){
                liste = prepend(x, y, 'E', liste);
            }
            if (verification_placement(grille_bateaux, taille, x, y, 'O', longueur) == 1){
                liste = prepend(x, y, 'O', liste);
            }

        }
    }

    return liste;
}

static void placement_rand(int **grille_bateaux, int taille,  t_liste *liste, t_bateau *pbateau)
{
    unsigned int n;
    int i;
    t_liste *noeud;

    n = nb_element(liste);
    i = rand()%n;
    noeud = get_element(liste, i);
    pbateau->x = noeud->valeur.x;
    pbateau->y = noeud->valeur.y;
    pbateau->sens = noeud->valeur.direction;
    placement_bateaux(grille_bateaux, taille, noeud->valeur.x, noeud->valeur.y, noeud->valeur.direction, pbateau->longueur, pbateau->nom[0]);
}


void ia_placement_bateaux(t_joueur *ia_joueur)
{
    int i;
    t_liste *liste;

    for(i=0; i<4; ++i){
      liste = NULL;
      liste = scan_positions(ia_joueur->grille_bateaux, ia_joueur->taille, ia_joueur->bateaux[i].longueur);
      placement_rand(ia_joueur->grille_bateaux, ia_joueur->taille, liste, &(ia_joueur->bateaux[i]));
      free_liste(liste);
    }

}

static int nb_bateaux(t_joueur *pjoueur)
{
    int j, c;
    j=4;

    for (c=0; c < 4; ++c){
        if (pjoueur->bateaux[c].longueur == pjoueur->bateaux[c].nbtouche){
            --j;
        }
    }
    return j;
}

typedef struct s_position{
    unsigned int x;
    unsigned int y;
}t_position;

typedef struct s_possibilite_tir{
    t_position *t;
    int tailleff ;
    }t_possibilite_tir;

static t_possibilite_tir *init_possibilite(int taille)
{
    int i;

    t_possibilite_tir *r;
    t_position *s;

    r = malloc(sizeof(*r));
    s = malloc(taille*taille*sizeof(*s));
    for (i=0; i< taille*taille; ++i){
        s[i].x = i%taille;
        s[i].y = i/taille;
    }

    r->t = s;
    r->tailleff = taille*taille;
    return r;
}

static void free_possibilite(t_possibilite_tir *pposs){
    free(pposs->t);
    free(pposs);
}

static t_position *attribution_tir(t_possibilite_tir *pposs){
    int i;
    t_position z;

    i = rand()% pposs->tailleff;
    z.x = pposs->t[i].x;
    z.y = pposs->t[i].y;

    pposs->t[i].x = pposs->t[pposs->tailleff-1].x;
 pposs->t[i].y = pposs->t[pposs->tailleff-1].y;

     pposs->t[pposs->tailleff-1].x = z.x;
          pposs->t[pposs->tailleff-1].y = z.y;

     pposs->tailleff -= 1;

     return &(pposs->t[pposs->tailleff-1]);
}


void tir_switch(t_joueur *pjoueur, t_joueur *padvers)
{

    int i;
	t_possibilite_tir *ppos; /* variable locale à la fonction */
	t_position *pos;
    i=0;
	ppos = init_possibilite(pjoueur->taille); /* S'initialise une fois par partie ( génère toutes les possibilités de tir ) */
    while(nb_bateaux(pjoueur)!= 0 && nb_bateaux(padvers) != 0){ /* Verification aussi que l'adversaire n'a pas perdu */
		/* AFFICHER LA GRILLE DU JOUEUR, qqch comme :
		affichage_grille(pjoueur); */
        Afficher_grille (pjoueur->grille_bateaux, pjoueur->grille_tir, pjoueur->taille);
        if ((i%2) == 0){
            demande_tir(pjoueur, padvers);
        }else{
            pos = attribution_tir(ppos);
            printf("Adversaire tire a %u %c... ", pos->x, pos->y + 'A');
			marquage_tir(padvers->grille_tir, pjoueur->grille_bateaux, pos->x, pos->y, pjoueur->bateaux);
        }
     ++i;
    }
	free_possibilite(ppos);
    if (i%2 == 0){
    printf("Partie finie ! Vous avez perdu");
    }else{
    printf("Partie finie ! Vous avez gagné");
    }
}

