#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "les_foctions.h"
 
blanc *HEAD_B=NULL;
noir *HEAD_N=NULL;
player *HEAD=NULL;
/* pour le troisieme livrable nous avons pu ajouter La sécurité des accès à l’aide de mots de passe,  */
//ces trois fonctions sert a desallouer l'espace memoire allouer par une liste chaine
void delet_liste_player()
{
    player *h=HEAD;
    player *suivant;
    while (h!=NULL)
    {
        suivant=h->next;
        free(h);
        h=suivant;
    }
    HEAD=NULL;
}
void delet_liste_noir()
{
    noir *h=HEAD_N;
    noir *suivant;
    while (h!=NULL)
    {
        suivant=h->next;
        free(h);
        h=suivant;
    }
    HEAD_N=NULL;
}
void delet_liste_blanc()
{
    blanc *h=HEAD_B;
    blanc *suivant;
    while (h!=NULL)
    {
        suivant=h->next;
        free(h);
        h=suivant;
    }
    HEAD_B=NULL;
}
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */ 
//sans cet fonction si on veut teste le nombre de match jouer par un joueur qui n'a encore pas jouer un match on trouve une valeur quelconaue
//d'ou la necessite de l'initialiser au cas de la premier utilisation 
void initialiser_player(char user_name[200])
{
    player* adrs=find_player(user_name);
    if (adrs!=NULL)  adrs->nombre_match=0;
}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */ 
//cette fonction sert a ajouter une variable de typre player ou specialement son user_name et son password
//dans une liste chaine 
void ajouter_player(char user_name[200],char password[30])
{
    player *new_player;//declaration de variable de type player
    new_player = (player*)malloc(sizeof(player));// l'allocation dinamique et necessaire vu qu'on utilise des liste chaine
    if (new_player==NULL) exit(0);
    if(HEAD == NULL){
        HEAD = new_player;
        strcpy(new_player->user_name, user_name);
        strcpy(new_player->password, password);
        new_player->next = NULL;
        return;
    }
    strcpy(new_player->user_name, user_name);
    strcpy(new_player->password, password);
    new_player->next = NULL;
    player* p=HEAD;
    while(p->next!=NULL) p=p->next;
    p->next=new_player;
}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */ 
// une fonction qui cherche dans la liste chaine des joueur le joueur pris en argument et renvoie 
//l'adresse ou se trouve ou NULL s il n'existe pas
player* find_player(char* user_name)
{ 
    player *h = HEAD;
    while (h != NULL)
    {
        if(strcmp(h->user_name, user_name) == 0) return h;
        h = h->next;
    }
    return NULL;
}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */ 
//en suivant les memes principe des fonction ci dessus l'ajout du score
void ajout_du_score(int score,char* user_name)
{
    player* adrs=find_player(user_name);
    if (adrs!=NULL)
    {   
        int nbr=adrs->nombre_match;
        adrs->score[nbr]=score;
        nbr++;
        adrs->nombre_match=nbr;
    }
}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */ 
//a la fin de l'execution le contenue de la liste chaine des joueur ne doit pas etre perdus alors
//on doit stocke ces informations dans un fichier
void ajout_fichier()
{   FILE *file;
    file = fopen("database.txt","w");
    if(file == NULL)
    {
        //printf("Error opening file\n");
        return;
    }
    player *h = HEAD;
    int nbr=0;
    while(h != NULL)
    {            
        fprintf(file,"%s %d %s\n",h->user_name,h->nombre_match,h->password);
        nbr=h->nombre_match;
        for (int i = 0; i < nbr; i++)
        {
            fprintf(file,"%d\n",h->score[nbr-1]);
        }
        h=h->next;
        if (h==NULL) break;
    }
    fclose(file);
}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */ 
//au debut d'un jeu le contenu de fichier database doit etre implementer dans la liste chaine des joueurs
void scanner_fichier()
{   FILE *file;
    file = fopen("database.txt","r");
    if(file == NULL)
    {
        //printf("Error opening file\n");
        return;
    }
    int nbr;
    player *h ;
    h = (player*)malloc(sizeof(player));
    if(h==NULL) exit(0);
    HEAD=h;
    while(fscanf(file,"%s %d %s",h->user_name,&(h->nombre_match),h->password)!= EOF)
    {   
        nbr=h->nombre_match;
        if (nbr>0)
        {
            for (int i = 0; i < nbr; i++)
            {
                fscanf(file,"%d\n",&h->score[i]);
            }
        }
        h->next = (player*)malloc(sizeof(player));
        h=h->next;
    }
    
    fclose(file);
    
}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */ 
//cette fonction sert simplement a verifier si le mot de passe saisie par l'utilisateur est juste ou pas
int verification_mot_de_passe(char player_[200],char password[30])
{
    player *adresse;
    adresse=find_player(player_);
    if (!strcmp(password,adresse->password)) return 1;
    return 0;
    
}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */ 
//cette fonction regroupe plusieur fonction afin d'assurer la premier phase du jou 
//"s'identifier" ou "s'inscrire" choisir le mode de jeu ou choisir la difficulter de l'adversaire robot
int PAGE_ACCEUIL(char player_1[200],char player_2[200],int hadi,int *niveau)
{   
    int xxx,i=1;
    char nom_1[200],nom_2[200],password_1[30],password_2[30];
    char matrices_test[8][8];
    char *p=&matrices_test[0][0];
    do 
    {
        printf("\nplayer_1 donnez votre nom : ");
        scanf("%s",player_1);
        if (find_player(player_1)==NULL)
        {   
            printf("Entrez un mot de passe : ");
            scanf("%s",password_1);
            ajouter_player(player_1,password_1);
            initialiser_player(player_1);
            break;
        }else
        {
            printf("Entrez votre mot de passe : ");
            scanf("%s",password_1);
            while (verification_mot_de_passe(player_1,password_1)!=1&&i<3)
            {   
                fflush(stdin);
                printf("\t\tmot de passe incorrect réessayez une %d fois : ",++i);
                scanf("%s",password_1);
            }
            if (verification_mot_de_passe(player_1,password_1)) break;
        }
    }while (1);//do while car si un utilisateur saisie un mdp errone 3 fois il va resseyer avec un autre user name
    if(!hadi)
    {
        do 
        {
            do 
            {
                printf("\nplayer_2 donnez votre nom : ");
                scanf("%s",player_2);
                fflush(stdin);
            }while (!strcmp(player_1,player_2));//pour s'assurer que l'utilisateur ne saisi pas le meme user name du player_1
            if (find_player(player_2)==NULL) 
            {
                printf("Entrez un mot de passe : ");
                scanf("%s",password_2);
                ajouter_player(player_2,password_2);
                initialiser_player(player_2);
                break;
            }else
            {   
                i=1;
                printf("Entrez votre mot de passe : ");
                scanf("%s",password_2);
                while (verification_mot_de_passe(player_2,password_2)!=1&&i<3)
                {
                    fflush(stdin);
                    printf("\tmot de passe incorrect réessayez une %d fois : ",++i);
                    scanf("%s",password_1);
                }
                if (verification_mot_de_passe(player_1,password_1)) break;
            }
        }while (1);
        charger_jeu(nom_1,nom_2,p);
        if (!strcmp(player_1,nom_1)||!strcmp(player_1,nom_2)||!strcmp(player_2,nom_1)||!strcmp(player_2,nom_2))
        {
            printf("\nvoulez vous charger le jeu que vous avez enregistrer\n----------- si oui tapez 1 sinon tapez 0 -----------\n");
            scanf("%d",&xxx);
            if (xxx==1) return 1;
        }
    }else
    {
        printf("\nquelle niveau de machine voulez vous jouez contre\n");
        printf("facile press   ----------------> 0\n");
        printf("moyen press    ----------------> 1\n");
        printf("difficile press----------------> 2\n");
        printf("-------------------------------> ");
        scanf("%i",niveau);           
    }
    fflush(stdin);
    return 0;
}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */ 
//c'est la fonction qui enregistre l'Othellier dans un fichier 
void sauvegarder_game(char player_1[200],char player_2[200],char *p)
{
    FILE *file;
    file = fopen("sauvegarder_game.txt","w");
    if(file == NULL)
    {
        //printf("Error opening file\n");
        return;
    }
    fprintf(file,"%s %s\n",player_1,player_2);
    for (int i = 0; i < 64; i++) fprintf(file,"%c",*(p+i));
    fclose(file);
} 

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
//charge les cordonnes enregistrer dans le fichier sauvegarder_game
void charger_jeu(char player_1[200],char player_2[200],char *p)
{
    FILE *file;
    file = fopen("sauvegarder_game.txt","r");
    if(file == NULL)
    {
        //printf("Error opening file\n");
        return;
    }
    if (fscanf(file,"%s %s",player_1,player_2)!=EOF)
    {
        //fscanf(file,"%s %s",player_1,player_2);
        //fscanf(file,"%c",p);
        for (int i = 0; i < 64; i++) fscanf(file,"%c",p+i*sizeof(char));
    }
    fclose(file);
}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
 
