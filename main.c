//NOM pr�nom �tudiant 1 : Lovanomena RAFIDISON
//NOM pr�nom �tudiant 2 : Nhung NGUYEN

#include "SDL.h"
#include "maSDL.h"    //biblioth�que avec des fonctions d'affichage utilisant la SDL
#include "towerdefend.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



/*--------- Main ---------------------*/
int main(int argc, char* argv[])
{
    SDL_Window *pWindow;
    SDL_Init(SDL_INIT_VIDEO);

    pWindow = SDL_CreateWindow(
        "Appuyez sur ECHAP pour quitter, S/C ET D/V les gerer les sauvegardes",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        LARGEURJEU*40,
        HAUTEURJEU*40,
        SDL_WINDOW_SHOWN
    );

    //SDL_Renderer * renderer = SDL_CreateRenderer(pWindow, -1, 0);  //non utilis�, pour m�moire

    SDL_Surface* pWinSurf = SDL_GetWindowSurface(pWindow);  //le sprite qui couvre tout l'�cran
    SDL_Surface* pSpriteTourSol = SDL_LoadBMP("./data/TourSol.bmp");  //indice 0 dans tabSprite (via l'enum TuniteDuJeu)
    SDL_Surface* pSpriteTourAir = SDL_LoadBMP("./data/TourAir.bmp");  //indice 1 dans tabSprite (via l'enum TuniteDuJeu)
    SDL_Surface* pSpriteTourRoi = SDL_LoadBMP("./data/TourRoi.bmp"); //indice 2
    SDL_Surface* pSpriteArcher = SDL_LoadBMP("./data/Archer.bmp"); //indice 3
    SDL_Surface* pSpriteChevalier = SDL_LoadBMP("./data/Chevalier.bmp"); //indice 4
    SDL_Surface* pSpriteDragon = SDL_LoadBMP("./data/Dragon.bmp"); //indice 5
    SDL_Surface* pSpriteGargouille = SDL_LoadBMP("./data/Gargouille.bmp"); //indice 6
    SDL_Surface* pSpriteEau = SDL_LoadBMP("./data/Eau.bmp"); //indice 7  Ne figure pas dans l'enum TuniteDuJeu
    SDL_Surface* pSpriteHerbe = SDL_LoadBMP("./data/Herbe.bmp"); //indice 8 idem
    SDL_Surface* pSpritePont = SDL_LoadBMP("./data/Pont.bmp"); //indice 9 idem
    SDL_Surface* pSpriteTerre = SDL_LoadBMP("./data/Terre.bmp"); //indice 10 idem

    // ASTUCE : on stocke le sprite d'une unit� � l'indice de son nom dans le type enum TuniteDuJeu, dans le tableau TabSprite
    // SAUF pour l'Eau, l''herbe et le pont qui apparaitront en l absence d'unit� (NULL dans le plateau) et en foction de certains indices x,y d�finissant le chemin central
    SDL_Surface* TabSprite[11]={pSpriteTourSol,pSpriteTourAir,pSpriteTourRoi,pSpriteArcher,pSpriteChevalier,pSpriteDragon,pSpriteGargouille,pSpriteEau,pSpriteHerbe,pSpritePont,pSpriteTerre};

    int** tabParcours=initChemin();  //tabParcours est un tableau de NBCOORDPARCOURS cases, chacune contenant un tableau � 2 cases (indice 0 pour X, indice 1 pour Y)

    if ( pSpriteTourSol )  //si le permier sprite a bien �t� charg�, on suppose que les autres aussi
    {
        TplateauJeu jeu = AlloueTab2D(LARGEURJEU,HAUTEURJEU);
        initPlateauAvecNULL(jeu,LARGEURJEU,HAUTEURJEU);
        affichePlateauConsole(jeu,LARGEURJEU,HAUTEURJEU);



        prepareAllSpriteDuJeu(jeu,tabParcours,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf);
        maj_fenetre(pWindow);


        /**********************************************************************/
        /*                                                                    */
        /*              DEFINISSEZ/INITIALISER ICI VOS VARIABLES              */

        TListePlayer playerRoi = (TListePlayer)malloc(sizeof(struct T_cell));
        playerRoi->suiv = NULL;
        playerRoi->pdata = creeTourRoi(4,1);


        TListePlayer playerHorde = NULL;

        // FIN de vos variables                                               */
        /**********************************************************************/

        // boucle principale du jeu
        srand(time(NULL));
        int cont = 1;
        int count = 0;
        while ( cont != 0 ){   //VOUS DEVEZ GERER (DETECTER) LA FIN DU JEU -> tourRoiDetruite

                SDL_PumpEvents(); //do events
                efface_fenetre(pWinSurf);
                prepareAllSpriteDuJeu(jeu,tabParcours,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf);

                /***********************************************************************/
                /*                                                                     */
                /*                                                                     */

                //APPELEZ ICI VOS FONCTIONS QUI FONT EVOLUER LE JEU

                //REMETTRE LE CHAMP PEUTATTAQUER � 1
                remettrePeutAttaquer(&playerRoi);
                remettrePeutAttaquer(&playerHorde);

                if (tourRoiDetruite(playerRoi)) {
                    printf("GAME OVER!\n");
                    cont = 0;
                }

                else {
                //PHASE COMBAT
                    printf("tour %d\n", count++);
                    // playerRoi joue
                    TListePlayer currentTour = playerRoi;
                    // parcourir tous les tour de playerRoi
                    while (currentTour != NULL){
                        // verifier s'il y des unites que le tour courant peut attaquer
                        TListePlayer listePorteeRoi = quiEstAPortee(jeu, currentTour->pdata);
                        if (listePorteeRoi != NULL) {
                            // tri la liste selon le point de vie
                            selectionSort(&listePorteeRoi);
                            // mettre a jour l'unite ayant le plus bas point de vie apres avoir passe par la fonction quiEstAPortee
                            // (parce que dans la fonction quiEstAPortee,
                            // quand on detecte une unite qu'on peut tirer dessus, on utilise les fonction creerDragon, creerArcher, etc.,
                            // ce qui donne la maximum point de vie par defaut)
                            MAJlistePortee(listePorteeRoi->pdata, &playerHorde);
                            combat(pWinSurf, currentTour->pdata, listePorteeRoi->pdata);
                            // mettre a jour le point de vie de cet unite dans la liste de joueur
                            MAJplayer(&playerHorde, listePorteeRoi->pdata);


                            if (listePorteeRoi->pdata->pointsDeVie <= 0) {
                                supprimerUnite(&playerHorde, listePorteeRoi->pdata, jeu);
                            }
                        }
                        currentTour = currentTour->suiv;
                    }

                    //printf("so we finish with Roi's turn, now it's showtime for Horde\n");

                    TListePlayer currentHorde = playerHorde;
                    while (currentHorde != NULL) {
                        if (currentHorde->pdata->pointsDeVie <= 0) {
                            Tunite* uniteAsupprime = currentHorde->pdata;
                            currentHorde = currentHorde->suiv;
                            supprimerUnite(&playerHorde, uniteAsupprime, jeu);
                        }
                        else {
                            TListePlayer listePorteeHorde = quiEstAPortee(jeu, currentHorde->pdata);
                            if (listePorteeHorde != NULL) {
                                // mettre a jour l'unite ayant le plus bas point de vie apres avoir passe par la fonction quiEstAPortee
                                // (parce que dans la fonction quiEstAPortee,
                                // quand on detecte une unite qu'on peut tirer dessus, on utilise les fonction creerDragon, creerArcher, etc.,
                                // ce qui donne la maximum point de vie par defaut)
                                MAJlistePortee(listePorteeHorde->pdata, &playerRoi);
                                combat(pWinSurf, currentHorde->pdata, listePorteeHorde->pdata);
                                // mettre a jour le point de vie de cet unite dans la liste de joueur
                                MAJplayer(&playerRoi, listePorteeHorde->pdata);
                            }
                            deplacerUnite(jeu, tabParcours, currentHorde->pdata);
                            currentHorde = currentHorde->suiv;
                        }
                    }

                    // PHASE CREATION

                    printf("Player Roi: \n");
                    printList(playerRoi);
                    printf("Player Horde: \n");
                    printList(playerHorde);
                    printf("------\n");

                    AjouterUnite(&playerRoi, randomUniteRoi(verifyPlaceRoi(tabParcours, randomCaseTabParcours(), jeu)));
                    PositionnePlayerOnPlateau(playerRoi, jeu);
                    AjouterUnite(&playerHorde, randomUniteHorde(tabParcours, jeu));
                    PositionnePlayerOnPlateau(playerHorde, jeu);


                }








                /*                                                                     */
                /*                                                                     */
                // FIN DE VOS APPELS
                /***********************************************************************/
                //affichage du jeu � chaque tour

                maj_fenetre(pWindow);
                SDL_Delay(50);  //valeur du d�lai � modifier �ventuellement


                //LECTURE DE CERTAINES TOUCHES POUR LANCER LES RESTAURATIONS ET SAUVEGARDES
                const Uint8* pKeyStates = SDL_GetKeyboardState(NULL);
                if ( pKeyStates[SDL_SCANCODE_V] ){
                        /* Ajouter vos appels de fonctions ci-dessous qd le joueur appuye sur V */

                        // APPELEZ ICI VOTRE FONCTION DE SAUVEGARDE/RESTAURATION DEMANDEE
                        message("Sauvegarde","Placer ici votre fonction de restauration/sauvegarde");

                        //Ne pas modifiez les 4 lignes ci-dessous
                        efface_fenetre(pWinSurf);
                        prepareAllSpriteDuJeu(jeu,tabParcours,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf);
                        maj_fenetre(pWindow);
                        SDL_Delay(300);
                }
                if ( pKeyStates[SDL_SCANCODE_C] ){
                        /* Ajouter vos appels de fonctions ci-dessous qd le joueur appuye sur C */

                        // APPELEZ ICI VOTRE FONCTION DE SAUVEGARDE/RESTAURATION DEMANDEE
                        message("Sauvegarde","Placer ici votre fonction de restauration/sauvegarde");

                        //Ne pas modifiez les 4 lignes ci-dessous
                        efface_fenetre(pWinSurf);
                        prepareAllSpriteDuJeu(jeu,tabParcours,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf);
                        maj_fenetre(pWindow);
                        SDL_Delay(300);
                }
                if ( pKeyStates[SDL_SCANCODE_D] ){
                        /* Ajouter vos appels de fonctions ci-dessous qd le joueur appuye sur D */

                        // APPELEZ ICI VOTRE FONCTION DE SAUVEGARDE/RESTAURATION DEMANDEE
                        message("Sauvegarde","Placer ici votre fonction de restauration/sauvegarde");

                        //Ne pas modifiez les 4 lignes ci-dessous
                        efface_fenetre(pWinSurf);
                        prepareAllSpriteDuJeu(jeu,tabParcours,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf);
                        maj_fenetre(pWindow);
                        SDL_Delay(300);
                }
                if ( pKeyStates[SDL_SCANCODE_S] ){
                        /* Ajouter vos appels de fonctions ci-dessous qd le joueur appyue sur S */

                        // APPELEZ ICI VOTRE FONCTION DE SAUVEGARDE/RESTAURATION DEMANDEE
                        message("Sauvegarde","Placer ici votre fonction de restauration/sauvegarde");

                        //Ne pas modifiez les 4 lignes ci-dessous
                        efface_fenetre(pWinSurf);
                        prepareAllSpriteDuJeu(jeu,tabParcours,LARGEURJEU,HAUTEURJEU,TabSprite,pWinSurf);
                        maj_fenetre(pWindow);
                        SDL_Delay(300);
                }
                if ( pKeyStates[SDL_SCANCODE_ESCAPE] ){
                        cont = 0;  //sortie de la boucle
                }

        }
        //fin boucle du jeu

        //free(*playerRoi);

        SDL_FreeSurface(pSpriteTourSol); // Lib�ration de la ressource occup�e par le sprite
        SDL_FreeSurface(pSpriteTourAir);
        SDL_FreeSurface(pSpriteTourRoi);
        SDL_FreeSurface(pSpriteArcher);
        SDL_FreeSurface(pSpriteChevalier);
        SDL_FreeSurface(pSpriteDragon);
        SDL_FreeSurface(pSpriteGargouille);
        SDL_FreeSurface(pSpriteEau);
        SDL_FreeSurface(pSpriteHerbe);
        SDL_FreeSurface(pSpritePont);
        SDL_FreeSurface(pWinSurf);
    }
    else
    {
        fprintf(stdout,"�chec de chargement du sprite (%s)\n",SDL_GetError());
    }

    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}
