#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
* ATTENTION cette fonction ne permet pas de formater la chaine (d'où le print sans f... logique ! enfin je crois ...)
*/
int print(char*a){
    return printf("%s \n",a);
}

/* Les includes peuvent nécessiter la fonction print(); */
#include <MLV/MLV_all.h>
#include <math.h>
#include "string.h"
#include "int.h"
#include "const.h"
#include "graphics.h"

void usage(char *nom){
    printf("Usage : %s [-f] [-size <longueur> <hauteur> (deux entiers supérieurs à 100)]\n",nom);
}

void update_seconde(){
    return;
}

void quit_game(){
    MLV_free_font(default_font);
    MLV_free_font(western_font);
    MLV_free_font(title_font);
    exit(0);
}

void update_menu(screen* current,int mouseX,int mouseY){
    int i,h;
    button* btn = current->buttons;
    print("updating_menu");
    for(i=0;i<current->btncount;i++){
       MLV_draw_text_box_with_font(btn[i].x,btn[i].y,btn[i].width,btn[i].height,btn[i].label,default_font,20,MLV_COLOR_WHITE,MLV_COLOR_BLACK,MLV_COLOR_GREY,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER); 
    }
    h = get_hovered_button(btn,mouseX,mouseY,current->btncount);
    if(h != -1){
        MLV_draw_text_box_with_font(btn[h].x,btn[h].y,btn[h].width,btn[h].height,btn[h].label,default_font,20,MLV_COLOR_WHITE,MLV_COLOR_WHITE,MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);
        if(current->pressed == MLV_PRESSED){
            switch (h)
            {
            case 3:
                quit_game();
                break;
            case 2:
                *current = gen_option(*current);
                break;
            default:
                break;
            }
        }
    }
}


void update_option(screen* current,int mouseX,int mouseY){
    int i,h;
    char res[MAX_STR];
    button* btn = current->buttons;
    print("CHECKING");
    get_screen_resolution_text(*current,res);
    btn[0].label = res;
    for(i=0;i<current->btncount;i++){
       MLV_draw_text_box_with_font(btn[i].x,btn[i].y,btn[i].width,btn[i].height,btn[i].label,default_font,20,MLV_COLOR_WHITE,MLV_COLOR_BLACK,MLV_COLOR_GREY,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER); 
    }
    h = get_hovered_button(btn,mouseX,mouseY,current->btncount);
    if(h != -1){
        MLV_draw_text_box_with_font(btn[h].x,btn[h].y,btn[h].width,btn[h].height,btn[h].label,default_font,20,MLV_COLOR_WHITE,MLV_COLOR_WHITE,MLV_COLOR_BLACK,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);
        if(current->pressed == MLV_PRESSED){
            switch (h)
            {
            case 4:
                *current = gen_menu(*current);
                break;
            case 0:
                *current = change_resolution(*current,resolution_16_9_w[(int_indexOf(current->width,resolution_16_9_w,7) + 1) %7],resolution_16_9_l[(int_indexOf(current->height,resolution_16_9_l,7) + 1) %7]);
                break;
            default:
                break;
            }
        }
    }
}

void update_frame(screen* current){
    int mouseX,mouseY;
    if(MLV_get_keyboard_state(MLV_KEYBOARD_LALT) == MLV_PRESSED && MLV_get_keyboard_state(MLV_KEYBOARD_F4) == MLV_PRESSED){
        MLV_free_window();
        exit(0);
    }
    print("update_frame");
    MLV_get_mouse_position(&mouseX,&mouseY);
    switch (current->id)
    {
    case MENU:
        update_menu(current,mouseX,mouseY);
        break;

    case OPTIONS:
        update_option(current,mouseX,mouseY);
        break;
    
    default:
        break;
    }
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
    button btn[MAX_BUTTON];
    screen current;
    MLV_Button_state LastState = MLV_RELEASED;
    int press_count = 0;
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
        update_frame(&current);
        if(MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED){
            if(LastState == MLV_PRESSED){
                press_count++;
                if(press_count >= 3){
                    current.pressed = MLV_RELEASED;
                }
                if(press_count >= 15){
                    current.long_press = MLV_PRESSED;
                }
            }else{
                LastState = MLV_PRESSED;
                current.pressed = MLV_PRESSED;
            }
        }else{
            LastState = MLV_RELEASED;
            current.pressed = MLV_RELEASED;
            current.long_press = MLV_RELEASED;
        }
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


