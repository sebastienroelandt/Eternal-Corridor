# include <stdio.h>
# include <stdlib.h>
# include <windows.h>
# include <math.h>
# include <time.h>
# include <SDL.h>
# include <SDL_Image.h>
# include <SDL_ttf.h>
# include <fmod.h>
# include "fonctionGraphique.h"
# include "relationUtilisateur.h"
# include "jeu.h"

float positionCouloir;
const int largeurMur=1; //1->901, 3->301,...
const int nombreMur=900;
const int tailleMur=150;
const int tailleArcEnCiel=30;
const int largeurBandeArcEnCiel=2;
const int nombreCouleurArcEnCiel=4;
int difficulte=0;
int temps;
int tempsFonction;
int tempsFonctionRestant;
float hauteurFonction;
int nombreFonction;
int fonction;
int fonctionSens;
int transitionFonction;
int manque;

LISTE insererMur(LISTE liste, int i, SDL_Surface *ecran)
{
    LISTE temp;
    if(liste==NULL)
    {
        temp=(LISTE)malloc(sizeof(listeMur));
        temp->imageMur=ajouterRectangle(ecran, i*largeurMur, 200, largeurMur, 150, 0, 0, 0);
        SDL_SetAlpha(temp->imageMur._texture, SDL_SRCALPHA, 64);
        temp->psuivant=NULL;
        return temp;
    }
    else
    {
        liste->psuivant=insererMur(liste->psuivant,i,ecran);
    }
}

LISTE decalerMur(LISTE liste, SDL_Surface *ecran,int vitesse)
{
    if(liste==NULL)
    {
        return liste;
    }
    else
    {
        liste->imageMur._position.x-=vitesse;
        if(liste->imageMur._position.x<0)
        {
            if(nombreFonction%100==49||nombreFonction%100==99) transitionFonction=1;
            if(transitionFonction==1)
            {
                if(199<positionCouloir&&positionCouloir<201)
                {
                    transitionFonction=0;
                    nombreFonction++;
                    positionCouloir=200;
                    tempsFonctionRestant=0;
                }
                else
                {
                    if(positionCouloir>200)positionCouloir=positionCouloir-0.5;
                    else positionCouloir=positionCouloir+0.5;
                }
            }
            else if(nombreFonction%100<6)
            {
                if(tempsFonctionRestant<=0)
                {
                    nombreFonction++;
                    tempsFonction=900+(rand()%(50*difficulte));
                    tempsFonctionRestant=tempsFonction;
                    hauteurFonction=difficulte*10-30-rand()%(difficulte*10);
                }
                positionCouloir=200+(hauteurFonction*sin((float)(tempsFonction-tempsFonctionRestant)*2*3.14/tempsFonction));
            }
            else if(nombreFonction%100<50)
            {
                if(tempsFonctionRestant<=0)
                {
                    fonction=rand()%2; //Donc 1 ou 0;
                    if(rand()%6==1)
                    {
                        if(fonctionSens==1) fonctionSens=0;
                        else fonctionSens=1;
                    }
                    tempsFonctionRestant=200;
                    nombreFonction++;
                }
                if(fonction==0)
                {
                    if(fonctionSens==0)positionCouloir=positionCouloir;
                    else positionCouloir=positionCouloir;
                }
                else
                {
                    if(fonctionSens==0)positionCouloir=positionCouloir+0.5;
                    else positionCouloir=positionCouloir-0.5;
                }
                tempsFonctionRestant--;
            }
            else
            {
                positionCouloir=rand()%9-4+positionCouloir;
                if(tempsFonctionRestant<=0)
                {
                    tempsFonctionRestant=200;
                    nombreFonction++;
                }
                tempsFonctionRestant--;
            }



            if(nombreFonction>100)nombreFonction=0;
            if(positionCouloir<0)
            {
                positionCouloir=0;
                fonctionSens=0;
            }
            if(positionCouloir+tailleMur>ecran->h)
            {
                positionCouloir=ecran->h-tailleMur;
                fonctionSens=1;
            }
            liste->imageMur = modifierRectangle(ecran,liste->imageMur,900-manque, positionCouloir, largeurMur, 150, 0, 0, 0);
            SDL_SetAlpha(liste->imageMur._texture, SDL_SRCALPHA, 64);
            tempsFonctionRestant--;
            manque--;
        }
        liste->psuivant=decalerMur(liste->psuivant,ecran, vitesse);
    }
}

void affichageMur(LISTE liste, SDL_Surface *ecran)
{
    if(liste==NULL) return;
    afficherRectangle(ecran,liste->imageMur);
    affichageMur(liste->psuivant,ecran);
}

