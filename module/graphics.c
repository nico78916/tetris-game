#include <MLV/MLV_all.h>
#include "../header/api.h"
#include "../header/const.h"
#include "../header/game.h"


void prompt(screen cur,char* message,int display_time){
    int w,h;
    MLV_get_size_of_text_with_font(message,&w,&h,western_font);
    MLV_draw_text_with_font(cur.width/2 - w,cur.height/2 - h,message,western_font,MLV_COLOR_WHITE);
    MLV_wait_milliseconds(display_time);
    MLV_draw_text_with_font(cur.width/2 - w,cur.height/2 - h,message,western_font,MLV_COLOR_BLACK);
}

/**
 * @return retourne l'index du bouton en cas de réussite ou -1 en cas d'échec
*/
int get_hovered_button(button* bts,int x,int y,int btn_size){
    int bx,by,bw,bh;
    while(btn_size > 0){
        btn_size -=1;
        bx = bts[btn_size].x;
        by = bts[btn_size].y;
        bw = bts[btn_size].width;
        bh = bts[btn_size].height;
        if(x > bx && x < bx+bw && y > by && y < by+bh){
            return btn_size;
        }
    }
    return -1;
}

/**
 * @param str de taille 21 EXACTEMENT
*/
void get_screen_resolution_text(screen current,char* str){
    char res[12] = "RESOLUTION ",stw[5],sth[5];/* 21 = nb caracteres possible pour un long int */
    sprintf(sth,"%d",current.height);
    sprintf(stw,"%d",current.width);
    print(stw);
    print(sth);
    strcat(str,res);
    strcat(str,stw);
    strcat(str,"x");
    strcat(str,sth);
}

screen gen_menu(screen current){
    int i,lastY,bw,bh,tw,th,width,height;
    char* labels[5] = {"NOUVELLE PARTIE","CONTINUER","CHARGER","OPTIONS","QUITTER"};
    current.btncount = 5;
    MLV_clear_window(MLV_COLOR_BLACK);
    width = current.width;
    height = current.height;
    bw = width/2;
    bh = height/10;
    lastY = height/(current.btncount-1) - bh/2;
    MLV_get_size_of_text_with_font("TETE RISSE",&tw,&th,title_font);
    MLV_draw_text_with_font(width/2-tw/2,height/10-th/2,"TETE RISSE",title_font,MLV_COLOR_WHITE);
    for(i=0;i<current.btncount;i++){
        button b;
        /*char a[MAX_STR];*/
        MLV_draw_text_box_with_font(width/2-bw/2,lastY,bw,bh,labels[i],default_font,20,MLV_COLOR_WHITE,MLV_COLOR_BLACK,MLV_COLOR_GREY,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);
        b.height = bh;
        b.width = bw;
        b.x = width/2-bw/2;
        b.y = lastY;
        b.label = labels[i];
        /*strcpy(b.label,labels[i]);*/
        current.buttons[i] = b;
        lastY += bh + height/16;
    }
    current.id = MENU;
    return current;
}

screen gen_option(screen current){
    int i,lastY,bw,bh,tw,th,width,height;
    char chaine[MAX_STR] = "";
    char* labels[5];
    current.btncount = 5;
    if(MLV_is_full_screen()){
        labels[1] = "MODE FULLSCREEN";
    }else{
        labels[1] = "MODE WIDOWED";
    }
        labels[2] = "SON : OFF";
        labels[3] = "COULEURS : OFF";
        labels[4] = "RETOUR";
    MLV_clear_window(MLV_COLOR_BLACK);
    print("[gen_option] INIT");
    printf("w = %d, h=%d, btn_c = %d, id = %d\n",current.width,current.height,current.btncount,current.id);
    width = current.width;
    height = current.height;
    /* POURQUOI ???? POURQUOI NOUS FAIRE ÇA ? */
    get_screen_resolution_text(current,chaine);
    labels[0] = chaine;
    printf("%s\n",chaine);
    /* Avec un malloc() ça aurait pris 2 lignes : sprintf(chaine,"%dx%d",width,height); ... free(chaine); */
    bw = width/2;
    bh = height/10;
    lastY = height/current.btncount - bh/2;
    MLV_get_size_of_text_with_font("OPTIONS",&tw,&th,title_font);
    MLV_draw_text_with_font(width/2-tw/2,height/10-th/2,"OPTIONS",title_font,MLV_COLOR_WHITE);
    for(i=0;i<current.btncount;i++){
        button b;
        /*char a[MAX_STR];*/
        MLV_draw_text_box_with_font(width/2-bw/2,lastY,bw,bh,labels[i],default_font,20,MLV_COLOR_WHITE,MLV_COLOR_BLACK,MLV_COLOR_GREY,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);
        b.height = bh;
        b.width = bw;
        b.x = width/2-bw/2;
        b.y = lastY;
        b.label = labels[i];
        current.buttons[i] = b;
        lastY += bh + height/16;
    }
    current.id = OPTIONS;
    print("[gen_option] End of function");
    return current;
}