char changer_joueur(char BN)
{
    if (BN=='B') return 'N';
    else return 'B';
}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */  
//initialise la matrice du jeu
void MATRICE(char *p)
{    
    for (int i = 0; i < 64; i++) *(p+i)= ',';
    *(p+27)='B';
    *(p+36)='B';
    *(p+28)='N';
    *(p+35)='N';
}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */ 

void affichage_historique()
{   
    if (HEAD_N==NULL && HEAD_B==NULL) return;
    blanc *BB=HEAD_B;
    noir *NN=HEAD_N;
    while (BB!=NULL || NN!=NULL)
    {
        if (BB!=NULL)
        {
            printf("  %c%d\t\t\t",BB->x,BB->y);
            BB=BB->next;
        }else printf("    \t\t\t");
        if (NN!=NULL)
        {
            printf("  %c%d\n",NN->x,NN->y);
            NN=NN->next;
        }
        else printf("\n");
    }
    
}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */ 

void AFFICHER_OTHELLIER(char *p,int *noir_1,int *blanc_2)
{   int k=0,j=0;
    *noir_1=0;
    *blanc_2=0;
    printf("\n\n||||||||||-----------------OTHELLO-----------------||||||||||\n");
    printf("-------------si vous voulez recommencez entrer 'r9'-------------\n");
    printf("----------si vous voulez enregistrezle jeu entrer 's9'----------\n");
    printf("  a    b    c    d    e    f    g    h\n");
    for (int i = 0; i < 64;i++)
    {  k++;
        printf(" %2c |",*(p+i));
        if (k==8)
        {
            printf("\n|___|____|____|____|____|____|____|____|%i\n",j++);
            k=0;
        }
        if(*(p+i)=='B')(*blanc_2)++;
        else if (*(p+i)=='N')(*noir_1)++;
    }
    printf("Blanc : %i \t\t noir : %i\n",*blanc_2,*noir_1);
    affichage_historique();
}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */  

