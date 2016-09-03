# include <stdio.h>
# include <stdlib.h>
# include <windows.h>
# include <SDL.h>
# include <SDL_Image.h>
# include <SDL_ttf.h>
# include <string.h>
# include "fonctionGraphique.h"
# include "relationUtilisateur.h"
# include "jeu.h"

int positionCouloir;
int nombreBarre;

LISTE ajouterMur(LISTE liste, int i, SDL_Surface *ecran)
{
    LISTE temp;
    if(liste==NULL)
    {
        temp=(LISTE)malloc(sizeof(listeMur));
        temp->imageMur=ajouterRectangle(ecran, i, 200, 1, 150, 0, 0, 0);
        SDL_SetAlpha(temp->imageMur._texture, SDL_SRCALPHA, 64);
        temp->psuivant=NULL;
        return temp;
    }
    else
    {
        liste->psuivant=insererMur(liste->psuivant,i,ecran);
    }
}

LISTE bougerMur(LISTE liste, SDL_Surface *ecran,int vitesse, int temps)
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
            nombreBarre++;
            if(nombreBarre<700)positionCouloir=200-(120*sin((float)(700-nombreBarre)*2*3.14/700));
            else positionCouloir=200;
            liste->imageMur = modifierRectangle(ecran,liste->imageMur,liste->imageMur._position.x+901, positionCouloir, 1, 150, 0, 0, 0);
            SDL_SetAlpha(liste->imageMur._texture, SDL_SRCALPHA, 64);
        }
        liste->psuivant=bougerMur(liste->psuivant,ecran, vitesse, temps);
    }
}

void introduction(SDL_Surface *ecran, image vaisseau)
{
    ///Variables utiles
    int temps=0;
    int i;
    positionCouloir=200;
    nombreBarre=0;

    ///Gestion de l'écriture
    TTF_Init();
    texte presentation1 = ajouterTexte(ecran, "Partie Développement :", 330, 200, 20, 200, 200, 200, "Font/CaslonRoman.ttf");
    texte presentation2 = ajouterTexte(ecran, "Sébastien Roelandt", 345, 225, 20, 200, 200, 200, "Font/CaslonRoman.ttf");
    texte presentation3 = ajouterTexte(ecran, "Partie Design :", 370, 300, 20, 200, 200, 200, "Font/CaslonRoman.ttf");
    texte presentation4 = ajouterTexte(ecran, "Sébastien Roelandt et Ibrahym Omrane", 250, 325, 20, 200, 200, 200, "Font/CaslonRoman.ttf");

    ///Gestion des transtions
    rectangle transition = ajouterRectangle(ecran,0,0,ecran->w,ecran->h,0,0,0);

    ///Creation du couloir
    LISTE liste=NULL;
    for(i=0; i<901; i++) liste=ajouterMur(liste, i, ecran);
    liste=bougerMur(liste, ecran, 700, temps);

    SDL_Event event;
    int continuer=1;
    while(continuer)
    {
        ///Gestion des changements graphiques (transitions)
        temps+=1;
        if(temps<=30) SDL_SetAlpha(transition._texture, SDL_SRCALPHA, 200*(temps)/30);
        else if(temps<80) SDL_SetAlpha(transition._texture, SDL_SRCALPHA, 200);
        else if(temps<110) SDL_SetAlpha(transition._texture, SDL_SRCALPHA, 200-200*(temps-80)/30);
        else SDL_SetAlpha(transition._texture, SDL_SRCALPHA, 0);


        ///Deplacement des murs
        liste=bougerMur(liste, ecran, 6, temps);

        ///Deplacement du vaisseau
        //En y:
        if(20<temps&&temps<40) vaisseau._position.y=vaisseau._position.y+5;
        else if(50<temps&&temps<85)vaisseau._position.y=vaisseau._position.y-6;
        else if(105<temps&&temps<133)vaisseau._position.y=vaisseau._position.y+4;
        //else if(temps==125)vaisseau._position.y=vaisseau._position.y+2;

        //En x:
        if(48<temps&&temps<58) vaisseau._position.x=vaisseau._position.x+6;
        else if(115<temps&&temps<125)vaisseau._position.x=vaisseau._position.x-6;

        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                exit(EXIT_SUCCESS);
                break;
            }
        }
        ///Condition d'arrêt: la fin du temps
        if(temps>=150) continuer=0;

        ///Debut de l'affichage
        //if(temps<=110)SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,29,24,44));
        //else
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 100, 100, 150));

        affichageMur(liste, ecran);
        afficherImage(ecran, vaisseau);

        if(temps<140)afficherRectangle(ecran, transition);

        if(30<=temps&&80>=temps)
        {
            afficherTexte(ecran, presentation1);
            afficherTexte(ecran, presentation2);
            afficherTexte(ecran, presentation3);
            afficherTexte(ecran, presentation4);
        }

        SDL_Flip(ecran);

        SDL_Delay(50);
    }
}

