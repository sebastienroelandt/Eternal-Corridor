# include <stdio.h>
# include <stdlib.h>
# include <windows.h>
# include <SDL.h>
# include <SDL_Image.h>
# include <SDL_ttf.h>
# include "fonctionGraphique.h"
# include "relationUtilisateur.h"
# include "menu.h"
# include <fmod.h>


int main(int argc, char **argv)
{
    // Initialisation et création de la fenetre graphique
    SDL_Init(SDL_INIT_VIDEO);

    TTF_Init();
    SDL_Surface *ecran = NULL;

    putenv("SDL_VIDEO_WINDOW_POS=center");
    ecran = SDL_SetVideoMode(900,600,32,SDL_HWSURFACE | SDL_DOUBLEBUF);

    SDL_WM_SetCaption("Eternal corridor !",NULL);
    //SDL_WM_SetIcon(IMG_Load("Image/Icone.bmp"), NULL);

    menu(ecran);
    putenv("SDL_VIDEO_WINDOW_POS=10,15");

    SDL_Quit();
    return 0;
}
