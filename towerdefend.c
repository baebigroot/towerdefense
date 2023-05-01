#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "towerdefend.h"
#include <time.h>
#include <math.h> //fonction round dans deplacerUnite


//typedef Tunite* ** TplateauJeu;

TplateauJeu AlloueTab2D(int largeur, int hauteur){
    TplateauJeu jeu;
    jeu = (Tunite***)malloc(sizeof(Tunite**)*largeur);
    for (int i=0;i<largeur;i++){
        jeu[i] = (Tunite**)malloc(sizeof(Tunite*)*hauteur);
    }
    return jeu;  //tab2D contenant des pointeurs
}
void initPlateauAvecNULL(TplateauJeu jeu,int largeur, int hauteur){
    for (int i=0;i<largeur;i++){
        for (int j=0;j<hauteur;j++){
            jeu[i][j] = NULL;
        }
    }

    //POUR LA DEMO D'AFFICHAGE UNIQUEMENT, A SUPPRIMER
    //(les tours ici ne sont pas liées aux listes des unités de vos joueurs)
    /*jeu[5][3]=creeTourSol(5,3);
    jeu[3][3]=creeTourAir(3,3);
    jeu[4][1]=creeTourRoi(4,1);
    jeu[4][15]=creeTourAir(4,15);
    jeu[5][17]=creeDragon(5,17);*/
    //FIN DEMO AFFICHAGE
}

/*
void ecritCheminVersleHaut  : permet d'initilaiser le tab chemin de distance cases (avec des coord x y) dans une direction, à partir d'un point x y donné

int **chemin  : tab de coordonnées x y du chemin
int *ichemin  : indice de la case du chemin d'où on part
int *xdepart, int *ydepart : valeur en x y de départ pouri la premiere case
int distance  : distance sur laquelle on va écrire des coordonnées dans le tab chemin
int *distanceMaxRestante : securité pour ne pas sortir de la plage d'indice de chemin
*/
void ecritCheminVersleHaut(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante){
    if ((*distanceMaxRestante - distance)>=0){
        int y;
        for (y=*ydepart;y>*ydepart-distance;y--){ //*ydepart = 19; *ydepart-distance=19-4; y=19
            chemin[*ichemin][X]= *xdepart;
            chemin[*ichemin][Y]= y;
            (*ichemin)++;
        }
        *ydepart=y;
    }
    else printf("erreur longueur chemin\n");
}
void ecritCheminVerslaDroite(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante){
    if ((*distanceMaxRestante - distance)>=0){
        int x;
        for (x=*xdepart;x<*xdepart+distance;x++){
            chemin[*ichemin][X]= x;
            chemin[*ichemin][Y]= *ydepart;
            (*ichemin)++;
        }
        *xdepart=x;
    }
    else printf("erreur longueur chemin\n");
}
void ecritCheminVerslaGauche(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante){
    if ((*distanceMaxRestante - distance)>=0){
        int x;
        for (x=*xdepart;x>*xdepart-distance;x--){
            chemin[*ichemin][X]= x;
            chemin[*ichemin][Y]= *ydepart;
            (*ichemin)++;
        }
        *xdepart=x;
    }
    else printf("erreur longueur chemin\n");
}