void CHOIX_CASE(char *x,int *y,char *p,char BN)//securise le choix de la case a jouer 
{  
    *x='x';
    *y='y';
    while (*x<'a' || *x>'h' || *y<0 || *y>7 )
    {
        printf("\tchoose where to put  %c : ",BN);
        scanf("%c%i",x,y);
        if (*x=='r'&&*y==9)return;
        if (*x=='s'&&*y==9)return;
        fflush(stdin);
    }
    //en_tete();
}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */  

int est_vide(char *p,int x,int y)//s'assurer si une case est vide ou pas
{   
    int z = (x-'a') + y*8;
    if (*(p+z)==',') return 1;
    else return 0;
}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */  

int Dans_le_cadre(int x,int y)//s'assurer si un choix est dans le cadre ou pas
{
    if(x<8 && x>=0 && y<8 && y>=0) return 1;
    else return 0;
}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */ 

int check(int les_cas[2],int x,int y,char BN,char matt[8][8])
{   x=x+les_cas[0];
    y=y+les_cas[1];
    while (Dans_le_cadre(x,y)&&matt[x][y]==changer_joueur(BN)){x+=les_cas[0];y+=les_cas[1];}
    if (Dans_le_cadre(x,y)&&matt[x][y]==BN&&matt[x-les_cas[0]][y-les_cas[1]]==changer_joueur(BN))return 1;
    return 0;
    
}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */ 
 