screen gen_game(screen current){
    int width,height,i,j;
    char *label ="PAUSE";
    button b;
    game setup;
    current.btncount = 1;
    width = current.width/10;
    height = current.height/5;
    MLV_clear_window(MLV_COLOR_BLACK);
    MLV_draw_text_box_with_font(width * 9,height * 4,width,height,label,default_font,20,MLV_COLOR_WHITE,MLV_COLOR_BLACK,MLV_COLOR_GREY,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);
    print("PASS");
    b.height = current.height/5;
    b.width = current.width/10;
    b.x = current.width - current.width/10;
    b.y = current.height ;
    b.label = label;
    current.buttons[0] = b;
    height = current.height;
    height -= height/4;
    width = height;
    setup.case_size = height/NB_LINES;
    for(i = 0;i < NB_COLS;i++){
        for(j = 0;j < NB_LINES;j++){
            MLV_draw_rectangle((current.width/4 - (NB_COLS*setup.case_size)/2) + i * setup.case_size,height/100 + j * setup.case_size,setup.case_size,setup.case_size,MLV_COLOR_GREY);
        }
    }
	/*ce qu'on peut faire, c'est mettre une couleur unique pour les cubes déjà posés comme ça on peut intégrer le bloc qui descend dans la matrice
	la valeur 0 pour le noir, 1 pour les cubes en bas, ducoup pour faire un traitement sur le cube qui descend, on dit :
	pour chaque valeur différente de 0 et 1, alors faire traitement*/
	/*ça permet d'avoir une seule matrice à gérer au lieu de 2 matrices supperposées mais il ne serait pas possible de faire les rotations*/
	int t1 = SDL_GetTicks(), t2, finw = 0;				/*penser à ajouter time.h*/
    while(est_fini == 0){								/*boucle du jeu*/
        figure figure;
        figure = game->figures;
        gen_block(figure.blocks);						/*génère le bloque*/
        /*ajouter un appel de fonction mlv qui matérialise le block*/
		while(finw == 0){								/*boucle qui fait tomber le bloque et attend les instructions*/
			t2 = SDL_GetTicks();
			if(t2 - t1 == 1000){
								/*si atente d'une seconde, le bloque tombe, modification de la grille*/
				t1 = t2;
			}/*else if(){*/
								/*descente accélérée*/	
			/*}*/
			/*if(MLV_wait_keyboards_or_milliseconds()avec un temps de 10ms != valeur si rien)*/
			/*vérifier que le coup est valide et le faire le cas échéant*/
			/*if(){*/			/*si flèche gauche, mouvement à gauche*/
				
			/*}else if(){*/			/*si flèche droite, mouvement à droite*/
				
			/*}else if(){*/			/*si boutton -, rotation à -90°, pas possible si une seule matrice*/
				
			/*}else if(){*/			/*si boutton +, rotation à 90°, pas possible si une seule matrice*/
				
			/*}*/
			/*à la fin de chaque while, vérifier que le bloque n'est pas déscendu en bas sinon return 1*/
		}
		/*le bloque est déscendu en bas, appel fonction pour vérif si ligne complète et donc effacer*/
		/*appel de la fonction pour vérifier si le jeu est fini et recommence au premier while*/
    }
    current.id = GAME;
    current.jeu = setup;
    return current;
}

