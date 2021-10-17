#include <math.h>

void create_solo(screen*,unsigned,unsigned);
void create_main(screen*,unsigned,unsigned);

void get_button_size(button* btn,int *sizeX,int *sizeY,MLV_Font* font){
    int label_width,label_height,margeY,margeX;  
    margeY = btn->margeY * 2;/*(haut + bas)*/
    margeX = btn->margeX *2;/*(droite + gauche)*/
    MLV_get_size_of_text_with_font(btn->label,&label_width,&label_height,font);
    if(btn->width < label_width){
        btn->width = label_width + margeX;
    }
    if(btn->height < label_height){
        btn->height = label_height + margeY;
    }
    *sizeY = btn->height;
    *sizeX = btn->width;
        
}

/**
 * Crée un bouton sur la fenêtre courante au coordonnées données
 * @param options Les options du boutton
 * @param table Le tableau des buttons
 * @param width Taille de l'écran
 * @param height Taille de l'écran
 */
void draw_buttons(button_list *table){
    int label_width,label_height,i;
    MLV_Color color;
    button* options;
    for(i=0;i<table->length;i++){
        options = &table->values[i];
        if(options->hide == TRUE){/* On ne souhaite pas afficher un bouton caché :) */
            continue;
        }
        if(options->x >= MAX_SCREEN_WIDTH || options->y >= MAX_SCREEN_HEIGHT){
            printf("[Graphics.h | draw_buttons] Position Out Of Screen ! (x=%d,y=%d) for i=%d\n",options->x,options->y,i);
            exit(-1);
        }
        if(options->drawn && options->hovered){
            color = options->hovercolor;
        }else{
            color = options->color;
        }
        MLV_get_size_of_text_with_font(options->label,&label_width,&label_height,options->font);
        MLV_draw_filled_rectangle(options->x,options->y,options->width,options->height,color);
        MLV_draw_text_with_font(options->x + options->width/2 - label_width/2,options->y + options->height/2 - label_height/2,options->label,options->font,MLV_COLOR_BLACK);
        options->drawn = TRUE;
    }
    MLV_update_window();
}

/**
 * Crée un bouton sur la fenêtre courante au coordonnées données
 * @param table la liste des textbox
 */
/* A FINIR */
void draw_textbox(textbox_list *table){
    int label_width,label_height,i;
    MLV_Color color;
    textbox* options;
    for(i=0;i<table->length;i++){
        options = &table->values[i];
        if(options->hide == TRUE){/* On ne souhaite pas afficher un bouton caché :) */
            continue;
        }
        if(options->x - options->width >= MAX_SCREEN_WIDTH || options->y-options->height >= MAX_SCREEN_HEIGHT){
            printf("[Graphics.h|draw_textbox] text box may be out of screen ! (x=%d,y=%d) for i=%d\n",options->x,options->y,i);
            exit(-1);
        }
        if(options->drawn && options->hovered){
            color = options->hovercolor;
        }else{
            color = options->color;
        }
        MLV_get_size_of_text_with_font(options->placeholder,&label_width,&label_height,options->font);
        if(options->width < label_width + options->margeX * 2){
            options->width = label_width + options->margeX * 2;
        }
        if(options->height < label_height + options->margeY * 2){
            options->height = label_height + options->margeY * 2;
        }
        MLV_draw_filled_rectangle(options->x,options->y,options->width,options->height,color);
        MLV_draw_text_with_font(options->x + options->width/2 - label_width/2,options->y + options->height/2 - label_height/2,options->placeholder,options->font,options->hovercolor);
        options->drawn = TRUE;
    }
    MLV_update_window();
}


