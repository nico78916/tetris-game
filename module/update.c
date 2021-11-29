#include <MLV/MLV_all.h>
#include "../header/api.h"
#include "../header/const.h"
#include "../header/graphics.h"
#include "../header/save.h"
#include "../header/game.h"

void quit_game(screen current){
    print("Quitting the game");
    MLV_free_font(default_font);
    MLV_free_font(western_font);
    MLV_free_font(title_font);
    MLV_free_window();
    if(current.jeu.ply_count == 1){
        write_save(current.jeu);
    }
    exit(0);
}

void return_menu(screen* current){
    printf("update.c    return_menu\n");
    *current = gen_menu(*current);
}

void on_click_menu(screen* current,int h){
    printf("update.c    on_click_menu\n");
    switch (h)
    {
    case 4:
        quit_game(*current);
        break;
    case 3:
        *current = gen_option(*current);
        break;
    case 2: 
        *current = gen_load(*current);
        break;
    case 0:
        *current = gen_newgame(*current);
        break;
    default:
        break;
    }
}

void on_click_opts(screen* current,int h){
    printf("update.c    on_click_opts\n");
    switch (h)
    {
    case 0:
        *current = change_resolution(*current,resolution_16_9_w[(int_indexOf(current->width,resolution_16_9_w,NB_RESOLUTION) + 1) %NB_RESOLUTION],resolution_16_9_l[(int_indexOf(current->height,resolution_16_9_l,NB_RESOLUTION) + 1) %NB_RESOLUTION]);
        break;
    case 1:
        *current = switch_widow_type(*current);
        break;
    case 4:
        *current = gen_menu(*current);
        break;
    default:
        break;
    }
}

void on_click_ng(screen* current,int h){
    printf("update.c    on_click_ng\n");
    switch (h)
    {
    case 4:
        print("Returning to menu");
        *current = gen_menu(*current);
        break;
    case 0:
        current->jeu = init_game(current->jeu);
        *current = gen_game(*current);
        *current = gen_over(*current);
        break;
    default:
        return_menu(current);
        break;
    }
}

void on_click_load(screen* current,int h){
    printf("update.c    on_click_ng\n");
    switch (h)
    {
    case 5:
        print("Returning to menu");
        *current = gen_menu(*current);
        break;
    default:
        current->jeu = load_save(h+1);
        *current = gen_game(*current);
        return_menu(current);
        break;
    }
}

void on_click_over(screen* current,int h){
    printf("update.c    on_click_ng\n");
    switch (h)
    {
    case 0:
        *current = gen_pseudo(*current);
        break;
    default:
        current->jeu = load_save(h+1);
        *current = gen_game(*current);
        return_menu(current);
        break;
    }
}

void update_frame(screen* current){
    printf("update.c    update_frame\n");
    if(MLV_get_keyboard_state(MLV_KEYBOARD_LALT) == MLV_PRESSED && MLV_get_keyboard_state(MLV_KEYBOARD_F4) == MLV_PRESSED){
        quit_game(*current);
    }

    if(MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) == MLV_PRESSED){
            return_menu(current);
    }
}

void on_click(screen* current,int h){
    printf("update.c    on_click\n");
    switch (current->id)
    {
    case MENU:
        print("Click on menu");
        on_click_menu(current,h);
        break;
    case OPTIONS:
        on_click_opts(current,h);
        break;
    case NEWGAME:
        on_click_ng(current,h);
        break;
    case LOAD:
        on_click_load(current,h);
    case OVER:
        on_click_over(current,h);
    break;
    default:
        break;
    }
}

void update_seconde(screen* current){
    /*printf("update.c    update_seconde\n");*/
    current->id;
    return;
}