int jouable(char matt[8][8],int x, int y,char BN)
{
    int bien=0;
    int xx,yy;
    int les_cas[8][2]={{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    for (int i = 0; i < 8; i++)
    {
        xx=x,yy=y;
        if(check(les_cas[i],x,y,BN,matt))
        {
            while (matt[xx+les_cas[i][0]][yy+les_cas[i][1]]==changer_joueur(BN))
            {
                matt[xx+les_cas[i][0]][yy+les_cas[i][1]]=BN;
                xx+=les_cas[i][0];
                yy+=les_cas[i][1];
                bien++;
            }
            
        }
    }
    return bien;
}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
//teste si le joueur BN est blocke ou pas
int JOUEUR_NON_BLOQUE(char matrices[8][8],char BN)
{   char copie_matrices[8][8];

    for (int i = 0; i < 8; i++)
    {   for (int j = 0; j < 8; j++) 
        {
            for (int k = 0; k < 8; k++)
                {for (int l = 0; l < 8; l++) copie_matrices[k][l]=matrices[k][l];}
            if (copie_matrices[i][j]=='B'||copie_matrices[i][j]=='N')continue;
            if (jouable(copie_matrices,i,j,BN))return 1;
        }
    }
    return 0;    

}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */ 

void AjouterFin_B(char x,int y)
{
    blanc *nouveau=malloc(sizeof(blanc));
    if(nouveau==NULL) exit(0);
    if (HEAD_B==NULL)
    {   HEAD_B=nouveau;
        nouveau->x=x;
        nouveau->y=y;
        nouveau->next=NULL;
        return;
    }
    nouveau->x=x;
    nouveau->y=y;
    nouveau->next=NULL;
    blanc* p = HEAD_B;
    while(p->next!=NULL) p = p->next;
     
    p->next=nouveau;
}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */

void AjouterFin_N(char x,int y)
{
    noir *nouveau=malloc(sizeof(noir));
    if(nouveau==NULL) exit(0);
    if (HEAD_N==NULL)
    {   HEAD_N=nouveau;
        nouveau->x=x;
        nouveau->y=y;
        nouveau->next=NULL;
        return;
    }    
 
    nouveau->x=x;
    nouveau->y=y;
    nouveau->next=NULL;
 
    noir* p = HEAD_N;
    while(p->next!=NULL) p = p->next;
     
    p->next=nouveau;
}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */

void historique(char x,int y,char BN)
{   
    if (BN=='N') AjouterFin_N(x,y);
    else AjouterFin_B(x,y);    
}

/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */ 

void swap(meilleur_score *xp, meilleur_score *yp) 
{ 
    meilleur_score temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
}
//un simple tri par insertion 
void selectionSort(meilleur_score score[10]) 
{ 
    int i, j, min_idx;  
    for (i = 0; i < 9; i++) 
    { 
        min_idx = i; 
        for (j = i+1; j < 10; j++) 
          if (score[j].score < score[min_idx].score) 
            min_idx = j; 
        swap(&score[min_idx], &score[i]); 
    }
}
void dix_meilleur_score(char gagnant[200],int vainqueur ,meilleur_score score[10])
{
    selectionSort(score);
    if (score[0].score<vainqueur) 
    {
        score[0].score=vainqueur;
        strcpy(score[0].joueur,gagnant);
        selectionSort(score);
    }
}
void initialisation_score(meilleur_score score[10])
{
    int nbr=0;
    FILE *file;
    file = fopen("meilleur_score.txt","r");
    if(file == NULL)
    {
        //printf("Error opening file\n");
        for (int i = nbr; i < 10; i++) score[i].score=0 ;
        return;
    }
    while(fscanf(file,"%s %d\n",score[nbr].joueur,&(score[nbr].score))!= EOF)  nbr++;
    for (int i = nbr; i < 10; i++) score[i].score=0 ;
    selectionSort(score);
    fclose(file);
}
void AFFICHER_MEILLEUR_SCORE(meilleur_score score[10])
{   int i=9;
    printf("\t\tles meilleurs scores sont:\n");
    while (score[i].score!=0 && i>0)
    {
        printf("\t\t\t%s : %d\n",score[i].joueur,score[i].score);
        i--;
    }
}
void sauvgarder_score(meilleur_score score[10])
{
    FILE *file;
    file = fopen("meilleur_score.txt","w");
    if(file == NULL)
    {
        //printf("Error opening file\n");
        return;
    }
    for (int i = 9; i > 0; i--) 
        {
            if ( score[i].score==0 ) break;
            fprintf(file,"%s %d\n",score[i].joueur,score[i].score);
        }
    fclose(file);
}
void en_tete()
{
    for (int i = 0; i < 20; i++)printf("\n");
    
    printf("||||||||||-----------------OTHELLO-----------------||||||||||\n");
    printf("-------------si vous voulez recommencez entrer 'r9'-------------\n");
    printf("----------si vous voulez enregistrezle jeu entrer 's9'----------\n");

}
// genere une liste des coups possible dans l'Othellier 
int LISTE_DES_COUP_POSSIBLE(char matt[8][8],char BN,int liste_de_CHOIX_CASE[64] )
{    
    char copie_matrices[8][8];
    int nbr=0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            for (int k = 0; k < 8; k++)
                {for (int l = 0; l < 8; l++) copie_matrices[k][l]=matt[k][l];}
            if (jouable(copie_matrices,i,j,'B')&&matt[i][j]==',')
            {
                liste_de_CHOIX_CASE[nbr]=i*8+j;
                nbr++;
            }
        }
    }
    return nbr;
}
//cette fonction est utile pour les fct minmax est alphabeta qui calcule le gain d'un joueur en prenant compte de la matrice de poids
int evaluation(char matt[8][8],char BN)
{
    int POIDS[8][8] = {{4 ,-3, 2, 2, 2, 2,-3, 4},
                       {-3,-4,-1,-1,-1,-1,-4,-3},
                       { 2,-1, 1, 0, 0, 1,-1, 2},
                       { 2,-1, 0, 1, 1, 0,-1, 2},
                       { 2,-1, 1, 0, 0, 1-1, 2},
                       {2,-1, 0, 1, 1, 0,-1, 2},
                       {-3,-4,-1,-1,-1,-1,-4,-3},
                       { 4,-3, 2, 2, 2, 2,-3, 4}};
    int white=0,black=0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (matt[i][j]=='B') white+=POIDS[i][j]+1;
            if (matt[i][j]=='N') black+=POIDS[i][j]+1;
        }
    }
    if (BN='B') return white-black;
    return black-white;
}
int alphabeta(int profondeur, int alpha, int beta,char BN,char matt[8][8])
{
    if (!JOUEUR_NON_BLOQUE(matt,'B')&&!JOUEUR_NON_BLOQUE(matt,'N') || profondeur == 0) return evaluation(matt,BN);
    int meilleur_coup;
    char copie_matrices[8][8],BNN;
    int listejouable[64];
    int nbr=LISTE_DES_COUP_POSSIBLE(matt,BN,listejouable);
    for (int i = 0; i < nbr; i++)
        {
            for (int k = 0; k < 8; k++)
                {for (int l = 0; l < 8; l++) copie_matrices[k][l]=matt[k][l];}
            int x=listejouable[i]/8;
            int y=listejouable[i]%8;
            jouable(copie_matrices,x,y,BN);
            copie_matrices[x][y]=BN;
            BNN=changer_joueur(BN);
            int score ;
            if (JOUEUR_NON_BLOQUE(matt,BNN)) score = -alphabeta(profondeur - 1, -beta, -alpha,BNN,copie_matrices);
            else score = alphabeta(profondeur - 1, alpha, beta ,BN,copie_matrices);
            if (score >= alpha)
            {
                alpha = score ;
                meilleur_coup = listejouable[i] ;
                if (alpha >= beta)
                break;
            }
        }
        return meilleur_coup;
}

