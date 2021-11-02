#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Les includes peuvent nécessiter la fonction print(); */
#include <MLV/MLV_all.h>
#include <math.h>
#include <time.h>
#include "./header/string.h"
#include "./header/const.h"
#include "./header/save.h"
#include "./header/game.h"
#include "./header/graphics.h"
#include "./header/update.h"
#include "./header/api.h"

void usage(char *nom){
    printf("Usage : %s [-f] [-size <longueur> <hauteur> (deux entiers supérieurs à 100)]\n",nom);
}

/**
 * Crée une nouvelle fenêtre et met à jour width et height
 * @param name Le nom de la fenêtre
 * @param icon Nom de l'icône
 * @param width la variable pour assigner la nouvelle longueur d'écran
 * @param height la variable pour assigner la nouvelle hauteur d'écran
 */
int main(int argc,char** argv){
    unsigned int mw,mh,width = DEFAULT_SCREEN_WIDTH,height = DEFAULT_SCREEN_HEIGHT;/* ils ne seront jamais négatif*/ 
    int i,count = 0,mouseX,mouseY,h;
    button btn[MAX_BUTTON];
    screen current;
    MLV_Button_state LastState = MLV_RELEASED;
    int press_count = 0;
    MLV_get_desktop_size(&mw,&mh);
    current.max_height = mh;
    current.max_width = mw;
    if(argc > 1){
        /*On check si l'utilisateur veut quelque chose de spécial*/
        if((i=indexOf("-f",argv,argc)) != -1){
            if(mw > MAX_SCREEN_WIDTH || mh > MAX_SCREEN_HEIGHT){
                mw = MAX_SCREEN_WIDTH; mh = MAX_SCREEN_HEIGHT;
            }
            width = mw;
            height = mh;
            MLV_create_full_screen_window("TÊTE RISSE","TR",mw,mh);
        }
        if((i=indexOf("-size",argv,argc)) != -1){
            width = atoi(argv[i+1]);
            height = atoi(argv[i+2]);
            if(width < 100 || height < 100){
                width = DEFAULT_SCREEN_WIDTH;
                height = DEFAULT_SCREEN_HEIGHT;
            }
            MLV_create_window("TÊTE RISSE","TR",width,height);
        }
    }else{
       MLV_create_window("TÊTE RISSE","TR",width,height); 
    } 
    default_font = MLV_load_font("./ressources/default.ttf",20);
    western_font = MLV_load_font("./ressources/main_font.ttf",20);
    title_font = MLV_load_font("./ressources/main_font.ttf",50);
    current.buttons = btn;
    current.id = MENU;
    current.height = height;
    current.width = width;
    current = gen_menu(current);
    while(1){/* Entrer dans la boucle principale */
        MLV_wait_milliseconds(1000/30);/* 30 FPS */
        for(i=0;i<current.btncount;i++){
            MLV_draw_text_box_with_font(btn[i].x,btn[i].y,btn[i].width,btn[i].height,btn[i].label,default_font,20,MLV_COLOR_WHITE,MLV_COLOR_BLACK,MLV_COLOR_GREY,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER); 
        }
        MLV_get_mouse_position(&mouseX,&mouseY);
        h = get_hovered_button(btn,mouseX,mouseY,current.btncount);
        if(h != -1){
            MLV_draw_text_box_with_font(btn[h].x,btn[h].y,btn[h].width,btn[h].height,btn[h].label,default_font,20,MLV_COLOR_WHITE,MLV_COLOR_WHITE,MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);
        }
        if(MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED){
            if(LastState == MLV_RELEASED){
                current.pressed = MLV_PRESSED;
                on_click(&current,h);
                LastState = MLV_PRESSED;
            }else{
                current.pressed = MLV_RELEASED;
            }
        }else{
            LastState = MLV_RELEASED;
            current.pressed = MLV_RELEASED;
            current.long_press = MLV_RELEASED;
        }
        update_frame(&current);
        count++;
        if(count >= 30){
            update_seconde();
            count = 0;
        }
        MLV_update_window();
    }  
}



/*
Le systeme du buttons[x][y] ne fonctionne pas à cause des capacité de mémoire ...
Du coup on est passé au système D -> buttons[i] = {x,y,clicked,...}
*/


