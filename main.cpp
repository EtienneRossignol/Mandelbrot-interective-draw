#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include "mandelbrot.h"
#include <windows.h>

SDL_Surface *ecran;
int positionX,positionY;
double X_deb=-2.2,Y_deb=-1.08;
double xD,yD;
int echelle=2;

bool clique=false;
void gestiondesevenements();

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    ecran = SDL_SetVideoMode(1000,1000, 32,
                             SDL_HWSURFACE);
    surface = SDL_CreateRGBSurface(0,1000,1000,32,0xff000000,0x00ff0000,0x0000ff00,0);
    Mandel(X_deb,Y_deb,echelle);
    //
    //while(true);
    gestiondesevenements();

    return EXIT_SUCCESS;

}

HANDLE Calcul;

unsigned long WINAPI appelPCacul(void *param)
{
    SDL_WM_SetCaption("En cours","");
    Mandel(xD,yD, echelle);
}

unsigned long WINAPI appelCacul(void *param)
{
    SDL_WM_SetCaption("Fixe","");
    Mandel(X_deb,Y_deb, echelle);
}

void gestiondesevenements()
{
    SDL_Rect position,decoupe;
    position.x = -10;
    position.y = -20;
    decoupe.x=0;
    decoupe.y=0;
    decoupe.w=1000;
    decoupe.h=1000;

    valeurEchelle[0]=.01;
    for (int i=1;i<1000;i++)valeurEchelle[i]=valeurEchelle[i-1]/2;
    SDL_Event event;
    while (true)
    {
        int calculDEmande=0;

         while ( SDL_PollEvent(&event) )
            {

        switch(event.type)
        {
            case SDL_QUIT:
                return;
            case SDL_MOUSEBUTTONDOWN:
            {
                if(event.button.button==SDL_BUTTON_RIGHT)return;
                if(event.button.button==SDL_BUTTON_LEFT)
                {
                    clique=true;
                    positionX = event.motion.x;
                    positionY = event.motion.y;
                    decoupe.x=0;
                    decoupe.y=0;
                    decoupe.w=1000;
                    decoupe.h=1000;
                }
                    if(event.button.button==SDL_BUTTON_WHEELDOWN)
                {
                    X_deb+=(event.motion.x-1000/4)*valeurEchelle[echelle];
                    Y_deb+=(event.motion.y-1000/4)*valeurEchelle[echelle];
                    if (echelle)
                    echelle--;
                    calculDEmande=2;
                    break;
                }
                    else if (event.button.button==SDL_BUTTON_WHEELUP)
                {
                    X_deb+=(event.motion.x-1000/4)*valeurEchelle[echelle];
                    Y_deb+=(event.motion.y-1000/4)*valeurEchelle[echelle];
                    if (echelle<1000)
                    echelle++;
                    calculDEmande=2;
                }
                break;
            }
            case SDL_MOUSEMOTION:
            {
                if (clique)
                {
                    calculDEmande=1;
                    xD=X_deb+(positionX-event.motion.x)*valeurEchelle[echelle];
                    yD=Y_deb+(positionY-event.motion.y)*valeurEchelle[echelle];
                    calculDEmande=1;
                    position.x=event.motion.x-positionX;
                    position.y=event.motion.y-positionY;
                    if (position.x<0){decoupe.x=-position.x;
                                        decoupe.w=1000+position.x;}
                    else
                    {
                        decoupe.x=0;
                    }
                    if (position.y<0){decoupe.y=-position.y;
                                        decoupe.h=1000+position.y;}
                    else
                    {
                        decoupe.y=0;
                    }
                }
                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                if(event.button.button==SDL_BUTTON_LEFT)
                {
                    clique=false;
                    X_deb+=(positionX-event.motion.x)*valeurEchelle[echelle];
                    Y_deb+=(positionY-event.motion.y)*valeurEchelle[echelle];
                    calculDEmande=2;
                }

                }
        }}
        if (calculDEmande==2)
        {
            decoupe.x=0;
            decoupe.y=0;
            decoupe.w=1000;
            decoupe.h=1000;
            position.x=0;
            position.y=0;
            TerminateThread(Calcul,0);
            Calcul=CreateThread(NULL,0,appelCacul,0,0,0);
        }
        else if (calculDEmande==1)
        {
            TerminateThread(Calcul,0);
            //Calcul=CreateThread(NULL,0,appelPCacul,0,0,0);
        }
        SDL_FillRect(ecran, 0, SDL_MapRGB(ecran->format, 0,0,0));
        SDL_BlitSurface(surface, &decoupe, ecran, &position);
        SDL_Flip(ecran);
        //SDL_Delay(200);
    }

}
