#include<cmath>

double valeurEchelle[1000];
double x,y,precision;
int __x=0,__y=0;
SDL_Surface *surface;

void definirPixel(int x, int y, Uint32 pixel)
{
    /*nbOctetsParPixel représente le nombre d'octets utilisés pour stocker un pixel.
    En multipliant ce nombre d'octets par 8 (un octet = 8 bits), on obtient la profondeur de couleur
    de l'image : 8, 16, 24 ou 32 bits.*/
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    /*Ici p est l'adresse du pixel que l'on veut modifier*/
    /*surface->pixels contient l'adresse du premier pixel de l'image*/
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;

    /*Gestion différente suivant le nombre d'octets par pixel de l'image*/
    switch(nbOctetsParPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            /*Suivant l'architecture de la machine*/
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}


int inline min(int el) {
    if (el<0)return 0;
    if (el >255)return 255;
    return el;}

int inline diverge()
{
    long double posR=0,posI=0,LPosR;
    for(int iter=0; iter<precision; iter++){
        LPosR=posR;
        posR=posR*posR-posI*posI + x;
        posI=2*LPosR*posI +y;
        if (posR*posR+posI*posI>25)
        {
            definirPixel( __x, __y, SDL_MapRGBA(surface->format, min(iter*3+10),min(iter*3+5),min(255-iter*2), 0));
            return 1;
        }
    }
    definirPixel( __x, __y, SDL_MapRGBA(surface->format, 25,0,5, 0));
    }

void Mandel(double Xdeb,double Ydeb, int e)
{
    double echelle=valeurEchelle[e];
    int NivPrecision[4][3]={1000,1000,10,1000,1000, 50,1000,1000, 400,1000,1000,2000};
    for (int i=0;i<4;i++)
    {   __x=0,__y=0;
        int XR=NivPrecision[i][0];
        double nbPix=ceil(XR/double(XR));
        precision=NivPrecision[i][2];
        double Aug=nbPix * echelle;

        for (x=Xdeb;__x<1000;x+=Aug){
                __x++;
                __y=0;
            for (y=Ydeb;__y<1000;y+=Aug){diverge();__y++;}
            }
    }
}
