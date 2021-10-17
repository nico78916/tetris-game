/* Ce fichier definit tout les écrans du jeu */
#include <MLV/MLV_all.h>

void create_main(screen *main,unsigned width,unsigned height){
    int i,lastY,sizeX,sizeY,label_width,label_height;
    button *options;
    MLV_Font *font = MLV_load_font("ressources/main_font.ttf",20);

    MLV_clear_window(MLV_COLOR_BLACK);
    main->title = "SHOOTRIS";
    create_button(&(main->buttons),1,width/2,0,0,height/50,width/2,0,"NOUVELLE PARTIE",MLV_rgba(255,255,255,255),MLV_rgba(200,200,200,255),font);
    create_button(&(main->buttons),2,width/2,0,0,height/50,width/2,0,"CHARGER",MLV_rgba(255,255,255,255),MLV_rgba(200,200,200,255),font);
    create_button(&(main->buttons),3,width/2,0,0,height/50,width/2,0,"TABLEAU DES SCORES",MLV_rgba(255,255,255,255),MLV_rgba(200,200,200,255),font);
    create_button(&(main->buttons),4,width/2,0,0,height/50,width/2,0,"OPTIONS",MLV_rgba(255,255,255,255),MLV_rgba(200,200,200,255),font);
    create_button(&(main->buttons),0,width/2,0,0,height/50,width/2,0,"QUITTER",MLV_rgba(255,255,255,255),MLV_rgba(200,200,200,255),font);
    lastY = height/(&(main->buttons))->length;
    for(i=0;i<(&(main->buttons))->length;i++){
        options = &(&(main->buttons))->values[i];
        get_button_size(options,&sizeX,&sizeY,font);
        MLV_get_size_of_text_with_font(options->label,&label_width,&label_height,font);
        options->y = lastY;
        options->width = sizeX;
        options->height = sizeY;
        options->x = sizeX/2;
        lastY += sizeY + (options->margeY*2) + 10;
    }
    create_textbox(&(main->texts_box),0,width/2,lastY,0,0,width/2,0,"ENTREZ VOTRE NOM",MLV_rgba(255,255,255,255),MLV_rgba(200,200,200,255),font);
    main->title_font = font;
    main->width = width;
    main->height = height;
    draw_screen(main);
}

void create_solo(screen *game,unsigned width,unsigned height){
    score score;
    score.text = "0";
    game->score = score;
    return;
}