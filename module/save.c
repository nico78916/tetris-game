#include <stdio.h>
#include <MLV/MLV_all.h>
#include "../header/api.h"
#include "../header/const.h"
#include "../header/game.h"
/* Pour lire ou écrire les sauvegardes */

void init_mat(int mat[NB_LINES][NB_COLS]){
    int i,j;
    for(i=0;i<NB_LINES;i++){
        for(j=0;j<NB_COLS;j++){
            mat[i][j] = 0;
        }
    }
}

void print_mat(int mat[NB_LINES][NB_COLS],int n,int m){
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
           printf("%d",mat[i][j]);
        }
       printf("\n");
    }
    for(j=0;j<m;j++)
   printf("-");
   printf("\n");
}

int get_score_from_save(FILE * save){
    char c;
    int i = 0;
    char str[21];
    while((c=fgetc(save)) != '\n'){
        str[i] = c;
        i++;
    }
    return atoi(str);
}

void get_game_matrice(FILE* save,int mat[NB_LINES][NB_COLS]){
    int i,j=0;
    char c;
    while(fgetc(save) != 'm');
    for(i=0;i<NB_LINES;i++){
        j=0;
        while((c=fgetc(save)) != '\n'){
            mat[i][j] = (c - '0');
            j++;
        }
    }
}

game get_figures(FILE* save,game g){
    int i,j,k;
    char s[10];

    while(fgetc(save) != 'b');
    for(i=0;i<5;i++){
        fgetc(save);/* on supprime le saut de ligne */
        print("----");
        for(k=0;k<4;k++){
            fgets(s,10,save);
            j = 0;
            while(s[j] != '\n'){
               printf("%d",s[j]- '0');
                g.figures[i].blocks[k][j] = s[j]- '0';
                g.figures[i].blocks[k][j] = g.figures[i].blocks[k][j] > 0 ? g.figures[i].blocks[k][j] + 10 : 0;
                j++;
            }
            print(" ");
        }
        print("\n----");
    }
    return g;
}

void load_save(screen *current){
    FILE * save;
    char path[13];
    char mat[STR_MAT_GRID] = "";
    char fig[MAX_FIGURES][STR_FIG] = {"","","","",""};
    int score,i,j,k = 0,l=0;
    init_mat(current->jeu.grid);
    sprintf(path,"./save/%d.txt",current->jeu.slot);
    save = fopen( path, "r" );
    if ( save == NULL ) {
        print( "La sauvegarde est corrompu ou inaccessible");
        exit(-1);
    }
    /* ,{%[^}]},{%[^}]},{%[^}]},{%[^}]}} ,fig[1],fig[2],fig[3],fig[4] */
    fscanf(save,"score:{%d};game:{%[^}]};figures:{{%[^}]},{%[^}]},{%[^}]},{%[^}]},{%[^}]}}",&score,mat,fig[0],fig[1],fig[2],fig[3],fig[4]);
    if(strlen(mat) != (NB_COLS)*(NB_LINES)){
        print("SAUVEGARDE CORROMPU IMPOSSIBLE DE CONTINUER LE PROGRAMME");
        exit(-1);
    }
    for(i = 0;i<NB_LINES;i++){
        for(j=0;j<NB_COLS;j++){
            current->jeu.grid[i][j] = mat[k] - '0';
            k++;
        }
    }
    print_mat(current->jeu.grid,NB_LINES,NB_COLS);
    for(i=0;i<MAX_FIGURES;i++){
        printf("fig[%d] = %s\n",i,fig[i]);
        l = 0;
        for(j=0;j<FIGURE_SIZE;j++){
            for(k=0;k<FIGURE_SIZE;k++){
                current->jeu.figures[i].blocks[j][k] = (fig[i][l]-'0') == 0 ? 0 : (fig[i][l]-'0')+10;
                l++;
            }
        }
    }
    current->jeu.ply_count = 1;
    current->jeu.players[0].score = score;
    fclose( save );
}

