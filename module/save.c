#include <stdio.h>
#include <MLV/MLV_all.h>
#include "../header/api.h"
#include "../header/const.h"
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
                j++;
            }
            print(" ");
        }
        print("\n----");
    }
    return g;
}

game load_save(int slot){
    FILE * save;
    game thing;
    char path[13];
    char mat[STR_MAT_GRID];
    char fig[MAX_FIGURES][STR_FIG];
    int score,i,j,k = 0,l=0;
    init_mat(thing.grid);
    sprintf(path,"./save/%d.txt",slot);
    save = fopen( path, "r" );
    if ( save == NULL ) {
        print( "La sauvegarde est corrompu ou inaccessible");
        return thing;
    }
    fscanf(save,"score:\n{%d};game:{%[^}];figures{{%[^}],{%[^}],{%[^}],{%[^}],{%[^}]}",&score,mat,fig[0],fig[1],fig[2],fig[3],fig[4]);
    fclose( save );
    if(strlen(mat) != (NB_COLS)*(NB_LINES)){
        print("SAUVEGARDE CORROMPU IMPOSSIBLE DE CONTINUER LE PROGRAMME");
        exit(-1);
    }
    for(i = 0;i<NB_LINES;i++){
        for(j=0;j<NB_COLS;j++){
            thing.grid[i][j] = mat[k] - '0';
            k++;
        }
    }
    print_mat(thing.grid,NB_LINES,NB_COLS);
    for(i=0;i<5;i++){
        l = 0;
        for(j=0;j<FIGURE_SIZE;j++){
            for(k=0;k<FIGURE_SIZE;k++){
                thing.figures[i].blocks[j][k] = fig[i][l]-'0';
                l++;
            }
        }
    }
    thing.players[0].score = score;
    return thing;
}

void write_save(game g){
    int i,j,k = 0,l;
    FILE * save;
    char path[13];
    char str[STR_MAT_GRID];
    char fig[STR_FIGS];
    if(g.ply_count){
        
     return;   
    }
    sprintf(path,"./save/%d.txt",g.slot);
    save = fopen( path, "w+" );
    for(i = 0;i < NB_LINES;i ++){
        for(j = 0; j < NB_COLS;j++){
            str[k]='0' + g.grid[i][j];
            k++;
        }
        str[k] = '\n';
        k++;
    }
    k = 0;
    print("registering");
    for (i = 0; i < 5; i++)
    {
        for(j = 0; j < 4;j++){
            for(l = 0; l < 4;l++){
                fig[k] = '0' + g.figures[i].blocks[j][l];
                printf("%c",fig[k]);
                k++;
            }
            fig[k] = '\n';
            k++;
            print("\n");
        }
        fig[k] = '\n';
        k++;
    }
    fprintf(save,"%d\nm\n%sb\n%s", g.players[0].score,str,fig);
    fclose(save);
}