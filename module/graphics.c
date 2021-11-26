#include <MLV/MLV_all.h>
#include "../header/api.h"
#include "../header/const.h"
#include "../header/game.h"


void prompt(screen cur,char* message,int display_time){
    int w,h;
    printf("graphics.c    prompt\n");
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
    printf("graphics.c    get_hovered_button\n");
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
    printf("graphics.c    get_screen_resolution_text\n");
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
    printf("graphics.c    gen_menu\n");
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
    printf("graphics.c    gen_option\n");
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
  int width,height,i,j, finw = 0, count = 0, rnd, couleur;
    char *label ="PAUSE";
    button b;
    game setup;
    figure figure[MAX_FIGURES];
    MLV_Keyboard_button sym;
    code_couleur code_couleur[MAX_COLOR];
    init_code_couleur(code_couleur);
    setup = init_game(setup);
    printf("graphics.c    gen_game\n");
    current.btncount = 1;
    width = current.width/10;
    height = current.height/5;
    MLV_clear_window(MLV_COLOR_BLACK);
    /*MLV_draw_text_box_with_font(width * 9,height * 4,width,height,label,default_font,20,MLV_COLOR_WHITE,MLV_COLOR_BLACK,MLV_COLOR_GREY,MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);*/
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
    
    setup.x = (current.width/4 - (NB_COLS*setup.case_size)/2);
    setup.y = height/100;
    setup.width = setup.case_size * NB_COLS;
    setup.height = setup.case_size * NB_LINES;

    /*ce qu'on peut faire, c'est mettre une couleur unique pour les cubes déjà posés comme ça on peut intégrer le bloc qui descend dans la matrice
      la valeur 0 pour le noir, 1 pour les cubes en bas, ducoup pour faire un traitement sur le cube qui descend, on dit :
      pour chaque valeur différente de 0 et 1, alors faire traitement*/
    /*ça permet d'avoir une seule matrice à gérer au lieu de 2 matrices supperposées mais il ne serait pas possible de faire les rotations*/
    /*penser à ajouter time.h*/
    for(i=0;i<NB_LINES;i++){
      for(j=0;j<NB_COLS;j++){
        setup.grid[i][j] = 0;
      }
    }
    for(i=0;i<MAX_FIGURES;i++){     /*génération des 5 premiers bloques*/
      gen_blocks(figure[i].blocks);
    }
    MLV_actualise_window();
    while(est_fini(setup) == 0){    /*boucle du jeu*/
      rnd = (rand() % 10) + 11;            /*pour la couleur*/
      printf("%d\n", rnd);
      for(i=0;i<FIGURE_SIZE;i++){
	for(j=0;j<FIGURE_SIZE;j++){
	  if(figure[0].blocks[i][j] == 1){
	    figure[0].blocks[i][j] = rnd;
	  }
	}
      }
      for(i=0;i<FIGURE_SIZE;i++){
	for(j=0;j<FIGURE_SIZE;j++){
	  printf("%d ", figure[0].blocks[i][j]);
	}
	printf("\n");
      }
      for(i=0;i<FIGURE_SIZE;i++){   /*la grille prend le bloque*/
	for(j=0;j<FIGURE_SIZE;j++){
	  setup.grid[i][j+5] = figure[0].blocks[i][j];
	}
      }
      for(i=1;i<MAX_FIGURES;i++){   /*les bloques dans la fille sont décallés*/
	figure[i-1] = figure[i];
      }
      gen_blocks(figure[4].blocks);/*génére nouveau bloque dans file d'attente*/
      /*ajouter un appel de fonction mlv qui matérialise le block*/
      while(finw == 0){								/*boucle qui fait tomber le bloque et attend les instructions*/
	/*penser à mettre un reset buffer*/
	MLV_actualise_window();
	MLV_wait_milliseconds(500);
	MLV_get_keyboard_state(MLV_KEYBOARD_DOWN);
	if(count >= 2){
	  if(verif_sienbas(setup) == 0){
	    setup = descente(setup);				/*si atente d'une seconde, le bloque tombe, modification de la grille*/
	    count = 0;
	  }
	}else if(count >= 1 && MLV_get_keyboard_state(MLV_KEYBOARD_DOWN) == MLV_PRESSED){	/*descente accélérée*/
	  if(verif_sienbas(setup) == 0){
	    setup = descente(setup);
	    count = 0;
	  }
	}
	/*vérifier que le coup est valide et le faire le cas échéant*/
	if(MLV_get_keyboard_state(MLV_KEYBOARD_LEFT) == MLV_PRESSED){	        /*si flèche gauche, mouvement à gauche*/
	  setup = mouv_gauche(setup);
	}else if(MLV_get_keyboard_state(MLV_KEYBOARD_RIGHT) == MLV_PRESSED){	/*si flèche droite, mouvement à droite*/
	  setup = mouv_droite(setup);
	}else if(MLV_get_keyboard_state(MLV_KEYBOARD_p) == MLV_PRESSED || MLV_get_keyboard_state(MLV_KEYBOARD_PAUSE) == MLV_PRESSED || MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) == MLV_PRESSED){
	  printf("insérer une fonction pour la pause (appuie sur p, pause ou echap)\n");
	  MLV_save_screen();
	  while(1){
	    MLV_clear_window(MLV_COLOR_BLACK);
	    MLV_draw_text_with_font(200, 200, "PAUSE", default_font, 20, MLV_COLOR_RED);
	    MLV_actualise_window();
	    MLV_wait_seconds(4);
	  }
	  MLV_load_screen();
	  MLV_actualise_window();
	  /*MLV_create_window("Pause", "Pause", 400, 200);*/
	  MLV_wait_seconds(4);
	}/*else if(){*/			/*si boutton +, rotation à 90°, pas possible si une seule matrice*/
				
	/*}*/
	/*à la fin de chaque while, vérifier que le bloque n'est pas déscendu en bas sinon return 1*/
	for(i=0;i<NB_LINES;i++){
	  for(j=0;j<NB_COLS;j++){
	    printf("%d ", setup.grid[i][j]);
	  }
	  printf("\n");
	}
	if(verif_sienbas(setup)==1){
	  finw = 1;
	}
	count++;
	sym = 0;
	MLV_clear_window(MLV_COLOR_BLACK);
	for(i = 0;i < NB_COLS;i++){
	  for(j = 0;j < NB_LINES;j++){
            MLV_draw_rectangle((current.width/4 - (NB_COLS*setup.case_size)/2) + i * setup.case_size,height/100 + j * setup.case_size,setup.case_size,setup.case_size,MLV_COLOR_GREY);
	  }
	}
	for(i=0;i<NB_COLS;i++){
	  for(j=0;j<NB_LINES;j++){
	    if(setup.grid[j][i] != 0){
	      couleur = setup.grid[j][i];
	      MLV_draw_filled_rectangle((current.width/4 - (NB_COLS*setup.case_size)/2) + i * setup.case_size,height/100 + j * setup.case_size,setup.case_size,setup.case_size,code_couleur[couleur].color);
	    }
	  }
	}
	MLV_actualise_window();
      }
      /*il faut que le bloque s'intègre aux bloques fixes*/
      setup = fixer_bloque(setup);
      setup = verif_lignecomplete(setup);
      finw = 0;
      /*le bloque est déscendu en bas, appel fonction pour vérif si ligne complète et donc effacer*/
      /*appel de la fonction pour vérifier si le jeu est fini et recommence au premier while*/
    }
    
    for(i=0;i<MAX_FIGURES;i++){
      setup.figures[i] = figure[i];
    }
    current.id = GAME;
    current.jeu = setup;
    return current;
}