int verificationCollision(LISTE liste, image vaisseau)
{
    // Si 1 alors on continu, si 0 on arrête le jeu.

    //Si on est a la même position en x, alors on verifie s'il y a collision en y.
    if(liste->imageMur._position.x==vaisseau._position.x)
    {
        if(liste->imageMur._position.y>vaisseau._position.y)
        {
            return 0;
        }
        if(liste->imageMur._position.y+tailleMur<vaisseau._position.y+vaisseau._texture->h)
        {
            return 0;
        }
    }
    if(liste->imageMur._position.x==vaisseau._position.x+vaisseau._texture->w)
    {
        if(liste->imageMur._position.y>vaisseau._position.y+vaisseau._texture->w/2)
        {
            return 0;
        }
        if(liste->imageMur._position.y+tailleMur<vaisseau._position.y+vaisseau._texture->w/2)
        {
            return 0;
        }
    }
    if(liste->psuivant!=NULL)
    {
        return verificationCollision(liste->psuivant, vaisseau);
    }
    else return 1;
}

int jeu(SDL_Surface *ecran)
{
    /// Création et initialisation des variables utiles au jeu
    //Initialisation des valeurs globales
    nombreFonction=0;
    positionCouloir=200;
    difficulte=6;
    temps=0;
    tempsFonction=0;
    tempsFonctionRestant=0;
    hauteurFonction=0;
    transitionFonction=0;
    manque=0;

    //Creation et initialisation de varibles
    LISTE liste=NULL;
    int i,j;
    int vitesse=3;
    int vitesseVaisseau=2;
    int affichageInfo=0;
    Uint8 *keystates= SDL_GetKeyState(NULL); // Gestionnaire de touche
    float couleursActuelles[3]= {100,100,150};
    int tempsActuel=0;
    int tempsPrecedent=0;

    /// Debut de l'aléatoire
    srand(time(NULL));

    /// Creation des murs
    for(i=0; i<nombreMur; i++) liste=insererMur(liste,i,ecran);

    /// Création du vaisseau
    image vaisseau = ajouterImage(ecran,"Image/Jeu/vaisseau.png",100,300-20);
    SDL_SetAlpha(vaisseau._texture, SDL_SRCALPHA, 40);


    /// Création de l'arc-en-ciel
    rectangle arcEnCiel[tailleArcEnCiel][nombreCouleurArcEnCiel];
    //Couleur rouge
    for(i=0; i<tailleArcEnCiel; i++) arcEnCiel[i][0]=ajouterRectangle(ecran,-50,-50,largeurBandeArcEnCiel,vaisseau._texture->h/nombreCouleurArcEnCiel, 254,56,71);
    //Couleur jaune
    for(i=0; i<tailleArcEnCiel; i++) arcEnCiel[i][1]=ajouterRectangle(ecran,-50,-50,largeurBandeArcEnCiel,vaisseau._texture->h/nombreCouleurArcEnCiel, 254,236,28);
    //Couleur Bleue clair
    for(i=0; i<tailleArcEnCiel; i++) arcEnCiel[i][2]=ajouterRectangle(ecran,-50,-50,largeurBandeArcEnCiel,vaisseau._texture->h/nombreCouleurArcEnCiel, 36,251,173);
    //Couleur Bleue foncé
    for(i=0; i<tailleArcEnCiel; i++) arcEnCiel[i][3]=ajouterRectangle(ecran,-50,-50,largeurBandeArcEnCiel,vaisseau._texture->h/nombreCouleurArcEnCiel, 2,101,254);

    //Application de la transparence
    for(j=0; j<nombreCouleurArcEnCiel; j++)
    {
        for(i=0; i<tailleArcEnCiel; i++) SDL_SetAlpha(arcEnCiel[i][j]._texture, SDL_SRCALPHA, 180-(180*i/tailleArcEnCiel));
    }

    /// Creation du score + gestion des ecritures
    TTF_Init();
    TTF_Font *policeCaslonRoman = TTF_OpenFont("Font/CaslonRoman.ttf", 30);
    SDL_Color couleur = {0, 0, 0};
    SDL_Surface *texteScore,*texteInfo;
    SDL_Rect positionScore,positionInfo;
    positionScore.x=700;
    positionScore.y=50;
    positionInfo.x=10;
    positionInfo.y=10;

    int score=0;
    texteScore = TTF_RenderText_Blended(policeCaslonRoman, "0", couleur);
    int anciennePositionVaisseau=100;
    texteInfo = TTF_RenderText_Blended(policeCaslonRoman, " ", couleur);

    char tmp[100];

    /// Texte de fin

    texte texteFin = ajouterTexte(ecran, "Vous avez perdu avec un score de:", 200,250,30,0,0,0,"Font/CaslonRoman.ttf");

    ///Son
    FMOD_SYSTEM *system;
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);

    FMOD_SOUND *sound = NULL;
    if (FMOD_System_CreateSound(system, "Sound/Son.s3m", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &sound)!= FMOD_OK)
        exit(EXIT_FAILURE);
    //Activation de la lecture infini
    FMOD_Sound_SetLoopCount(sound, -1);
    //Démarrage de la musique
    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound, 0, NULL);

    ///Debut des evenements
    SDL_Event event;
    int continuer=1;
    while(continuer==1)
    {
        manque=vitesse;
        liste=decalerMur(liste, ecran, vitesse);

        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                exit(EXIT_SUCCESS);
                break;
            }
        }
        ///Changement du score
        score=score+vitesse+vaisseau._position.x-anciennePositionVaisseau;
        anciennePositionVaisseau=vaisseau._position.x;

        /// Deplacement du vaisseau
        if(keystates[SDLK_UP]) vaisseau._position.y-=vitesseVaisseau;
        if(keystates[SDLK_DOWN]) vaisseau._position.y+=vitesseVaisseau;
        if(keystates[SDLK_RIGHT]) vaisseau._position.x+=vitesseVaisseau;
        if(keystates[SDLK_LEFT]) vaisseau._position.x-=vitesseVaisseau;

        if(vaisseau._position.y<0)vaisseau._position.y=0;
        if(vaisseau._position.y+vaisseau._texture->h>ecran->h)vaisseau._position.y=ecran->h-vaisseau._texture->h;
        if(vaisseau._position.x<0)vaisseau._position.x=0;
        if(vaisseau._position.x+vaisseau._texture->w>ecran->w)vaisseau._position.x=ecran->w-vaisseau._texture->w;


        ///Deplacement de l'arc en ciel
        for(j=0; j<nombreCouleurArcEnCiel; j++)
        {
            for(i=0; i<tailleArcEnCiel; i++)arcEnCiel[i][j]._position.x=vaisseau._position.x-i*largeurBandeArcEnCiel;
            arcEnCiel[0][j]._position.y=vaisseau._position.y+(j*vaisseau._texture->h)/4;
            for(i=tailleArcEnCiel-1; i>0; i--)arcEnCiel[i][j]._position.y=arcEnCiel[i-1][j]._position.y;
        }


        ///Verification de colision
        continuer = verificationCollision(liste, vaisseau);
        //score = verificationCollision(liste, vaisseau);

        ///Autres touches
        if(keystates[SDLK_ESCAPE]) continuer=2;
        if(keystates[SDLK_h])
        {
            if(affichageInfo==1)affichageInfo=0;
            else affichageInfo=1;
        }


        ///Gestion du temps + score + difficulte selon la vitesse:
        temps++;
        if(nombreFonction%100<6)
        {
            if(temps%120==1)difficulte++;
        }
        vitesse=3+temps/3000;
        if(keystates[SDLK_SPACE])
        {
            vitesse+=10;
            score+=10;
        }

        ///Modification des variables d'affichage de differentes variables:
        //Affichage du score
        sprintf(tmp, "%d", score);
        SDL_FreeSurface(texteScore);
        texteScore = TTF_RenderText_Blended(policeCaslonRoman, tmp, couleur);

        //Affichage des autres infos
        if(affichageInfo==1)
        {
            sprintf(tmp, "Difficulté:%d    Hauteur:%d   Periode:%d", (int)difficulte, (int)hauteurFonction, (int)tempsFonction);
            SDL_FreeSurface(texteInfo);
            texteInfo = TTF_RenderText_Blended(policeCaslonRoman, tmp, couleur);
        }

        //Couleurs des murs
        if(6<nombreFonction&&nombreFonction<26)
        {
            if(couleursActuelles[0]!=150&&couleursActuelles[2]!=100)
            {
                couleursActuelles[0]+=1;
                couleursActuelles[2]-=1;
            }
        }
        else if(50<nombreFonction&&nombreFonction<56)
        {
            if(couleursActuelles[0]!=100&&couleursActuelles[1]!=150)
            {
                couleursActuelles[1]+=1;
                couleursActuelles[0]-=1;
            }
        }
        else if(97<nombreFonction||(0<nombreFonction&&nombreFonction<6))
        {
            if(couleursActuelles[1]!=100&&couleursActuelles[2]!=150)
            {
                couleursActuelles[1]-=1;
                couleursActuelles[2]+=1;
            }
        }

        /// Debut de l'Affichage
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,couleursActuelles[0],couleursActuelles[1],couleursActuelles[2]));

        affichageMur(liste, ecran);
        for(j=0; j<nombreCouleurArcEnCiel; j++)
        {
            for(i=0; i<tailleArcEnCiel; i++)afficherRectangle(ecran, arcEnCiel[i][j]);
        }
        afficherImage(ecran, vaisseau);
        SDL_BlitSurface(texteScore, NULL, ecran, &positionScore);
        if(affichageInfo==1)SDL_BlitSurface(texteInfo, NULL, ecran, &positionInfo);

        SDL_Flip(ecran);

        tempsActuel = SDL_GetTicks();
        if (tempsActuel - tempsPrecedent < 20)
            SDL_Delay(20 - (tempsActuel - tempsPrecedent));
        tempsPrecedent = tempsActuel;

    }

    ///Affichage du score final en cas de collision avec le mur
    if(continuer==0)
    {
        //Arret de la musique
        FMOD_Sound_Release(sound);

        //Affichage du score
        positionScore.x=420;
        positionScore.y=300;

        afficherTexte(ecran, texteFin);
        SDL_BlitSurface(texteScore, NULL, ecran, &positionScore);

        SDL_Flip(ecran);

        SDL_Delay(5000);
    }
    if(continuer==2) score=0;

    ///Fin du son
    FMOD_System_Close(system);
    FMOD_System_Release(system);

    return score;

}
