#ifndef SDL_PROJET_H
#define SDL_PROJET_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

//define booleen
typedef int bool;
#define TRUE  1
#define FALSE 0

//Resolution d'ecran
#define  SCREEN_WIDTH  640
#define  SCREEN_HEIGHT  480
#define  SCREEN_BPP  32

//Declaration de surface et fenetre
extern SDL_Surface *gpScreen;

extern SDL_Renderer *renderer;
//Declaration de l'evenement
extern SDL_Event myEvent;

//Declaration de police
extern TTF_Font *font;

/*Declarer les couleurs communes*/
extern const SDL_Color RGB_Black;
extern const SDL_Color RGB_Red;
extern const SDL_Color RGB_Green;
extern const SDL_Color RGB_Blue;
extern const SDL_Color RGB_Yellow;
extern const SDL_Color RGB_White;

/*Declaration les fonctions*/
extern bool init();
extern bool create_fenetre(char* titre, SDL_Window* window);
extern SDL_Texture* load_image(char * filename,SDL_Renderer * renderer);
extern void apply_surface(int x, int y, SDL_Texture *tex, SDL_Renderer *rend);
extern SDL_Texture* RenderText(char* message, char* fontFile, SDL_Color color, int fontSize);
#endif