screen gen_newgame(screen current){
    int i,lastY,bw,bh,tw,th,width,height;
    char* labels[5] = {"SOLO","2 JOUEURS","3 JOUEURS","4 JOUEURS","RETOUR"};
    printf("graphics.c    gen_newgame\n");
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
    printf("graphics.c    gen_load\n");
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

int htoi(char hex){
    printf("graphics.c    htoi\n");
    if(hex > '0' && hex < '9'){
        return hex - '0';
    }else{
        return hex - 'A' + 9;
    }
}

MLV_Color convert_hex_to_color(char* x){
    printf("graphics.c    convert_hex_to_color\n");
    if(x[0] != '#'){
        return MLV_COLOR_BLACK;
    }
    return MLV_rgba(htoi(x[1]) * 16 + htoi(x[2]),htoi(x[3]) * 16 + htoi(x[4]),htoi(x[5]) * 16 + htoi(x[6]),255);
}

void draw_game(screen current){
    game setup;
    int i,j;
    printf("graphics.c    draw_game\n");
    setup = current.jeu;
    for(i = 0;i < NB_COLS;i++){
        for(j = 0;j < NB_LINES;j++){
            MLV_draw_rectangle(setup.x + i * setup.case_size,setup.y + j * setup.case_size,setup.case_size,setup.case_size,convert_hex_to_color(convertions[setup.grid[i][j]]));
        }
    }
}
