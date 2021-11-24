#include <MLV/MLV_all.h>
#include <time.h>
#include "../header/api.h"
#include "../header/const.h"
#include "../header/save.h"

void copy_blocks(int src[][4],int dest[][4]){
    int i,j;
    printf("game.c    copy_blocks\n");
    for (i = 0; i < FIGURE_SIZE; i++)
        {
            for (j = 0; j < FIGURE_SIZE;j++)
            {
                dest[i][j] = src[i][j];
            }
        }
}

int get_empty_figure(game g){
    int i, j, k;
    printf("game.c    get_empty_figure\n");
    for (k = 0; k < MAX_FIGURES;k++)
    {
        for (i = 0; i < FIGURE_SIZE; i++)
        {
            for (j = 0; j < FIGURE_SIZE;j++)
            {
                if (g.figures[k].blocks[i][j] != 0)
                {
                    return k;
                }
            }
        }
    }
    return -1;
}

void transp(int dest[][FIGURE_SIZE]){
    int src[FIGURE_SIZE][FIGURE_SIZE];
    int i, j;
    printf("game.c    transp\n");
    copy_blocks(dest,src);
    for (i = 0; i < FIGURE_SIZE; i++)
    {
        for (j = 0; j < FIGURE_SIZE; j++)
        {
            dest[j][i] = src[i][j];
        }
    }
}

void reverse_row(int dest[][FIGURE_SIZE]){
    int i, j;
    int src[FIGURE_SIZE][FIGURE_SIZE];
    printf("game.c    reverse_row\n");
    copy_blocks(dest,src);
    for (i = 0; i < FIGURE_SIZE; i++)
    {
        for (j = 0; j < FIGURE_SIZE; j++)
        {
            dest[i][FIGURE_SIZE - (j+1)] = src[i][j];
        }
    }
}

void reverse_col(int dest[][FIGURE_SIZE]){
    int i, j;
    int src[FIGURE_SIZE][FIGURE_SIZE];
    printf("game.c    reverse_col\n");
    copy_blocks(dest,src);
    for (i = 0; i < FIGURE_SIZE; i++)
    {
        for (j = 0; j < FIGURE_SIZE; j++)
        {
            dest[FIGURE_SIZE - (i+1)][j] = src[i][j];
        }
    }
}

/**
 * Rotation de +90°
*/
void rot_90(int dest[][FIGURE_SIZE]){
    printf("game.c    rot_90\n");
    transp(dest);
    reverse_row(dest);
}

/**
 * Rotation de -90°
*/
void rot_m90(int dest[][FIGURE_SIZE]){
    printf("game.c    rot_m90\n");
    transp(dest);
    reverse_col(dest);
}

void gen_blocks(int block[][FIGURE_SIZE]){
    int i,rnd;
    int figs[8][4][4] = {
        {
            {0,0,0,0},
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {0,1,0,0},
            {0,1,1,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {1,1,0,0},
            {0,1,1,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,1,1,0}
        },
        {
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0}
        },
        {
            {0,0,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0}
        },
        {
            {0,0,0,0},
            {0,0,0,0},
            {0,1,0,0},
            {0,1,0,0}
        },
        {
            {1,1,1,1},
            {1,1,1,1},
            {1,1,1,1},
            {1,1,1,1}
        },

    };
    printf("game.c    gen_blocks\n");
    rnd = rand() % 8;
    copy_blocks(figs[rnd],block);
    rnd = rand() % 4;
    for(i = 0; i < rnd;i++){
        rot_90(block);
    }
}

void print_blocks(int mat[][4])
{
    int i, j;
    printf("game.c    print_blocks\n");
    print("------------------------------");
    for (i = 0; i < FIGURE_SIZE; i++)
    {
        for (j = 0; j < FIGURE_SIZE; j++)
        {
            printf("%d",mat[i][j]);
        }
        print(" ");
    }
    print("------------------------------");
}

void next_turn(game g){
    int i;
    printf("game.c    next_turn\n");
    for(i=0;i < MAX_FIGURES - 1 ; i++){
        copy_blocks(g.figures[i+1].blocks,g.figures[i].blocks);
    }
    gen_blocks(g.figures[MAX_FIGURES-1].blocks);
    write_save(g);
}