screen gen_newgame(screen current){
    int i,lastY,bw,bh,tw,th,width,height;
    char* labels[5] = {"SOLO","2 JOUEURS","3 JOUEURS","4 JOUEURS","RETOUR"};
    current.btncount = 5;
    MLV_clear_window(MLV_COLOR_BLACK);
    width = current.width;
    height = current.height;
    bw = width/2;
    bh = height/10;
    lastY = height/(current.btncount-1) - bh/2;
    MLV_get_size_of_text_with_font("Choisissez le nombre de joueurs",&tw,&th,title_font);
    MLV_draw_text_with_font(width/2-tw/2,height/10-th/2,"Choisissez le nombre de joueurs",title_font,MLV_COLOR_WHITE);
    for(i=0;i<current.btncount;i++){
        button b;
        /*char a[MAX_STR];*/
        MLV_draw_text_box_with_font(width/2-bw/2,lastY,bw,bh,labels[i],default_font,20,MLV_COLOR_WHITE,MLV_COLOR_BLACK,MLV_COLOR_GREY,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);
        b.height = bh;
        b.width = bw;
        b.x = width/2-bw/2;
        b.y = lastY;
        b.label = labels[i];
        /*strcpy(b.label,labels[i]);*/
        current.buttons[i] = b;
        lastY += bh + height/16;
    }
    current.id = NEWGAME;
    return current;
}

screen gen_load(screen current){
    int i,lastY,bw,bh,tw,th,width,height;
    char* labels[6] = {"SAUVEGRADE 1","SAUVEGARDE 2","SAUVEGARDE 3","SAUVEGARDE 4","SAUVEGARDE 5","RETOUR"};
    current.btncount = 6;
    MLV_clear_window(MLV_COLOR_BLACK);
    width = current.width;
    height = current.height;
    bw = width/2;
    bh = height/(current.btncount*2);
    lastY = height/(current.btncount-1) - bh/2;
    MLV_get_size_of_text_with_font("SAUVEGARDE",&tw,&th,title_font);
    MLV_draw_text_with_font(width/2-tw/2,height/10-th/2,"SAUVEGARDE",title_font,MLV_COLOR_WHITE);
    for(i=0;i<current.btncount;i++){
        button b;
        /*char a[MAX_STR];*/
        MLV_draw_text_box_with_font(width/2-bw/2,lastY,bw,bh,labels[i],default_font,20,MLV_COLOR_WHITE,MLV_COLOR_BLACK,MLV_COLOR_GREY,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);
        b.height = bh;
        b.width = bw;
        b.x = width/2-bw/2;
        b.y = lastY;
        b.label = labels[i];
        /*strcpy(b.label,labels[i]);*/
        current.buttons[i] = b;
        lastY += bh + height/16;
    }
    current.id = LOAD;
    return current;
}

screen switch_widow_type(screen current){
    print("SWITCHING STRING TYPE");
    if(MLV_is_full_screen()){
        MLV_change_window_size(current.max_width,current.max_height);
        MLV_disable_full_screen();
        MLV_change_window_size(DEFAULT_SCREEN_WIDTH,DEFAULT_SCREEN_HEIGHT);
        current.width = DEFAULT_SCREEN_WIDTH;
        current.height = DEFAULT_SCREEN_HEIGHT;
    }else{
        MLV_change_window_size(current.max_width,current.max_height);
        MLV_enable_full_screen();
        current.width = current.max_width;
        current.height = current.max_height;
    }
    print("END");
    return gen_option(current);
}

screen change_resolution(screen current,int rw,int rh){
    printf("\nRésolution demandée : %dx%d\n",rw,rh);
    if(MLV_is_full_screen()){   
        current = switch_widow_type(current);
    }
    current.height = rh;
    current.width = rw;
    MLV_change_window_size(current.width,current.height);
    return gen_option(current);
}