void create_button(button_list *table,int id,unsigned x,unsigned y,unsigned margeX,unsigned margeY,int width,int height,char* label,MLV_Color color,MLV_Color hovercolor, MLV_Font *font){
    button b;
    if(table->length == TAILLE_BOUTON ){
        printf("IMPOSSIBLE D'AJOUTER PLUS DE BOUTONS, pour régler le problème changez TAILLE_BOUTON\n");
        exit(-1);
    }
    /*btn = (button){x,y,margeX,margeY,width,height,FALSE,FALSE,label,color,hovercolor};*/
    b.x = x;
    b.y = y;
    b.margeX = margeX;
    b.margeY = margeY;
    b.width = width;
    b.height = height;
    b.hovered = FALSE;
    b.hide = FALSE;
    b.drawn = FALSE;
    b.label = label;
    b.color = color;
    b.hovercolor = hovercolor;
    b.font = font;
    b.id = id;

    table->values[table->length] = b;
    table->length++;
}

void create_textbox(textbox_list *table,int id,unsigned x,unsigned y,unsigned margeX,unsigned margeY,int width,int height,char* placeholder,MLV_Color color,MLV_Color hovercolor, MLV_Font *font){
    textbox b;
    char val[MAX_STR];
    if(table->length == TAILLE_TEXTBOX ){
        printf("IMPOSSIBLE D'AJOUTER PLUS DE TEXTBOX, pour régler le problème changez TAILLE_TEXTBOX ou videz le tableau\n");
        exit(-1);
    }
    /*btn = (button){x,y,margeX,margeY,width,height,FALSE,FALSE,label,color,hovercolor};*/
    b.x = x;
    b.y = y;
    b.margeX = margeX;
    b.margeY = margeY;
    b.width = width;
    b.height = height;
    b.hovered = FALSE;
    b.hide = FALSE;
    b.drawn = FALSE;
    b.focused = FALSE;
    b.value = val;
    b.placeholder = placeholder;
    b.color = color;
    b.hovercolor = hovercolor;
    b.font = font;
    b.id = id;

    table->values[table->length] = b;
    table->length++;
}




void hover_button(button_list* bts,int mouseX,int mouseY){
    int i;
    button *btn;
    for(i=0;i<bts->length;i++){
        unsigned int minX,maxX,minY,maxY;
        btn = &(bts->values[i]);
        minX = btn->x;
        maxX = btn->x + btn->width;
        minY = btn->y;
        maxY = btn->y + btn->height;
        if(minX < (unsigned) mouseX && maxX  > (unsigned) mouseX && minY < (unsigned) mouseY && maxY > (unsigned) mouseY ){
            btn->hovered = TRUE;
        }else{
            btn->hovered = FALSE;
        }
    }
    draw_buttons(bts);
}

void quit_app(){
    /*Lorsqu'on quitte l'app normalement*/
    exit(0);
}

void click_button(screen* scr,screen* screens){
    int i;
    button a;
    button_list *btn;
    btn = &(scr->buttons);
    for (i = 0; i < btn->length; i++)
    {
        a = btn->values[i];
        if(a.hovered && MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED){
            switch(a.id){
                case 0 :
                    quit_app();
                    break;
                case 1 :
                    create_solo(&screens[SOLO_SCREEN],scr->width,scr->height);
                    break;
                    
            }
        }
    }
    
}

/**
 * Dessine le texte centré sur le point(x,y)
 */
void draw_centered_text(int x,int y,MLV_Font *font,char* text,MLV_Color color){
    int lw = 0,lh = 0;
    MLV_get_size_of_text_with_font(text,&lw,&lh,font);
    if(x - lw/2 < 0 || y -lh/2 < 0){
        printf("[graphics.h|draw_centered_text]POINT OUT OF SCREEN !!!\n");
        exit(-1);
    }
    MLV_draw_text_with_font(x - lw/2,y -lh/2,text,font,color);
}
void draw_screen(screen *screen){
    draw_centered_text(screen->width/2,screen->height/10,screen->title_font,screen->title,MLV_COLOR_WHITE);
    /*draw_buttons(&(screen->buttons));
    draw_textbox(&(screen->texts_box));*/
}

void update_score(score* score,int new_score){
    score->score = new_score;
    sprintf(score->text,"%d",score->score);
    draw_centered_text(score->x,score->y,score->font,score->text,score->color);
}