int  minimax (int profondeur,char matt[8][8],char BN)
{
    if (!JOUEUR_NON_BLOQUE(matt,'B')&&!JOUEUR_NON_BLOQUE(matt,'N') || profondeur == 0) return evaluation(matt,BN);
    if (!JOUEUR_NON_BLOQUE(matt,BN)) BN=changer_joueur(BN);
    int meilleur_score;
    char copie_matrices[8][8];
    int meilleur_coup;
    int listejouable[64];
    int nbr=LISTE_DES_COUP_POSSIBLE(matt,BN,listejouable);
    if (BN == 'B') 
    { 
        meilleur_score = -100;
        for (int i = 0; i < nbr; i++)
        {
            for (int k = 0; k < 8; k++)
                {for (int l = 0; l < 8; l++) copie_matrices[k][l]=matt[k][l];}
            int x=listejouable[i]/8;
            int y=listejouable[i]%8;
            jouable(copie_matrices,x,y,'B');
            copie_matrices[x][y]='B';
            int score = minimax(profondeur - 1,copie_matrices,'N');
            if (score > meilleur_score) 
            {
                meilleur_score = score;
                meilleur_coup = listejouable[i] ;
            }
        }
    }
    else 
    { 
        meilleur_coup = 100;
        for (int i = 0; i < nbr; i++) 
        {
            for (int k = 0; k < 8; k++)
                {for (int l = 0; l < 8; l++) copie_matrices[k][l]=matt[k][l];}
            int x=listejouable[i]/8;
            int y=listejouable[i]%8;
            jouable(copie_matrices,x,y,'N');
            copie_matrices[x][y]='N';
            int score = minimax(profondeur - 1,copie_matrices,'B');
            if (score < meilleur_score) 
            {
                meilleur_score = score;
                meilleur_coup =listejouable[i] ;
            }
        }
    }
    return meilleur_coup ;
}

