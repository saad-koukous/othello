#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct playersothello
{
    char user_name[200];
    char password[25];
    int score[200];
    int nombre_match;
    struct playersothello *next;
}player;
typedef struct noir
{
    char x;
    int y;
    struct noir *next;
}noir;
typedef struct blanc
{
    char x;
    int y;
    struct blanc *next;
}blanc;
typedef struct meilleur_score
{
    char joueur[200];
    int score;
}meilleur_score;


//Créer et enregistrer les joueurs et leurs caractéristiques (nom et score) sur fichier
void ajouter_player(char user_name[200],char password[30]);
int verification_mot_de_passe(char player[200],char password[30]);
player* find_player(char* user_name);
void ajout_du_score(int score,char* user_name);
void ajout_fichier();
void scanner_fichier();
int PAGE_ACCEUIL(char player_1[200],char player_2[200],int hadi,int *niveau);


//hadchi dyal Permettre le chargement d’un jeu sauvegardé auparavant
void sauvegarder_game(char player_1[200],char player_2[200],char *p);
void charger_jeu(char player_1[200],char player_2[200],char *p);


//Jouer contre un non humain Avec des positions aléatoires,
int machine(char matt[8][8]);

//jouer contre machine avec position a recherche optimise
int  minimax (int profondeur,char matt[8][8],char BN);
int alphabeta(int profondeur, int alpha, int beta,char BN,char matt[8][8]);
int evaluation(char matt[8][8],char BN);

char changer_joueur(char BN);
void MATRICE(char *p);
void affichage_historique();
void AFFICHER_OTHELLIER(char *p,int *noir_1,int *blanc_2);
void CHOIX_CASE(char *x,int *y,char *p,char BN);
int est_vide(char *p,int x,int y);
int Dans_le_cadre(int x,int y);
int check(int les_cas[2],int x,int y,char BN,char matt[8][8]);
int jouable(char matt[8][8],int x, int y,char BN);
int JOUEUR_NON_BLOQUE(char matrices[8][8],char BN);
int LISTE_DES_COUP_POSSIBLE(char matt[8][8],char BN,int liste_de_CHOIX_CASE[64] );


//AFFICHER_OTHELLIER l’historique des mouvements effectués par les joueurs
void AjouterFin_B(char x,int y);
void AjouterFin_N(char x,int y);
void historique(char x,int y,char BN);

//hadchi dyal les 10 meilleur score
void swap(meilleur_score *xp, meilleur_score *yp);
void selectionSort(meilleur_score score[10]);
void dix_meilleur_score(char gagnant[200],int vainqueur ,meilleur_score score[10]);
void initialisation_score(meilleur_score score[10]);
void AFFICHER_MEILLEUR_SCORE(meilleur_score score[10]);
void sauvgarder_score(meilleur_score score[10]);

void main();