game init_game(game g){
    int i;
    printf("game.c    init_game\n");
    g.case_size = 0;
    for(i = 0;i < MAX_FIGURES;i++){
        gen_blocks(g.figures[i].blocks);
    }
    return g;
}

int est_fini(game g){
    int i;
    printf("game.c    est_fini\n");
    for(i=0;i<NB_COLS;i++){
        if(g.grid[3][i]!=0){
            return 1;       /*Si return 1 alors il y a un bloque sur la première ligne, le jeu est fini*/
        }
    }
    return 0;
}

int verif_sienbas(game g){
  int i, j;
  printf("game.c    verif_sienbas\n");
  for(j=0;j<NB_COLS;j++){
    if(g.grid[NB_LINES-1][j] > 1){
      return 1;
    }
  }
  for(i=NB_LINES-1;i>0;i--){
    for(j=0;j<NB_COLS;j++){
      if(g.grid[i][j] == 1 && g.grid[i-1][j] > 1){
	return 1;
      }
    }
  }
  return 0;
}

game verif_lignecomplete(game g){	/*pour chaque ligne, vérifie si la première valeur de la ligne est 1*/
  int i, j = 0, k, verif[NB_LINES]={0};		/*si elle vaut 1 alors ça check toute les valeurs de la ligne*/		/*j fait le count et compte le nombre d'itération de 1*/
  printf("game.c    verif_lignecomplete\n");/*si j est égal au NB_COLS, alors toute la ligne vaut 1*/
  for(i=0;i<NB_LINES;i++){
    if(g.grid[i][j]==1){
      for(k=0;k<NB_COLS;k++){
	if(g.grid[i][k]==1){
	  j++;
	}
      }
      if(j==NB_COLS){
	verif[i]=1;
      }
      j = 0;
    }
  }
  for(i=0;i<NB_LINES;i++){
    if(verif[i]==1){
      for(j=0;j<NB_COLS;j++){
	g.grid[i][j] = 0;
      }
    }
  }
  return g;
}

/*void ligne_complete(game g, int verif[NB_LINES]){efface une ligne complète
    int i, j;
  printf("game.c    ligne_complete\n");
  for(i=0;i<NB_LINES;i++){
    if(verif[i]==1){
      for(j=0;j<NB_COLS;j++){
	g.grid[i][j] = 0;
      }
    }
  }
}*/

int verif_jeufini(game g){
  int i;
  printf("game.c    verif_jeufini\n");
  for(i=0;i<NB_COLS;i++){
    if(g.grid[3][i] > 1){
      return 1;
    }
  }
  return 0;
}

game descente(game g){
  int i, j;
  printf("game.c    descente\n");
  for(i=NB_LINES-1;i>=0;i--){
    for(j=0;j<NB_COLS;j++){
      if(g.grid[i][j]!=0 && g.grid[i][j]!=1){
	g.grid[i+1][j] = g.grid[i][j];
	g.grid[i][j] = 0;
      }
    }
  }
  return g;
}

game mouv_gauche(game g){
  int i, j;
  printf("game.c    mouv_gauche\n");
  for(i=0;i<NB_LINES;i++){
    if(g.grid[i][0] > 1){
      printf("0 ");
      return g;
    }
  }
  for(j=1;j<NB_COLS;j++){
    for(i=0;i<NB_LINES;i++){
      if(g.grid[i][j] > 1){
	printf("1\n");
	g.grid[i][j-1] = g.grid[i][j];
	g.grid[i][j] = 0;
      }
    }
  }
  printf("2\n");
  return g;
}

game mouv_droite(game g){
  int i, j;
  printf("game.c    mouv_droite\n");
  for(i=0;i<NB_LINES;i++){
    if(g.grid[i][NB_COLS-1]!=0 && g.grid[i][NB_COLS-1]!=1){
      return g;
    }
  }
  for(j=NB_COLS-2;j>=0;j--){
    for(i=0;i<NB_LINES;i++){
      if(g.grid[i][j]!=0 && g.grid[i][j]!=1){
	g.grid[i][j+1] = g.grid[i][j];
	g.grid[i][j] = 0;
      }
    }
  }
  return g;
}
	
game fixer_bloque(game g){
  int i, j;
  for(i=0;i<NB_LINES;i++){
    for(j=0;j<NB_COLS;j++){
      if(g.grid[i][j] > 1){
	g.grid[i][j] = 1;
      }
    }
  }
  return g;
}
