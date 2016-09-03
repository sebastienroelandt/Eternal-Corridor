# include <stdio.h>
# include <stdlib.h>
# include <SDL.h>
# include <SDL_image.h>
# include <SDL_ttf.h>
# include "fonctionGraphique.h"

image ajouterImage (SDL_Surface *ecran, char *lienImage, int positionX, int positionY)
{
    image nouvelleImage;
    nouvelleImage._texture = IMG_Load(lienImage);
    nouvelleImage._position.x = positionX;
    nouvelleImage._position.y = positionY;
    return nouvelleImage;
}

texte ajouterTexte (SDL_Surface *ecran, char *phrase, int positionX, int positionY,int taille, int couleur1, int couleur2, int couleur3, char *policeTexte)
{
    texte nouveauTexte;
    TTF_Font *police = TTF_OpenFont(policeTexte, taille);
    SDL_Color couleur = {couleur1, couleur2, couleur3};
    nouveauTexte._texture = TTF_RenderText_Blended(police, phrase, couleur);
    nouveauTexte._position.x = positionX;
    nouveauTexte._position.y = positionY;
    return nouveauTexte;
}

rectangle ajouterRectangle (SDL_Surface *ecran,int positionX, int positionY, int largeur, int hauteur, int couleur1, int couleur2, int couleur3)
{
    rectangle nouveauRectangle;
    nouveauRectangle._texture = SDL_CreateRGBSurface(SDL_HWSURFACE, largeur, hauteur,32, 0, 0, 0, 0);
    SDL_FillRect(nouveauRectangle._texture, NULL, SDL_MapRGB(ecran->format, couleur1, couleur2, couleur3));
    nouveauRectangle._position.x = positionX;
    nouveauRectangle._position.y = positionY;
    return nouveauRectangle;
}

rectangle modifierRectangle (SDL_Surface *ecran, rectangle nouveauRectangle,int positionX, int positionY, int largeur, int hauteur, int couleur1, int couleur2, int couleur3)
{
    nouveauRectangle._texture = SDL_CreateRGBSurface(SDL_HWSURFACE, largeur, hauteur,32, 0, 0, 0, 0);
    SDL_FillRect(nouveauRectangle._texture, NULL, SDL_MapRGB(ecran->format, couleur1, couleur2, couleur3));
    nouveauRectangle._position.x = positionX;
    nouveauRectangle._position.y = positionY;
    return nouveauRectangle;
}

bouton ajouterBouton(int positionX, int positionY, char *texte, char *policeTexte,SDL_Surface *texture)
{
    bouton nouveauBouton;
    nouveauBouton._position.x=positionX;
    nouveauBouton._position.y=positionY;

    nouveauBouton._texture=texture;

    TTF_Font *police = TTF_OpenFont(policeTexte, 20);
    SDL_Color couleurNoire = {0, 0, 0};

    nouveauBouton._texte=TTF_RenderText_Blended(police, texte, couleurNoire);
    nouveauBouton._positiontexte.x=nouveauBouton._position.x+nouveauBouton._texture->w/2-nouveauBouton._texte->w/2;
    nouveauBouton._positiontexte.y=nouveauBouton._position.y+nouveauBouton._texture->h/2-nouveauBouton._texte->h/2;

    nouveauBouton._clic=0;

    return nouveauBouton;
}

bouton modifierBouton(bouton nouveaubouton, int sourisX, int sourisY, SDL_Event event,SDL_Surface *texture1,SDL_Surface *texture2)
{
    if(sourisX>nouveaubouton._position.x&&sourisX<(nouveaubouton._position.x+nouveaubouton._texture->w)
            &&sourisY>nouveaubouton._position.y&&sourisY<(nouveaubouton._position.y+nouveaubouton._texture->h))
    {
        nouveaubouton._texture=texture2;
        if (event.button.button == SDL_BUTTON_LEFT) nouveaubouton._clic=1;
    }
    else  nouveaubouton._texture=texture1;

    return nouveaubouton;
}

void afficherBouton(SDL_Surface *ecran, bouton bouton1)
{
    SDL_BlitSurface(bouton1._texture, NULL, ecran, &bouton1._position);
    SDL_BlitSurface(bouton1._texte, NULL, ecran, &bouton1._positiontexte);
}

void afficherRectangle(SDL_Surface *ecran, rectangle rectangle1)
{
    SDL_BlitSurface(rectangle1._texture, NULL, ecran, &rectangle1._position);
}

void afficherTexte(SDL_Surface *ecran, texte texte1)
{
    SDL_BlitSurface(texte1._texture, NULL, ecran, &texte1._position);
}

void afficherImage(SDL_Surface *ecran, image image1)
{
    SDL_BlitSurface(image1._texture, NULL, ecran, &image1._position);
}