int lectureScore(int tableauScore[5])
{
    int i;
    FILE* fichier = NULL;
    fichier = fopen("Table_des_scores.txt", "r");
    fscanf(fichier, "%d %d %d %d %d", &tableauScore[0],
           &tableauScore[1], &tableauScore[2], &tableauScore[3], &tableauScore[4]);
    fclose(fichier);
    return tableauScore;
}

char lecturePseudo(char tableauPseudo[5][20])
{
    int i,j;
    FILE* fichier = NULL;
    fichier = fopen("Table_des_pseudos.txt", "r");
    /*for(i=0;i<5;i++)
    {
        for(j=0;j<20;j++)tableauPseudo[i][j]=fgetc(fichier);
    }*/
    fscanf(fichier, "%s %s %s %s %s", &tableauPseudo[0],
           &tableauPseudo[1], &tableauPseudo[2], &tableauPseudo[3], &tableauPseudo[4]);
    //fgets(tableauPseudo[i], 20, fichier);
    fclose(fichier);
    return tableauPseudo;
}

void ecriturePseudo(char tableauPseudo[5][20])
{
    int i,j;
    FILE* fichier = NULL;
    fichier = fopen("Table_des_pseudos.txt", "w+");
    for(i=0;i<5;i++)
    {
        for(j=0;j<20;j++)fprintf(fichier,"%c", tableauPseudo[i][j]);
        fprintf(fichier," ");
    }
    fclose(fichier);
}

void ecritureScore(int tableauScore[5])
{
    int i;
    FILE* fichier = NULL;
    fichier = fopen("Table_des_scores.txt", "w+");
    for(i=0;i<5;i++) fprintf(fichier,"%d ", tableauScore[i]);
    fclose(fichier);
}

char demandePseudo(SDL_Surface *ecran, char pseudo[5][20], int i)
{
    int j;
    char nouveauPseudo[20]="";
    demandeFonction(ecran, nouveauPseudo,i);
    for(j=0;j<20;j++)pseudo[i][j]=nouveauPseudo[j];
    return pseudo;
}

