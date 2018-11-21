#ifndef PLAYER_H_INC
#define PLAYER_H_INC
#define NOMBAT_LONG 60


struct s_bateau{
    char nom[NOMBAT_LONG + 1];
    unsigned int longueur;
    unsigned int x;
    unsigned int y;
    unsigned int nbtouche;
    char sens;
    };

typedef struct s_bateau t_bateau;

typedef struct s_joueur{
    int **grille_bateaux;
    int **grille_tir;
    int taille;
    t_bateau bateaux[4];


} t_joueur;

t_joueur *nouveau_player(int taille);
void demander_placement_bateaux(t_joueur *pjoueur);
void demande_tir(t_joueur *pjoueur,t_joueur *padvers);
void ia_placement_bateaux(t_joueur *ia_joueur);
void tir_switch(t_possibilite_tir *pposs, int taille, t_joueur *padvers, t_joueur *pjoueur);

#endif // PLAYER_H_INC
