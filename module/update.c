#include <MLV/MLV_all.h>
#include <time.h>
#include "../header/api.h"
#include "../header/const.h"
#include "../header/graphics.h"
#include "../header/save.h"
#include "../header/game.h"

void quit_game(screen* current){
    print("Quitting the game");
    MLV_free_font(default_font);
    MLV_free_font(western_font);
    MLV_free_font(title_font);
    MLV_stop_music();
    MLV_free_audio();
    MLV_free_window();
    if(current->jeu.ply_count == 1){
        write_save(&current->jeu);
    }
    exit(0);
}

void gen_screen(screen *current,screen_id id){
    current->last_screen_id = current->id;
    current->id = id;
    switch (id)
    {
    case MENU:
        if(current->jeu.sound == 1){
            MLV_stop_music();
            current->cursong = MLV_load_music("ressources/title.wav");
            MLV_play_music(current->cursong,1.0,1);
        }
        *current = gen_menu(*current);
        break;
    case OPTIONS:
    *current = gen_option(*current);
    break;
    case GAME:
        MLV_stop_music();
        *current = gen_game(*current);
    break;
    case LOAD:
        *current = gen_load(*current);
    break;
    case SAVE :
        *current = gen_save(*current);
    break;
    case PSEUDO :
        *current = gen_pseudo(*current);
    break;
    case OVER:
        *current = gen_over(*current);
    break;
    case PAUSE:
        *current = gen_pause(*current);
    break;
    default:
        print("L'identifiant de l'écran n'est pas connu");
        exit(-1);
        break;
    }

}

void return_menu(screen* current){
    gen_screen(current,MENU);
}

void on_click_menu(screen* current,int h){
    switch (h)
    {
    case 4:
        quit_game(current);
        break;
    case 3:
        gen_screen(current,OPTIONS);
        break;
    case 2: 
        gen_screen(current,LOAD);
        break;
    case 1:
        current->jeu.slot = 0;
        load_save(current);
        gen_screen(current,GAME);
        gen_screen(current,current->last_screen_id);
        break;
    
    case 0:
        current->jeu = init_game(current->jeu);
        current->jeu.players[0].score = 0;
        current->jeu.slot = 0;
        gen_screen(current,GAME);
        gen_screen(current,current->last_screen_id);
        break;
    }
}

void on_click_opts(screen* current,int h){
    switch (h)
    {
    case 0:
        *current = change_resolution(*current,resolution_16_9_w[(int_indexOf(current->width,resolution_16_9_w,NB_RESOLUTION) + 1) %NB_RESOLUTION],resolution_16_9_l[(int_indexOf(current->height,resolution_16_9_l,NB_RESOLUTION) + 1) %NB_RESOLUTION]);
        current->id = current->last_screen_id;
        gen_screen(current,OPTIONS);
        break;
    case 1:
        *current = switch_widow_type(*current);current->id = current->last_screen_id;
        gen_screen(current,OPTIONS);
        break;
    case 3:
        toggleColor(current);
        *current = gen_option(*current);
        break;
    case 2:
        toggleSound(current);
        *current = gen_option(*current);
        break;
    case 4:
        gen_screen(current,current->last_screen_id);
        break;
    default:
        break;
    }
}

void on_click_load(screen* current,int h){
    switch (h)
    {
    case 5:
        gen_screen(current,MENU);
        break;
    default:
        if(h == -1) break;
        current->jeu.slot = h+1;
        load_save(current);
        current->jeu.slot = 0;
        gen_screen(current,GAME);
        gen_screen(current,current->last_screen_id);
        break;
    }
}

void on_click_save(screen* current,int h){
    switch (h)
    {
    case 5:
        gen_screen(current,PAUSE);
        break;
    default:
        current->jeu.slot = h+1;
        write_save(&current->jeu);
        current->jeu.slot = 0;
        current->last_screen_id = GAME;
        gen_screen(current,PAUSE);
        break;
    }
}

void on_click_over(screen* current,int h){
    switch (h)
    {
    case 0:
        if(strcmp(current->buttons[0].label,"RETOURNER AU MENU") == 0){
            gen_screen(current,MENU);
            current->last_screen_id = MENU;
        }else{
            gen_screen(current,PSEUDO);
            gen_screen(current,MENU);
        }
        break;
    default:
        gen_screen(current,MENU);
        break;
    }
}

void on_click_pause(screen* current,int h){
    switch (h)
    {
    case 0:
        gen_screen(current,SAVE);
        break;
    case 1:
        gen_screen(current,OPTIONS);
        break;
    default:
        load_save(current);
        current->id = PAUSE;
        gen_screen(current,GAME);
        gen_screen(current,current->last_screen_id);
        break;
    }
}

void update_frame(screen* current){
    if(MLV_get_keyboard_state(MLV_KEYBOARD_LALT) == MLV_PRESSED && MLV_get_keyboard_state(MLV_KEYBOARD_F4) == MLV_PRESSED){
        quit_game(current);
    }

    if(MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) == MLV_PRESSED){
            while(MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) == MLV_PRESSED);
            if(current->last_screen_id == GAME && (current->id != OVER && current->id != PSEUDO)){
                load_save(current);
                gen_screen(current,GAME);
            }
            if(current->id != OVER && current->id != PSEUDO){
                gen_screen(current,current->last_screen_id);
            }
    }
}

void on_click(screen* current,int h){
    switch (current->id)
    {
    case MENU:
        on_click_menu(current,h);
        break;
    case OPTIONS:
        on_click_opts(current,h);
        break;
    case LOAD:
        on_click_load(current,h);
        break;
    case OVER:
        on_click_over(current,h);
    break;
    case SAVE:
        on_click_save(current,h);
        break;
    case PAUSE:
        on_click_pause(current,h);
    default:
        break;
    }
}

void update_seconde(){
    srand(time(NULL));
    return;
}