int machine(char matt[8][8])
{
    char copie_matrices[8][8];
    int liste_de_CHOIX_CASE[64],nbr=0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            
            for (int k = 0; k < 8; k++)
                {for (int l = 0; l < 8; l++) copie_matrices[k][l]=matt[k][l];}
            if (jouable(copie_matrices,i,j,'B')&&matt[i][j]==',')
            {
                liste_de_CHOIX_CASE[nbr]=i*8+j;
                nbr++;
            }
        }
    }
    int num;
    num = (rand() % (nbr  )); //genere un nombre aleatoire compris entre 0 et nbr
    return liste_de_CHOIX_CASE[num];
}
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */ 

void main()
{   
    char BN='N';
    meilleur_score score[10];
    int k=0,colonne,mode,noir_1=0,blanc_2=0,jeu=1,machine_CHOIX_CASE,x,y,mode_1,niveau;
    char ligne;
    char player_1[200],player_2[200];
    char matrices[8][8];
    char *p=&matrices[0][0];
    srand(time(0));
    scanner_fichier();
    initialisation_score(score);
    printf("||||||||||-----------------OTHELLO-----------------||||||||||\n");
    AFFICHER_MEILLEUR_SCORE(score);
    printf("----------pour jouer player_1 VS player_2 press '0'----------\n");
    printf("------------pour jouer player VS machine press '1'-----------\n");
    do{scanf("%i",&mode_1);
    fflush(stdin);}while (mode_1!=0&&mode_1!=1);
    mode=PAGE_ACCEUIL(player_1,player_2,mode_1,&niveau);
    if(mode) charger_jeu(player_1,player_2,p);
    else MATRICE(p);
    AFFICHER_OTHELLIER(p,&noir_1,&blanc_2);
    while (JOUEUR_NON_BLOQUE(matrices,'B')&&JOUEUR_NON_BLOQUE(matrices,'N'))
    {   
        if (JOUEUR_NON_BLOQUE(matrices,BN))      
        {
            if (BN=='N'||!mode_1)
            {    CHOIX_CASE(&ligne,&colonne,p,BN);
                if (ligne=='r'&&colonne==9)
                {
                    printf("\nle jeu va recommencer\n");
                    delet_liste_blanc();
                    delet_liste_noir();
                    MATRICE(p);
                    AFFICHER_OTHELLIER(p,&noir_1,&blanc_2);
                    continue;
                }else if (ligne=='s'&&colonne==9)
                {   
                    sauvegarder_game(player_1,player_2,p);
                    jeu=0;
                    break;
                }           
                int z=((int)ligne-97);
                if (est_vide(p,ligne,colonne)&&(jouable(matrices,colonne,z,BN)>0))
                {   
                    jouable(matrices,colonne,z,BN);
                    matrices[colonne][z]=BN;
                    historique(ligne,colonne,BN);
                    BN=changer_joueur(BN);
                }
                else 
                    {
                    printf("\n-----c'est pas le bon CHOIX_CASE reessaye -----\n");
                    continue;
                    }
            }else 
            {
                switch (niveau)
                {
                case 0:machine_CHOIX_CASE=machine(matrices);
                    break;
                case 1:machine_CHOIX_CASE=minimax(3,matrices,'B');
                    break;
                case 2:machine_CHOIX_CASE=alphabeta(5,-100,100,BN,matrices);
                
                default:
                    break;
                }
                x=machine_CHOIX_CASE/8;
                y=machine_CHOIX_CASE-x*8;
                jouable(matrices,x,y,BN);
                matrices[x][y]=BN;
                historique(y+97,x,BN);
                BN=changer_joueur(BN);
            }
        AFFICHER_OTHELLIER(p,&noir_1,&blanc_2); 
        }      
       else 
            {
            printf("le joueur %c est blocke",BN);
            BN=changer_joueur(BN);
            }
        
    }
    if(jeu)
    {
        if (noir_1>blanc_2)
        {
            printf("\nle vainqueur est %s avec un score de %d",player_1,noir_1);
            ajout_du_score(noir_1,player_1);
            dix_meilleur_score(player_1,noir_1,score);
        }
        else {
            printf("\nle vainqueur est %s avec un score de %d",player_2,blanc_2);
            ajout_du_score(blanc_2,player_2);
            dix_meilleur_score(player_2,blanc_2,score);
            }
    }
    sauvgarder_score(score);
    ajout_fichier();
    delet_liste_player();
    delet_liste_blanc();
    delet_liste_noir();
}