void menu(SDL_Surface *ecran)
{
    int i,j,k;

    image vaisseau = ajouterImage(ecran,"Image/Jeu/Vaisseau.png",100,300-20);
    SDL_SetAlpha(vaisseau._texture, SDL_SRCALPHA, 40);
    introduction(ecran, vaisseau);

    // Chargement des fonts de boutons:
    SDL_Surface *boutonNormal = IMG_Load("Image/Menu/Bouton_1.png");
    SDL_Surface *boutonFonce = IMG_Load("Image/Menu/Bouton_2.png");

    bouton demarrer = ajouterBouton(ecran->w/2-boutonNormal->w/2,ecran->h-200, "Demarrer", "Font/CaslonRoman.ttf", boutonNormal);

    texte titre = ajouterTexte(ecran, "Eternal corridor", 280 ,20 ,50, 50, 50, 120, "Font/CaslonRoman.ttf");

    ///Version
    // Chargement des fonts de petits boutons:
    int afficherVersion=0;

    SDL_Surface *boutonPetitNormal = IMG_Load("Image/Menu/Bouton_petit_1.png");
    SDL_Surface *boutonPetitFonce = IMG_Load("Image/Menu/Bouton_petit_2.png");
    SDL_Surface *boutonQuitterNormal = IMG_Load("Image/Menu/Bouton_quitter_1.png");
    SDL_Surface *boutonQuitterFonce = IMG_Load("Image/Menu/Bouton_quitter_2.png");

    bouton version = ajouterBouton(ecran->w-110,ecran->h-40, "Version", "Font/CaslonRoman.ttf", boutonPetitNormal);
    bouton quitterVersion = ajouterBouton(ecran->w-100,100, "X", "Font/CaslonRoman.ttf", boutonQuitterNormal);
    rectangle fondVersion = ajouterRectangle(ecran, 100 ,100,ecran->w-200, ecran->h-200, 0, 0, 0);
    SDL_SetAlpha(fondVersion._texture, SDL_SRCALPHA, 200);

    //Texte à écrire
    texte texte1 = ajouterTexte(ecran, "Version 1.9 :                       05/11/2013", 120 ,150 ,30, 100, 100, 150, "Font/CaslonRoman.ttf");
    texte texte2 = ajouterTexte(ecran, "- Création du 3ème type de terrain: l'aléatoire", 150 ,210 ,20, 100, 100, 150, "Font/CaslonRoman.ttf");
    texte texte3 = ajouterTexte(ecran, "- Modification des couleurs de fond", 150 ,235 ,20, 100, 100, 150, "Font/CaslonRoman.ttf");
    texte texte4 = ajouterTexte(ecran, "- Résolution du bug de génération des murs", 150 ,260 ,20, 100, 100, 150, "Font/CaslonRoman.ttf");
    texte texte5 = ajouterTexte(ecran, "- Réglage d'un FPS minimal, pour une meilleure fluidité", 150 ,285 ,20, 100, 100, 150, "Font/CaslonRoman.ttf");
    texte texte6 = ajouterTexte(ecran, "- Ajout du bouton Version pour être au courant des changements", 150 ,310 ,20, 100, 100, 150, "Font/CaslonRoman.ttf");
    texte texte7 = ajouterTexte(ecran, "- Ajout de la musique", 150 ,335 ,20, 100, 100, 150, "Font/CaslonRoman.ttf");
    texte texte8 = ajouterTexte(ecran, "- Ajout de la table des records", 150 ,360 ,20, 100, 100, 150, "Font/CaslonRoman.ttf");


    ///Gestion des scores
    int tableauScore[5];
    lectureScore(tableauScore);
    int score=0;
    bouton boutonScore = ajouterBouton(ecran->w/2-boutonNormal->w/2,ecran->h-130, "Score", "Font/CaslonRoman.ttf", boutonNormal);
    int afficherScore=0;

    TTF_Font *policeCaslonRoman = TTF_OpenFont("Font/CaslonRoman.ttf", 30);
    SDL_Color couleur = {100, 100, 150};
    SDL_Surface *texteScore[5];
    for(i=0;i<5;i++)texteScore[i] = TTF_RenderText_Blended(policeCaslonRoman, "0", couleur);
    SDL_Rect positionScore[i];
    for(i=0;i<5;i++)
    {
        positionScore[i].x=300;
        positionScore[i].y=200+i*30;
    }
    char tmp[100];

    //Gestion de pseudo
    char pseudo[5][20];
    for(i=0;i<5;i++)
    {
        for(j=0;j<20;j++)pseudo[i][j]=' ';
    }
    lecturePseudo(pseudo);


    ///Gestion du fond
    rectangle couloir = ajouterRectangle(ecran, 0 ,200,ecran->w, 150, 0, 0, 0);
    SDL_SetAlpha(couloir._texture, SDL_SRCALPHA, 64);

    SDL_Event event;
    int continuer=1;
    while(continuer)
    {

        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                exit(EXIT_SUCCESS);
                break;
            }
        }
        if(afficherVersion==0&&afficherScore==0)
        {
            demarrer = modifierBouton(demarrer, event.button.x, event.button.y, event, boutonNormal, boutonFonce);
            boutonScore = modifierBouton(boutonScore, event.button.x, event.button.y, event, boutonNormal, boutonFonce);
            version = modifierBouton(version, event.button.x, event.button.y, event, boutonPetitNormal, boutonPetitFonce);
        }
        else
        {
            quitterVersion = modifierBouton(quitterVersion, event.button.x, event.button.y, event, boutonQuitterNormal, boutonQuitterFonce);
        }

        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 100, 100, 150));

        afficherRectangle(ecran, couloir);
        afficherImage(ecran, vaisseau);
        afficherBouton(ecran, demarrer);
        afficherBouton(ecran, version);
        afficherBouton(ecran, boutonScore);

        if(afficherVersion==1)
        {
            afficherBouton(ecran, quitterVersion);
            afficherRectangle(ecran, fondVersion);
            afficherTexte(ecran, texte1);
            afficherTexte(ecran, texte2);
            afficherTexte(ecran, texte3);
            afficherTexte(ecran, texte4);
            afficherTexte(ecran, texte5);
            afficherTexte(ecran, texte6);
            afficherTexte(ecran, texte7);
            //afficherTexte(ecran, texte8);
        }
        if(afficherScore==1)
        {
            for(i=0;i<5;i++)
            {
                sprintf(tmp, "%d: %d %s", i+1, tableauScore[i], pseudo[i]);
                SDL_FreeSurface(texteScore[i]);
                texteScore[i] = TTF_RenderText_Blended(policeCaslonRoman, tmp, couleur);
            }
            afficherBouton(ecran, quitterVersion);
            afficherRectangle(ecran, fondVersion);
            for(i=0;i<5;i++)SDL_BlitSurface(texteScore[i], NULL, ecran, &positionScore[i]);
        }
        afficherTexte(ecran, titre);

        SDL_Flip(ecran);

        SDL_Delay(50);

        if (demarrer._clic)
        {
            score=jeu(ecran);
            for(i=0;i<5;i++)
            {
                if(score>tableauScore[i])
                {
                    for(j=5;j>i;j--)
                    {
                        tableauScore[j]=tableauScore[j-1];
                    }
                    tableauScore[i]=score;
                    for(j=5;j>i;j--)
                    {
                        for(k=0;k<20;k++)pseudo[j][k]=pseudo[j-1][k];
                    }
                    demandePseudo(ecran, pseudo, i);
                    i=5;
                }
            }
            ecritureScore(tableauScore);
            ecriturePseudo(pseudo);
            demarrer._clic=0;
        }
        if (version._clic)
        {
            afficherVersion=1;
            version._clic=0;
        }
        if (boutonScore._clic)
        {
            afficherScore=1;
            boutonScore._clic=0;
        }
        if (quitterVersion._clic)
        {
            afficherVersion=0;
            afficherScore=0;
            quitterVersion._clic=0;
        }
    }

    SDL_FreeSurface(boutonNormal);
    SDL_FreeSurface(boutonFonce);

}
