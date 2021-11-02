#include <MLV/MLV_all.h>
#include <time.h>
#include "../header/api.h"
#include "../header/const.h"
#include "../header/save.h"

void copy_blocks(int src[][4],int dest[][4]){
    int i,j;
    for (i = 0; i < FIGURE_SIZE; i++)
        {
            for (j = 0; j < FIGURE_SIZE;j++)
            {
                dest[i][j] = src[i][j];
            }
        }
}

int get_empty_figure(game g)
{
    int i, j, k;
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

void transp(int dest[][FIGURE_SIZE])
{
    int src[FIGURE_SIZE][FIGURE_SIZE];
    int i, j;
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
    transp(dest);
    reverse_row(dest);
}

/**
 * Rotation de -90°
*/
void rot_m90(int dest[][FIGURE_SIZE]){
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
    srand(time(NULL));
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
    for(i=0;i < MAX_FIGURES - 1 ; i++){
        copy_blocks(g.figures[i+1].blocks,g.figures[i].blocks);
    }
    gen_blocks(g.figures[MAX_FIGURES-1].blocks);
    write_save(g);
}

game init_game(game g){
    int i;
    g.case_size = 0;
    for(i = 0;i < MAX_FIGURES;i++){
        gen_blocks(g.figures[i].blocks);
    }
    return g;
}