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
    /*MLV_free_music(current.cursong);*/
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
        *current = gen_menu(*current);
        break;
    case OPTIONS:
    *current = gen_option(*current);
    break;
    case GAME:
        *current = gen_game(*current);
    break;
    case NEWGAME:
        *current = gen_newgame(*current);
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
   printf("update.c    on_click_menu\n");
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
    default:
        /* écran réservé pour le multi mais ce dernié n'a pas été implémenté
        gen_screen(current,NEWGAME);*/
        current->jeu = init_game(current->jeu);
        gen_screen(current,GAME);
        gen_screen(current,current->last_screen_id);
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
    case 3:
        toggleColor(current);
        *current = gen_option(*current);/* on ne veut pas changer l'encien écran */
        break;
    case 2:
        toggleSound(current);
        *current = gen_option(*current);
        break;
    case 4:
        gen_screen(current,MENU);
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
        gen_screen(current,MENU);
        break;
    case 0:
        current->jeu = init_game(current->jeu);
        gen_screen(current,GAME);
        gen_screen(current,current->last_screen_id);
        break;
    default:
        gen_screen(current,MENU);
        break;
    }
}

void on_click_load(screen* current,int h){
   printf("update.c    on_click_ng\n");
    switch (h)
    {
    case 5:
        gen_screen(current,MENU);
        break;
    default:
        current->jeu.slot = h+1;
        load_save(current);
        gen_screen(current,GAME);
        gen_screen(current,current->last_screen_id);
        break;
    }
}

void on_click_save(screen* current,int h){
   printf("update.c    on_click_ng\n");
    switch (h)
    {
    case 5:
        gen_screen(current,PAUSE);
        break;
    default:
        current->jeu.slot = h+1;
        write_save(&current->jeu);
        current->last_screen_id = GAME;
        gen_screen(current,PAUSE);
        break;
    }
}

void on_click_over(screen* current,int h){
   printf("update.c    on_click_ng\n");
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
   printf("update.c    on_click_ng\n");
    switch (h)
    {
    case 0:
        gen_screen(current,SAVE);
        break;
    default:
        load_save(current);
        gen_screen(current,GAME);
        break;
    }
}

void update_frame(screen* current){
    if(MLV_get_keyboard_state(MLV_KEYBOARD_LALT) == MLV_PRESSED && MLV_get_keyboard_state(MLV_KEYBOARD_F4) == MLV_PRESSED){
        quit_game(current);
    }

    if(MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) == MLV_PRESSED){
            while(MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) == MLV_PRESSED);
            gen_screen(current,current->last_screen_id);
    }
}

void on_click(screen* current,int h){
   printf("update.c    on_click\n");
    switch (current->id)
    {
    case MENU:
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

void update_seconde(screen* current){
    srand(time(NULL));
    return;
}
