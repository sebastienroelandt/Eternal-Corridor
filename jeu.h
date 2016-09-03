#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

typedef struct mur
{
    rectangle imageMur;
    struct noeud *psuivant;

} listeMur;

typedef listeMur *LISTE;

int jeu(SDL_Surface *ecran);
int verificationCollision(LISTE liste, image vaisseau);
void affichageMur(LISTE liste, SDL_Surface *ecran);

#endif // JEU_H_INCLUDED
