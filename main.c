#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>
#include "lists/string.h"
#include "const.h"
#include "typedef.h"
#include "struct.h"
#include "graphics.h"
#include "screen.h"



void usage(){
    printf("Usage : ./main [-f] [-size <longueur> <hauteur> (deux entiers supérieurs à 100)]\n");
}


void init_screen(screen *scr){
    button bu[TAILLE_BOUTON];
    button_list btla;
    textbox b[TAILLE_TEXTBOX];
    textbox_list btl;


    btla.length = 0;
    btla.values = bu;

    btl.length = 0;
    btl.values = b;

    scr->buttons = btla;
    scr->texts_box = btl;
}

void update_seconde(screen *cur,screen** screens){
    return;
}

void update_frame(screen *cur,screen** screens){
    int mouseX,mouseY;
    if(MLV_get_keyboard_state(MLV_KEYBOARD_LALT) == MLV_PRESSED && MLV_get_keyboard_state(MLV_KEYBOARD_F4) == MLV_PRESSED){
        MLV_free_window();
        exit(0);
    }
    MLV_get_mouse_position(&mouseX,&mouseY);
    hover_button(&(cur->buttons),mouseX,mouseY);
}



/**
 * Crée une nouvelle fenêtre et met à jour width et height
 * @param name Le nom de la fenêtre
 * @param icon Nom de l'icône
 * @param width la variable pour assigner la nouvelle longueur d'écran
 * @param height la variable pour assigner la nouvelle hauteur d'écran
 */
int main(int argc,char** argv){
    unsigned int width = DEFAULT_SCREEN_WIDTH,height = DEFAULT_SCREEN_HEIGHT;/* ils ne seront jamais négatif*/ 
    int i,count = 0;
    char max[MAX_STR];
    screen *menu,*solo,*pause,*options;
    screen *current;
    screen *screens[NB_SCREEN];
    button bt[TAILLE_BOUTON];
    button_list btl;
    textbox tb[TAILLE_TEXTBOX];
    textbox_list tbl;
    screen vide;

    btl.length = 0;
    btl.values = bt;

    tbl.length = 0;
    tbl.values = tb;
    
    vide.buttons = btl;
    vide.texts_box = tbl;
    vide.title = max;

    menu = &vide;
    options = &vide;
    pause = &vide;
    solo = &vide;

    screens[MAIN_SCREEN] = menu;
    screens[OPTION_SCREEN] = options;
    screens[PAUSE_SCREEN] = pause;
    screens[SOLO_SCREEN] = solo;

    if(argc > 1){
        /*On check si l'utilisateur veut quelque chose de spécial*/
        if((i=indexOf("-f",argv,argc)) != -1){
            MLV_get_desktop_size(&width,&height);
            if(width > MAX_SCREEN_WIDTH || height > MAX_SCREEN_HEIGHT){
                width = MAX_SCREEN_WIDTH; height = MAX_SCREEN_HEIGHT;
            }
            MLV_create_full_screen_window("Menu","Menu",width,height);
        }
        if((i=indexOf("-size",argv,argc)) != -1){
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



    create_main(menu,width,height);/* création du menu principal */
    current = menu;
    
    while(1){/* Entrer dans la boucle principale */
        MLV_wait_milliseconds(1000/30);/* 30 FPS */
        update_frame(current,screens);
        MLV_update_window();
        count++;
        if(count >= 30){
            update_seconde(current,screens);
            count = 0;
        }
    }
    
}



/*
Le systeme du buttons[x][y] ne fonctionne pas à cause des capacité de mémoire ...
Du coup on est passé au système D -> buttons[i] = {x,y,clicked,...}
*/