void delete_useless(int grid[NB_LINES][NB_COLS]){
    int i,j;
    for(i=0;i<NB_LINES;i++){
        for(j=0;j<NB_COLS;j++){
            grid[i][j] = grid[i][j] > 9 ? 0 : grid[i][j];
        }   
    }
}

void write_save(game *g){
    int i,j,k = 0,l;
    FILE * save;
    char path[13];
    char str[STR_MAT_GRID] = "";
    char fig[STR_FIGS] = "";
    print("SAVING");
    delete_useless(g->grid);
    for(i=0;i<MAX_FIGURES;i++){
        for(j=0;j<FIGURE_SIZE;j++)
            for(k=0;k<FIGURE_SIZE;k++){
                g->figures[i].blocks[j][k] = g->figures[i].blocks[j][k] > 10 ? g->figures[i].blocks[j][k] - 10 : g->figures[i].blocks[j][k];
            }
    }
    if(g->ply_count > 1){
        print("Tentative de sauvegarde d'une partie en multijoueur,\n annulation de la sauvegarde ...");
     return;   
    }
    k=0;
    sprintf(path,"./save/%d.txt",g->slot);
    save = fopen( path, "w+" );
    for(i = 0;i < NB_LINES;i ++){
        for(j = 0; j < NB_COLS;j++){
            str[k]='0' + g->grid[i][j];
            k++;
        }
    }
    str[k] = '\0';
    k = 0;
    for (i = 0; i < 5; i++)
    {
        fig[k] = '{';
        k++;
        for(j = 0; j < 4;j++){
            for(l = 0; l < 4;l++){
                fig[k] = '0' + g->figures[i].blocks[j][l];
                k++;
            }
        }
        fig[k] = '}';
        k++;
        fig[k] = ',';
        k++;
    }
    k--;
    fig[k] = '\0';
    printf("game :\n %s\n",str);
    printf("figures : \n %s\n",fig);
    fprintf(save,"score:{%d};game:{%s};figures:{%s}", g->players[0].score,str,fig);
    fclose(save);
}

void get_scoreboard(player_info scoreboard[10]){
    FILE * save;
    int i = 0;
    char a[64] = "";
    int b;
    save = fopen( "./save/scoreboard.txt", "r" );
    if ( save == NULL ) {
        print( "La sauvegarde est corrompu ou inaccessible");
        return;
    }
    while(fscanf(save,"%s\n%d",a,&b) == 2 && i < 10){
        printf("%s %d %d\n",a,b,i);
        strcpy(scoreboard[i].name,a);
        scoreboard[i].score = b;
        i++;
    }
    if(i != 10){
        printf("[Erreur|Fatale] Scoreboard corrompu %d\n",i);
        exit(-1);
    }
    print("Fin");
    fclose(save);
}

void set_score(player_info infos){
    player_info scoreboard[10];
    int i,index = 10;
    FILE * saved;
    print("STARTING REGISTERING NEW SCORE");
    get_scoreboard(scoreboard);
    saved = fopen( "./save/scoreboard.txt", "w" );
    if ( saved == NULL ) {
        print("Impossible d'ouvrir scoreboard.txt");
        return;
    }
    for(i=9;i>=0;i--){
        if(scoreboard[i].score < infos.score){
            printf("name : %s\n",scoreboard[i].name);
            index--;
        }
    }
    if(index == 10){
        return;
    }
    for(i=9;i>index;i--){
        scoreboard[i].score = scoreboard[i-1].score;
        strcpy(scoreboard[i].name,scoreboard[i-1].name);
    }
    printf("%d\n",index);
    scoreboard[index].score = infos.score;
    strcpy(scoreboard[i].name,infos.name);
    
    for(i=0;i<10;i++){
        fprintf(saved,"%s\n%d\n",scoreboard[i].name,scoreboard[i].score);
    }
    print("END OF NEW SCORE");
}