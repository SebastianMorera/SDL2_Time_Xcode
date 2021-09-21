//
//  main.c
//  SDL_Time: Learn to play with time and display a counter in ms
//  and also move/stop zozor with keys
//
//
//  Created by Sebastian Morera on 2021-09-07.
//

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)

void SDL_ExitWithDestruction(SDL_Renderer *renderer, SDL_Window *window, const char *message);
void SDL_ExitWithError(const char *message);
Uint32 bougerZozo(Uint32 intervalle, void *parametre); /* Fonction de callback (sera appelée toutes les 30 ms) */

#define TAILLE_MAX 1000

int main()
{
    char *title = "SDL Time";
    int continuer = 1;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Surface *zozor = NULL;
    SDL_Texture *textureZozor = NULL, *textureTexte = NULL, *textureChiffre = NULL;
    SDL_Event event;
    SDL_Color couleurNoire = {0, 0, 0}, couleurBlanche = {255, 255, 255};

    // Initialisation SDL + affichage fênetre avec une image + une icone de fênetre
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Lancement SDL, chargement de tous les systemes
    //if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
        SDL_ExitWithError("Initialisation de la SDL");

    // Création fenêtre + rendu
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderClear(renderer); // Clear the entire screen to our selected color.

    // Création de l'icone pour la fênetre
    SDL_Surface *icone = IMG_Load("caisse.jpg");
    SDL_SetWindowIcon(window, icone);
    SDL_FreeSurface(icone);
    
    // Lancement SDL_ttf
    char *texteAfficher = "Une reussite de plus !";
    TTF_Init();
    TTF_Font *police = TTF_OpenFont("angelina.ttf", 40);    //this opens a font style and sets a size
    /* Le texte sera écrit en italique et souligné */
    //TTF_SetFontStyle(police, TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE);
    // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    SDL_Surface* surfaceTexte = TTF_RenderText_Blended(police, texteAfficher, couleurNoire);
    //SDL_Surface* surfaceTexte = TTF_RenderText_Solid(police, "Une reussite de plus!", couleurNoire);
    
    textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
    SDL_FreeSurface(surfaceTexte);
    
    SDL_Rect positionTexte; //create a rect
    positionTexte.x = 300;  //controls the rect's x coordinate
    positionTexte.y = 100; // controls the rect's y coordinte
    TTF_SizeText(police, "Une reussite de plus !", &positionTexte.w, &positionTexte.h);
    SDL_RenderCopy(renderer, textureTexte, NULL, &positionTexte);

    /* Chargement et blittage du menu sur la scène */
    zozor = IMG_Load("zozor.bmp");
    if (zozor == NULL)
        SDL_ExitWithDestruction(renderer, window, "Impossible de charger image");

    SDL_Rect positionZozor;
    positionZozor.x = 0; //WINDOW_WIDTH / 2 - zozor->w / 2;
    positionZozor.y = WINDOW_HEIGHT / 2 - zozor->h / 2;

    SDL_SetColorKey(zozor, SDL_TRUE, SDL_MapRGB(zozor->format, 0, 0, 255));
    SDL_SetSurfaceAlphaMod(zozor, 128); //Transperence de notre image
    textureZozor = SDL_CreateTextureFromSurface(renderer, zozor);
    SDL_FreeSurface(zozor);

    if (textureZozor == NULL)
        SDL_ExitWithDestruction(renderer, window, "Impossible de creer la texture");

    if(SDL_QueryTexture(textureZozor, NULL, NULL, &positionZozor.w, &positionZozor.h) != 0)
        SDL_ExitWithDestruction(renderer, window, "Impossible de charger la texture");

    if(SDL_RenderCopy(renderer, textureZozor, NULL, &positionZozor) != 0)
        SDL_ExitWithDestruction(renderer, window, "Impossible d afficher la texture");

    SDL_RenderPresent(renderer); // Afficher l'image de fond
    
    // Events (ce que le programme doit faire)
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //int tempsPrecedent = 0, tempsActuel = 0;
    int timerON = 0;
    SDL_TimerID timer = 0; /* Variable pour stocker le numéro du timer */
    
    /* Initialisation du temps et du texte */
    int tempsActuel = 0, tempsPrecedent = 0, compteur = 0;
    char temps[20] = "";
    
    SDL_Surface* surfaceChiffre = TTF_RenderText_Shaded(police, temps, couleurNoire, couleurBlanche);;
    textureChiffre = SDL_CreateTextureFromSurface(renderer, surfaceChiffre);
    SDL_FreeSurface(surfaceChiffre);
    
    SDL_Rect positionChiffre; //create a rect
    positionChiffre.x = 400;  //controls the rect's x coordinate
    positionChiffre.y = 500; // controls the rect's y coordinte
    tempsActuel = SDL_GetTicks();
    sprintf(temps, "Temps : %d", compteur);
    TTF_SizeText(police, temps, &positionTexte.w, &positionTexte.h);
    
    while(continuer)
    {
        tempsActuel = SDL_GetTicks();
        
        SDL_PollEvent(&event);
        switch (event.type)
        {
            case SDL_QUIT:
                continuer = 0;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE: /* Appui sur la touche Echap, on arrête le programme */
                        continuer = 0;
                        break;

                    case SDLK_q: /* Appui sur la touche Echap, on arrête le programme */
                        timerON = 1;
                        if (timerON)
                        {
                            timer = SDL_AddTimer(50, bougerZozo, &positionZozor); /* Démarrage du timer */
                        }
                        break;

                    case SDLK_p: /* Appui sur la touche Echap, on arrête le programme */
                        timerON = 0;
                        SDL_RemoveTimer(timer); /* Arrêt du timer */
                        break;

                    default:
                        break;
                }
                break;
                
            default:
                break;
        }
        
//            // Timer version SDL_GetTicks
//            tempsActuel = SDL_GetTicks();
//            if (tempsActuel - tempsPrecedent > 100) /* Si 30 ms se sont écoulées depuis le dernier tour de boucle */
//            {
//                positionZozor.x++; /* On bouge Zozor */
//                tempsPrecedent = tempsActuel; /* Le temps "actuel" devient le temps "precedent" pour nos futurs calculs */
//            }
        
        if (tempsActuel - tempsPrecedent >= 100) /* Si 100 ms se sont écoulées depuis le dernier tour de boucle */
        {
            compteur += 100;
            sprintf(temps, "Temps : %d", compteur); /* On écrit dans la chaîne "temps" le nouveau temps */
            //SDL_FreeSurface(surfaceChiffre); /* On supprime la surface précédente */
            
            surfaceChiffre = TTF_RenderText_Shaded(police, temps, couleurNoire, couleurBlanche); /* On écrit la chaîne temps dans la SDL_Surface */
            TTF_SizeText(police, temps, &positionChiffre.w, &positionChiffre.h);
            
            textureChiffre = SDL_CreateTextureFromSurface(renderer, surfaceChiffre);
            SDL_FreeSurface(surfaceChiffre);
            
            tempsPrecedent = tempsActuel; /* On met à jour le tempsPrecedent */
        }

        SDL_RenderClear(renderer); // Clear the entire screen
        SDL_RenderCopy(renderer, textureChiffre, NULL, &positionChiffre);
        SDL_RenderCopy(renderer, textureTexte, NULL, &positionTexte);
        SDL_RenderCopy(renderer, textureZozor, NULL, &positionZozor);
        SDL_RenderPresent(renderer); // Afficher l'image de fond
    }

    // Destruction de tous les élements crées
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TTF_CloseFont(police); /* Doit être avant TTF_Quit() */
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

Uint32 bougerZozo(Uint32 intervalle, void *parametre)
{
    SDL_Rect* positionZozor = parametre; /* Conversion de void* en SDL_Rect* */
    positionZozor->x++;

    return intervalle;
}

void SDL_ExitWithDestruction(SDL_Renderer *renderer, SDL_Window *window, const char *message)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_ExitWithError(message);
}

void SDL_ExitWithError(const char *message)
{
    SDL_Log("Erreur: %s > %s \n", message ,SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}