int **initChemin(){
    int **chemin = (int**)malloc(sizeof(int*)*NBCOORDPARCOURS);

    for (int j=0;j<NBCOORDPARCOURS;j++){
        chemin[j] = (int*)malloc(sizeof(int)*2);  //2 cases :indice 0 pour la coord en X, indice 1 pour la coord en Y
    }

    int ydepart = 18;  //19 on part du bas
    int xdepart = 5;  //5 = milieu de la fenetre de 11 de largeur (0-10)
    int i = 0;  //parcourt les i cases du chemin
    int distanceMaxRestante = NBCOORDPARCOURS;

    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 3, &distanceMaxRestante);
    ecritCheminVerslaDroite(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVerslaGauche(chemin, &i, &xdepart, &ydepart, 5, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVerslaDroite(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 3, &distanceMaxRestante);
    ecritCheminVerslaGauche(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 3, &distanceMaxRestante);

    return chemin;  //tab2D contenant des pointeurs
}

void afficheCoordonneesParcours(int **chemin, int nbcoord){
    printf("Liste coordonnees: ");
    for (int i=0; i<nbcoord; i++){
        printf("(%d, %d)",chemin[i][X], chemin[i][Y]);
    }
    printf("\nfin liste coordonnées\n");
}

void affichePlateauConsole(TplateauJeu jeu, int largeur, int hauteur){
    //pour un affichage sur la console, en relation avec enum TuniteDuJeu
    const char* InitialeUnite[7]={"s", "a", "r", "A", "C", "D", "G"};

    printf("\n");
    for (int j=0;j<hauteur;j++){
        for (int i=0;i<largeur;i++){
                // A ne pas donner aux etudiants
            if (jeu[i][j] != NULL){
                    printf("%s",InitialeUnite[jeu[i][j]->nom]);
            }
            else printf(" ");  //cad pas d'unité sur cette case
        }
        printf("\n");
    }
}

Tunite *creeTourSol(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tourSol;
    nouv->cibleAttaquable = sol;
    nouv->maposition = sol;
    nouv->pointsDeVie = 500;
    nouv->vitesseAttaque = 1.5;
    nouv->degats = 120;
    nouv->portee = 5;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}
Tunite *creeTourAir(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tourAir;
    nouv->cibleAttaquable = air;
    nouv->maposition = sol;
    nouv->pointsDeVie = 500;
    nouv->vitesseAttaque = 1.0;
    nouv->degats = 100;
    nouv->portee = 3;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}
Tunite *creeTourRoi(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tourRoi;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = sol;
    nouv->pointsDeVie = 800;
    nouv->vitesseAttaque = 1.2;
    nouv->degats = 180;
    nouv->portee = 4;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}

Tunite *creeDragon(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = dragon;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = air;
    nouv->pointsDeVie = 200;
    nouv->vitesseAttaque = 1.1;
    nouv->degats = 70; //70 et pas 180
    nouv->portee = 2;
    nouv->vitessedeplacement = 2;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    return nouv;
}
Tunite *creeArcher(int posx, int posy) {
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = archer;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = sol;
    nouv->pointsDeVie = 80;
    nouv->vitesseAttaque = 0.7;
    nouv->degats = 120;
    nouv->portee = 3;
    nouv->vitessedeplacement = 1;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    return nouv;


}
Tunite *creeChevalier(int posx, int posy) {
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = chevalier;
    nouv->cibleAttaquable = sol;
    nouv->maposition = sol;
    nouv->pointsDeVie = 400;
    nouv->vitesseAttaque = 1.5;
    nouv->degats = 250;
    nouv->portee = 1;
    nouv->vitessedeplacement = 2;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    return nouv;
}
Tunite *creeGargouille(int posx, int posy) {
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = gargouille;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = air;
    nouv->pointsDeVie = 80;
    nouv->vitesseAttaque = 0.6;
    nouv->degats = 90;
    nouv->portee = 1;
    nouv->vitessedeplacement = 3;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    return nouv;
}

bool tourRoiDetruite(TListePlayer player){
    return (player->pdata->pointsDeVie == 0 || player->pdata->pointsDeVie < 0);
}

bool listeVide(TListePlayer l){
    return (l==NULL);
}

int randomCaseTabParcours() {
    return rand() % 32 + 1; // rand() % (max_number + 1 - minimum_number) + minimum_number
    //not taken 0 (pas de case precedente) and 33 (pas de case suivante) into consideration
}

// Verifier si la valeur de telle position sur le Plateau du Jeu est nulle et que ses coordonnees n'appartient pas a TabParcours

int *verifyPlaceRoi(int** chemin, int randomCase, TplateauJeu jeu) {
    int *posRoi;
    posRoi = (int *)malloc(sizeof(int)*2);
    int posXTabParcours = chemin[randomCase][0];
    int posYTabParcours = chemin[randomCase][1];

    // la case precedente de randomCase dans TabParcours
    int posXcasePrec = chemin[randomCase - 1][0];
    int posYcasePrec = chemin[randomCase - 1][1];

    int posXcaseSuiv = chemin[randomCase + 1][0];
    int posYcaseSuiv = chemin[randomCase + 1][1];

    // La case a gauche
    if ((jeu[posXTabParcours - 1][posYTabParcours] ==  NULL)
        && ((posXTabParcours - 1 != posXcasePrec) || (posYTabParcours != posYcasePrec))
        && ((posXTabParcours - 1 != posXcaseSuiv) || (posYTabParcours != posYcaseSuiv))) {
            posRoi[0] = posXTabParcours - 1;
            posRoi[1] = posYTabParcours;
            return posRoi;
        }
    // La case a droite
    else if ((jeu[posXTabParcours + 1][posYTabParcours] ==  NULL)
        && ((posXTabParcours + 1 != posXcasePrec) || (posYTabParcours != posYcasePrec))
        && ((posXTabParcours + 1 != posXcaseSuiv) || (posYTabParcours != posYcaseSuiv))) {
            posRoi[0] = posXTabParcours + 1;
            posRoi[1] = posYTabParcours;
            return posRoi;
        }
    // La case en bas
    else if ((jeu[posXTabParcours][posYTabParcours + 1] ==  NULL)
        && ((posXTabParcours != posXcasePrec) || (posYTabParcours  + 1 != posYcasePrec))
        && ((posXTabParcours != posXcaseSuiv) || (posYTabParcours + 1 != posYcaseSuiv))) {
            posRoi[0] = posXTabParcours;
            posRoi[1] = posYTabParcours + 1;
            return posRoi;
        }
    // La case en haut
    else if ((jeu[posXTabParcours][posYTabParcours - 1] ==  NULL)
        && ((posXTabParcours != posXcasePrec) || (posYTabParcours - 1 != posYcasePrec))
        && ((posXTabParcours != posXcaseSuiv) || (posYTabParcours - 1 != posYcaseSuiv))) {
            posRoi[0] = posXTabParcours;
            posRoi[1] = posYTabParcours - 1;
            return posRoi;
        }
    //printf("posXcasePrec = %d, posYcasePrec = %d, posXcaseSuiv = %d, posYcaseSuiv = %d\n", posXcasePrec, posYcasePrec, posXcaseSuiv, posYcaseSuiv);
    return NULL;
}
Tunite *randomUniteRoi(int *caseTabParcours){
    if (caseTabParcours != NULL) {
        int randomTour = rand()%2;
        if (randomTour == 0)
            return creeTourAir(caseTabParcours[0], caseTabParcours[1]);
        else
            return creeTourSol(caseTabParcours[0], caseTabParcours[1]);
    }
    else return NULL;
}

Tunite *randomUniteHorde (int** chemin, TplateauJeu jeu) {
    int posXHorde = chemin[0][0];
    int posYHorde = chemin[0][1];
    //printf("x=%d, y=%d\n", posXHorde, posYHorde);
    if (jeu[posXHorde][posYHorde] == NULL) {
        int randomHorde = rand()% 4 + 3;
        if (randomHorde == 3){
            return creeArcher(posXHorde, posYHorde);
        }
        if (randomHorde == 4){
            return creeChevalier(posXHorde, posYHorde);
        }
        if (randomHorde == 5){
            return creeDragon(posXHorde, posYHorde);
        }
        if (randomHorde == 6){
            return creeGargouille(posXHorde, posYHorde);
        }
        else return NULL;
    }
    else return NULL;
}

void AjouterUnite(TListePlayer* player, Tunite* nouvelleUnite) {
    if (nouvelleUnite != NULL) {
        TListePlayer nouv = (TListePlayer)malloc(sizeof(struct T_cell));
        nouv->pdata = nouvelleUnite;
        nouv->suiv = NULL;
        if ((*player) == NULL) {
            // If the value of *player is null -> the list is empty, add the unit at the beginning
            *player = nouv;
        }
        else {
            int probabilite = rand() % 100;
            if ((*player)->pdata->nom == tourRoi) {
                // If the playerRoi is found, insert the unit after it
                if (probabilite >= 15 && probabilite <= 23) {
                    nouv->suiv = (*player)->suiv;
                    (*player)->suiv = nouv;
                }

            }
            else {
                // If the playerRoi is not found, add the unit at the beginning
                if (probabilite >= 5 && probabilite <= 90) {
                    nouv->suiv = (*player);
                    (*player) = nouv;
                }


            }
        }
    }
}

void PositionnePlayerOnPlateau(TListePlayer player, TplateauJeu jeu) {
    TListePlayer p = player;
    /*
    while (p != NULL) {
        Tunite *unite = p->pdata;
        int x = unite->posX;
        int y = unite->posY;
        printf("check x=%d, y=%d\n", x, y);
        jeu[x][y] = unite;
        p = p->suiv;
    }
    */

    // faire le lien entre le plateau avec que le 1er et 2eme element dans la liste
    for (int i = 0;i<2;i++) {
        if (p != NULL) {
            Tunite *unite = p->pdata;
            int x = unite->posX;
            int y = unite->posY;
            //printf("check x=%d, y=%d\n", x, y);
            jeu[x][y] = unite;
            p = p->suiv;
        }
    }

}


int calculDistance(int x, int y) {
    return abs(x) + abs(y);
}

TListePlayer quiEstAPortee(TplateauJeu jeu, Tunite *UniteAttaquante) {
    TListePlayer listeRes = NULL;
    //printf("The one who holds the gun: %d\n", UniteAttaquante->nom);
    // si UniteAttaquante appartient a playerHorde
    if (UniteAttaquante->nom == archer || UniteAttaquante->nom == chevalier || UniteAttaquante->nom == dragon || UniteAttaquante->nom == gargouille) {
        int posXroi = 4;
        int posYroi = 1;
        if (calculDistance(UniteAttaquante->posX - posXroi, UniteAttaquante->posY - posYroi) <= UniteAttaquante->portee) {
            AjouterUnite(&listeRes, creeTourRoi(4,1));
        }
    }


    // si UniteAttaquante appartient a playerRoi
     else {
        //calculer toutes les cases valides
        //les cases au centre de la zone valide

        int xCenter = UniteAttaquante->posX;
        int yCenter = UniteAttaquante->posY;
        //la distance maximale que l'unite peut atteindre
        int maxDistance = UniteAttaquante->portee;
        int **chemin = initChemin();
        //parcourir toutes les cases valides dans la zone predefinie
        for (int i = xCenter-maxDistance; i <= xCenter+maxDistance; i++) {
            //if (i != xCenter) { // verifie que la case n'est pas celle d'UniteAttaquante
                for (int j = yCenter-maxDistance; j <= yCenter+maxDistance; j++) {
                    if (j != yCenter && i != xCenter) { // verifie que la case n'est pas celle d'UniteAttaquante
                        int distance = calculDistance(i-xCenter,j-yCenter); //calculer la distance entre UniteAttaquante et la case a la position (i, j)
                        //verifier s'il la case appartient a tabParcours
                        for (int k=0; k<NBCOORDPARCOURS; k++){
                            if (chemin[k][0] == i && chemin[k][1] == j){
                                if (distance <= maxDistance) { //si la distance < la portee d'UniteAttaquante
                                    Tunite *uniteCourante = (Tunite*)malloc(sizeof(Tunite));
                                    uniteCourante = jeu[i][j];
                                    if ( uniteCourante != NULL) { //verifier s'il y a une unite
                                        TuniteDuJeu nomUniteCourante = uniteCourante->nom;
                                        if (UniteAttaquante->nom == tourAir) {
                                            if (nomUniteCourante == dragon) {
                                            AjouterUnite(&listeRes, creeDragon(i,j));
                                            }
                                            else if (nomUniteCourante == gargouille) {
                                                AjouterUnite(&listeRes, creeGargouille(i,j));
                                            }
                                        }
                                        else if (UniteAttaquante->nom == tourSol) {
                                            if (nomUniteCourante == chevalier) {
                                            AjouterUnite(&listeRes, creeChevalier(i,j));
                                            }
                                            else if (nomUniteCourante == archer) {
                                                AjouterUnite(&listeRes, creeArcher(i,j));
                                            }
                                        }
                                        else if (UniteAttaquante->nom == tourRoi) {

                                            if (nomUniteCourante == chevalier) {
                                            AjouterUnite(&listeRes, creeChevalier(i,j));
                                            }
                                            else if (nomUniteCourante == archer) {
                                                AjouterUnite(&listeRes, creeArcher(i,j));
                                            }
                                            else if (nomUniteCourante == dragon) {
                                            AjouterUnite(&listeRes, creeDragon(i,j));
                                            }
                                            else if (nomUniteCourante == gargouille) {
                                                AjouterUnite(&listeRes, creeGargouille(i,j));
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                }
            //}

        }
    }

    return listeRes;
}

void selectionSort(TListePlayer *listeUnites) {
    TListePlayer current = *listeUnites;
    while (current != NULL)
    {
        TListePlayer min = current;
        TListePlayer ptr = current->suiv;
        while (ptr != NULL)
        {
            if ((ptr->pdata->pointsDeVie) < (min->pdata->pointsDeVie))
            {
                min = ptr;
            }
            ptr = ptr->suiv;
        }
        // Swap data of current node and min node
        Tunite *temp = current->pdata;
        current->pdata = min->pdata;
        min->pdata = temp;
        current = current->suiv;
    }
}

void combat(SDL_Surface *surface , Tunite * UniteAttaquante, Tunite * UniteCible) {
    if (UniteCible != NULL) {
        if (UniteAttaquante->peutAttaquer == 1) {
            dessineAttaque(surface,UniteAttaquante,UniteCible);
            UniteCible->pointsDeVie = UniteCible->pointsDeVie - UniteAttaquante->degats;
            UniteAttaquante->peutAttaquer = 0;

        }
    }
    //printf("combat: unit %d posx = %d, posy = %d pdv = %d\n", UniteCible->nom, UniteCible->posX, UniteCible->posY, UniteCible->pointsDeVie);
}

void remettrePeutAttaquer(TListePlayer *player) {
    TListePlayer cur = *player;
    while (cur != NULL) {

        if (cur->pdata->peutAttaquer == 0) {

            cur->pdata->peutAttaquer = 1;

        }
        cur = cur->suiv;
    }
}

void MAJplayer(TListePlayer *player, Tunite* UniteMAJ) {
    TListePlayer cur = *player;
    // parcourir toute la liste de player pour trouver uniteMAJ - l'unite ayant besoin de mettre a jour
    while (cur != NULL) {
        // comparer la position de chaque unite dans la liste avec l'uniteMAJ
        if (cur->pdata->posX == UniteMAJ->posX && cur->pdata->posY == UniteMAJ->posY) {
            // si on l'a trouve, mettez a jour le points de vie et sortez de la boucle
            cur->pdata->pointsDeVie = UniteMAJ->pointsDeVie;
            break;
        }
        cur = cur->suiv;
    }
}

void MAJlistePortee(Tunite *UniteMAJ, TListePlayer *player) {
    TListePlayer cur = *player;
    // parcourir toute la liste de player pour trouver uniteMAJ - l'unite ayant besoin de mettre a jour
    while (cur != NULL) {
        // comparer la position de chaque unite dans la liste avec l'uniteMAJ
        if (cur->pdata->posX == UniteMAJ->posX && cur->pdata->posY == UniteMAJ->posY) {
            // si on l'a trouve, mettez a jour le points de vie et sortez de la boucle
            UniteMAJ->pointsDeVie = cur->pdata->pointsDeVie;
            break;
        }
        cur = cur->suiv;
    }
}

void supprimerUnite(TListePlayer *player, Tunite *UniteDetruite, TplateauJeu jeu) {
    TListePlayer cur = *player;
    TListePlayer prec = NULL;

    while (cur != NULL && cur->pdata->nom != UniteDetruite->nom) {
        prec = cur;
        cur = cur->suiv;
    }

    if (cur == NULL) {
        return;
    }
    if (prec == NULL) {
        *player = cur->suiv;
    }
    else {
        prec->suiv = cur->suiv;
    }
    jeu[cur->pdata->posX][cur->pdata->posY] = NULL;
    free(cur);

}

void printList(TListePlayer player) {
    TListePlayer current = player;
    int count = 0;
    while (current != NULL) {
        printf("unit %d: %d points de vie, posx = %d, posy = %d\n", (int)current->pdata->nom, (int)current->pdata->pointsDeVie, current->pdata->posX, current->pdata->posY);
        current = current->suiv;
        count++;
    }
    printf("List has %d elements\n", count);

}


void deplacerUnite(TplateauJeu jeu, int** chemin, Tunite* UniteADeplacer) {
    int posXunite = UniteADeplacer->posX;
    int posYunite = UniteADeplacer->posY;
    float vitesse = UniteADeplacer->vitessedeplacement;

    //parcourir le chemin pour trouver la position d'UniteADeplacer
    for (int i=0; i< NBCOORDPARCOURS; i++) {
        if (chemin[i][0] == posXunite && chemin[i][1] == posYunite) { // Ah je l'ai trouvé !!
            // Vérifiez si la case vers laquelle l'unité est censée se déplacer est libre (sur le plateau du Jeu)
            if ((int)round(i+vitesse) < NBCOORDPARCOURS) { // verifier que la case ne depasse pas 34
                int posXtest = chemin[(int)round(i+vitesse)][0];
                int posYtest = chemin[(int)round(i+vitesse)][1];
                if (jeu[posXtest][posYtest] == NULL) {
                    //changer les coordonnees d'UniteADeplacer
                    UniteADeplacer->posX = posXtest;
                    UniteADeplacer->posY = posYtest;

                    //Move UniteADeplacer to its new position on the game board
                    jeu[posXtest][posYtest] = UniteADeplacer;

                    // Remove UniteADeplacer from its old position on the game board
                    jeu[posXunite][posYunite] = NULL;
                    break;
                }
            }
            /*
            else {
                jeu[posXunite+1][posYunite+1] = UniteADeplacer;
                jeu[posXunite][posYunite] = NULL;
            }
            */


        }
    }
}
