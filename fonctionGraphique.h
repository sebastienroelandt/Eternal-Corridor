#ifndef FONCTIONGRAPHIQUE_H_INCLUDED
#define FONCTIONGRAPHIQUE_H_INCLUDED

# include <SDL.h>
# include <SDL_image.h>

typedef struct
{
    SDL_Surface *_texture;
    SDL_Rect _position;
} image;

typedef struct
{
    SDL_Surface *_texture;
    SDL_Rect _position;
} texte;

typedef struct
{
    SDL_Surface *_texture;
    SDL_Rect _position;
} rectangle;

typedef struct
{
    SDL_Rect _position;
    SDL_Rect _positiontexte;
    SDL_Surface *_texte;
    SDL_Surface *_texture;
    int _clic;
} bouton;

image ajouterImage (SDL_Surface *ecran, char *lienImage, int positionX, int positionY);
texte ajouterTexte (SDL_Surface *ecran, char *phrase, int positionX, int positionY,int taille, int couleur1, int couleur2, int couleur3, char *policeTexte);
rectangle ajouterRectangle (SDL_Surface *ecran,int positionX, int positionY, int largeur, int hauteur, int couleur1, int couleur2, int couleur3);
bouton ajouterBouton(int positionX, int positionY, char *texte, char *policeTexte, SDL_Surface *texture);

bouton modifierBouton(bouton bouton, int sourisX, int sourisY, SDL_Event event,SDL_Surface *texture1,SDL_Surface *texture2);
rectangle modifierRectangle (SDL_Surface *ecran, rectangle nouveauRectangle,int positionX, int positionY, int largeur, int hauteur, int couleur1, int couleur2, int couleur3);

void afficherBouton(SDL_Surface *ecran, bouton bouton1);
void afficherRectangle(SDL_Surface *ecran, rectangle rectangle1);
void afficherTexte(SDL_Surface *ecran, texte texte1);
void afficherImage(SDL_Surface *ecran, image image1);
#endif // FONCTIONGRAPHIQUE_H_INCLUDED
