#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>
#include "lists/string.h"

#define DEFAULT_SCREEN_WIDTH 500
#define DEFAULT_SCREEN_HEIGHT 500

void usage(){
    printf("Usage : ./main [-f] [-size <longueur> <hauteur> (deux entiers supérieurs à 100)]\n");
}

/**
 * Crée une nouvelle fenêtre et met à jour width et height
 * @param name Le nom de la fenêtre
 * @param icon Nom de l'icône
 * @param width la variable pour assigner la nouvelle longueur d'écran
 * @param height la variable pour assigner la nouvelle hauteur d'écran
 */
void create_fullscreen(char* name,char* icon,unsigned *width,unsigned *height){
    unsigned int w = *width,h = *height;
    MLV_get_desktop_size(width,height);
    MLV_create_full_screen_window(name,icon,w,h);
}

int main(int argc,char** argv){
    unsigned int width = DEFAULT_SCREEN_WIDTH,height = DEFAULT_SCREEN_HEIGHT;/* ils ne seront jamais négatif*/ 
    int i;
    if(argc > 1){/*On check si l'utilisateur veut quelque chose de spécial*/
        if((i=indexOf("-f",argv,argc)) != -1){/* i représente l'index du parametre (si != -1) */
            create_fullscreen("MENU","Menu principale",&width,&height);
        }
        if((i=indexOf("-size",argv,argc)) != -1){/* custom size d'écran */
            width = atoi(argv[i+1]);
            height = atoi(argv[i+2]);
            if(width < 100 || height < 100){
                width = DEFAULT_SCREEN_WIDTH;
                height = DEFAULT_SCREEN_HEIGHT;
            }
            MLV_create_window("MENU","Menu principal",width,height);
        }
    }else{
       MLV_create_window("MENU","Menu principal",width,height); 
    }
    printf("Screen size %dx%d\n",width,height);
    
    MLV_wait_seconds(10);
    MLV_free_window();
    exit(0);
}