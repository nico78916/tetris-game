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
    char* labels[4] = {"NOUVELLE PARTIE","CHARGER","OPTIONS","QUITTER"};
    MLV_clear_window(MLV_COLOR_BLACK);
    width = current.width;
    height = current.height;
    bw = width/2;
    bh = height/10;
    lastY = height/3 - bh/2;
    MLV_get_size_of_text_with_font("TETE RISSE",&tw,&th,title_font);
    MLV_draw_text_with_font(width/2-tw/2,height/10-th/2,"TETE RISSE",title_font,MLV_COLOR_WHITE);
    for(i=0;i<4;i++){
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
        current.btncount = 4;
        lastY += bh + height/16;
    }
    current.id = MENU;
    return current;
}

screen gen_option(screen current){
    int i,lastY,bw,bh,tw,th,width,height;
    char chaine[MAX_STR] = "";
    char* labels[5];
        labels[1] = "MODE FULLSCREEN";
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
    lastY = height/5 - bh/2;
    MLV_get_size_of_text_with_font("OPTIONS",&tw,&th,title_font);
    MLV_draw_text_with_font(width/2-tw/2,height/10-th/2,"OPTIONS",title_font,MLV_COLOR_WHITE);
    for(i=0;i<5;i++){
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
    current.btncount = 5;
    current.id = OPTIONS;
    print("[gen_option] End of function");
    return current;
}

screen change_resolution(screen current,int rw,int rh){
    MLV_change_window_size(rw,rh);
    current.height = rh;
    current.width = rw;
    return gen_option(current);
}