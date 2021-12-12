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
    int i,count = 0,mouseX,mouseY,h,tw,th;
    screen current;
    MLV_Button_state LastState = MLV_RELEASED;
    srand(time(NULL));
    MLV_get_desktop_size(&mw,&mh);
    current.max_height = mh;
    current.max_width = mw;
    current.jeu = init_game(current.jeu);
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
    current.id = MENU;
    current.height = height;
    current.width = width;
    current.jeu.colors = 1;
    current.jeu.sound = 1;
    current.jeu.ply_count = 1;
    current.jeu.players[0].name = "";
    current.jeu.players[0].score = 0;
    current = gen_menu(current);
    current.jeu.case_size = height/NB_LINES;
    current.jeu.width = current.jeu.case_size * NB_COLS;
    current.jeu.height = current.jeu.case_size * NB_LINES;
    current.jeu.x = (current.width/2 - (current.jeu.width)/2);
    current.jeu.y = height/100;
    gen_blocks(current.jeu.figures[0].blocks);
	current.jeu.figures[0].x = current.jeu.x + (rand() % (current.jeu.x + current.jeu.width)) - current.jeu.case_size/2;
    current.jeu.figures[0].y = 0;
    MLV_change_frame_rate(30);
    while(1){/* Entrer dans la boucle principale */
        MLV_delay_according_to_frame_rate();/* 30 FPS */
        if(current.id == MENU){
        	/* ON UPDATE L'ARRIERE PLAN */
        	if(count == 29){
				gen_blocks(current.jeu.figures[0].blocks);

				current.jeu.figures[0].x = current.jeu.x + (rand() % (current.jeu.x + current.jeu.width)) - current.jeu.case_size/2;
				current.jeu.figures[0].y = 0;
        	}
        	erase_figure(current.jeu.figures[0],current.jeu.case_size);
        	draw_figure(current.jeu.figures[0],current.jeu.case_size,current.jeu.colors);
        	current.jeu.figures[0].y += current.jeu.case_size;
        	MLV_get_size_of_text_with_font("TETE RISSE",&tw,&th,title_font);
    		MLV_draw_text_with_font(width/2-tw/2,height/10-th/2,"TETE RISSE",title_font,MLV_COLOR_WHITE);
        }
        
        for(i=0;i<current.btncount;i++){
            MLV_draw_text_box_with_font(current.buttons[i].x,current.buttons[i].y,current.buttons[i].width,current.buttons[i].height,current.buttons[i].label,default_font,20,MLV_COLOR_WHITE,MLV_COLOR_BLACK,MLV_COLOR_GREY,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER); 
        }
        MLV_get_mouse_position(&mouseX,&mouseY);
        h = get_hovered_button(current.buttons,mouseX,mouseY,current.btncount);
        if(h != -1){
            MLV_draw_text_box_with_font(current.buttons[h].x,current.buttons[h].y,current.buttons[h].width,current.buttons[h].height,current.buttons[h].label,default_font,20,MLV_COLOR_WHITE,MLV_COLOR_WHITE,MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);
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
            update_seconde(&current);
            count = 0;
        }
        MLV_update_window();
    }  
}



/*
Le systeme du buttons[x][y] ne fonctionne pas à cause des capacité de mémoire ...
Du coup on est passé au système D -> buttons[i] = {x,y,clicked,...}
*/


