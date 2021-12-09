#include <MLV/MLV_all.h>
#include <time.h>
#include "../header/api.h"
#include "../header/const.h"
#include "../header/save.h"

void copy_blocks(int src[FIGURE_SIZE][FIGURE_SIZE], int dest[FIGURE_SIZE][FIGURE_SIZE])
{
  int i, j;
  printf("game.c    copy_blocks\n");
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      dest[i][j] = src[i][j];
    }
  }
}

void set_colors(int blocks[FIGURE_SIZE][FIGURE_SIZE])
{
  int rnd, i, j;
  rnd = (rand() % 10) + 10; /*pour la couleur*/
  if (rnd == 0 || rnd == 10)
    rnd++;
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      if (blocks[i][j] == 1)
      {
        blocks[i][j] = rnd == 10 ? 0 : rnd;
      }
    }
  }
}

int get_empty_figure(game g)
{
  int i, j, k;
  printf("game.c    get_empty_figure\n");
  for (k = 0; k < MAX_FIGURES; k++)
  {
    for (i = 0; i < FIGURE_SIZE; i++)
    {
      for (j = 0; j < FIGURE_SIZE; j++)
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

void transp(int dest[FIGURE_SIZE][FIGURE_SIZE])
{
  int src[FIGURE_SIZE][FIGURE_SIZE];
  int i, j;
  printf("game.c    transp\n");
  copy_blocks(dest, src);
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      dest[j][i] = src[i][j];
    }
  }
}

void reverse_row(int dest[FIGURE_SIZE][FIGURE_SIZE])
{
  int i, j;
  int src[FIGURE_SIZE][FIGURE_SIZE];
  printf("game.c    reverse_row\n");
  copy_blocks(dest, src);
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      dest[i][FIGURE_SIZE - (j + 1)] = src[i][j];
    }
  }
}

void reverse_col(int dest[FIGURE_SIZE][FIGURE_SIZE])
{
  int i, j;
  int src[FIGURE_SIZE][FIGURE_SIZE];
  printf("game.c    reverse_col\n");
  copy_blocks(dest, src);
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      dest[FIGURE_SIZE - (i + 1)][j] = src[i][j];
    }
  }
}

/**
 * Rotation de +90°
 */
void rot_90(int dest[FIGURE_SIZE][FIGURE_SIZE])
{
  printf("game.c    rot_90\n");
  transp(dest);
  reverse_row(dest);
}

/**
 * Rotation de -90°
 */
void rot_m90(int dest[FIGURE_SIZE][FIGURE_SIZE])
{
  printf("game.c    rot_m90\n");
  transp(dest);
  reverse_col(dest);
}

void print_blocks(int mat[FIGURE_SIZE][FIGURE_SIZE])
{
  int i, j;
  printf("game.c    print_blocks\n");
  print("------------------------------");
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      printf(" %d ", mat[i][j]);
    }
    print(" ");
  }
  print("------------------------------");
}

void gen_blocks(int block[FIGURE_SIZE][FIGURE_SIZE])
{
  int i, j, rnd, x, y, lastrnd = -1;
  printf("game.c    gen_blocks\n");
  rnd = rand() % 4 + 1;
  for (i = 0; i < FIGURE_SIZE; i++)
    for (j = 0; j < FIGURE_SIZE; j++)
      block[i][j] = 0;
  /*
    Algo :
    On démarre au centre de la matrice (x = y = FIGURE_SIZE / 2)
    Tant que i < rnd :
      on met à 1 les coords
      puis on tire une direction aléatoire
      puis on met a jour x et y en fonction de cette direction
    fin tant que
    */
  x = FIGURE_SIZE / 2;
  y = x;
  for (i = 0; i < rnd; i++)
  {
    block[x][y] = 1;
    j = rand() % 4;
    if (j == lastrnd)
    {
      i--;
    }
    lastrnd = (j * -1) % 4;
    switch (j)
    {
    case 0:
      /* Droite */
      x++;
      break;
    case 1:
      /* Gauche */
      x--;
      break;
    case 2:
      /* Haut */
      y--;
      break;

    default:
      y++;
      /* Bas */
      break;
    }
    if (x >= FIGURE_SIZE)
    {
      i--;
      x = FIGURE_SIZE - 1;
    }
    if (x < 0)
    {
      i--;
      x = 0;
    }
    if (y >= FIGURE_SIZE)
    {
      y = FIGURE_SIZE - 1;
      i--;
    }
    if (y < 0)
    {
      y = 0;
      i--;
    }
  }
  rnd = rand() % 4;
  for (i = 0; i < rnd; i++)
  {
    rot_90(block);
  }
  set_colors(block);
}

void next_turn(screen *current)
{
  int i;
  printf("game.c    next_turn\n");
  for (i = 1; i < MAX_FIGURES; i++)
  {
    current->jeu.figures[i - 1] = current->jeu.figures[i];
  }
  gen_blocks(current->jeu.figures[MAX_FIGURES - 1].blocks);
  write_save(&current->jeu);
}

game init_game(game g)
{
  int i, j;
  printf("game.c    init_game\n");
  g.case_size = 0;
  g.slot = 0;
  for (i = 0; i < MAX_FIGURES; i++)
  {
    gen_blocks(g.figures[i].blocks);
    print_blocks(g.figures[i].blocks);
  }
  for (i = 0; i < NB_LINES; i++)
    for (j = 0; j < NB_COLS; j++)
    {
      g.grid[i][j] = 0;
    }
  return g;
}

int est_fini(game g)
{
  int i;
  printf("game.c    est_fini\n");
  for (i = 0; i < NB_COLS; i++)
  {
    if (g.grid[3][i] != 0)
    {
      return 1; /*Si return 1 alors il y a un bloque sur la première ligne, le jeu est fini*/
    }
  }
  return 0;
}

int verif_sienbas(game g)
{
  int i, j;
  printf("game.c    verif_sienbas\n");
  for (j = 0; j < NB_COLS; j++)
  {
    if (g.grid[NB_LINES - 1][j] > (MAX_COLOR - 1) / 2)
    {
      return 1;
    }
  }
  for (i = NB_LINES - 1; i > 0; i--)
  {
    for (j = 0; j < NB_COLS; j++)
    {
      if (g.grid[i][j] > 0 && g.grid[i][j] <= (MAX_COLOR - 1) / 2 && g.grid[i - 1][j] > (MAX_COLOR - 1) / 2)
      {
        return 1;
      }
    }
  }
  return 0;
}

game verif_lignecomplete(game g)
{                                                                                                            /*pour chaque ligne, vérifie si la première valeur de la ligne est 1*/
  int i, j = 0, k, l, verif[NB_LINES] = {0}; /*si elle vaut 1 alors ça check toute les valeurs de la ligne*/ /*j fait le count et compte le nombre d'itération de 1*/
  printf("game.c    verif_lignecomplete\n");                                                                 /*si j est égal au NB_COLS, alors toute la ligne vaut 1*/
  for (i = 0; i < NB_LINES; i++)
  {
    if (g.grid[i][j] > 0 && g.grid[i][j] <= (MAX_COLOR - 1) / 2)
    {
      for (k = 0; k < NB_COLS; k++)
      {
        if (g.grid[i][k] > 0 && g.grid[i][k] <= (MAX_COLOR - 1) / 2)
        {
          j++;
        }
      }
      if (j == NB_COLS)
      {
        verif[i] = 1;
      }
      j = 0;
    }
  }
  for (i = 0; i < NB_LINES; i++)
  {
    if (verif[i] == 1)
    {
      for (j = 0; j < NB_COLS; j++)
      {
        g.grid[i][j] = 0;
        for (l = i; l > 3; l--)
        {
          g.grid[l][j] = g.grid[l - 1][j];
        }
      }
    }
  }
  return g;
}

int verif_jeufini(game g)
{
  int i;
  printf("game.c    verif_jeufini\n");
  for (i = 0; i < NB_COLS; i++)
  {
    if (g.grid[3][i] > 0)
    {
      return 1;
    }
  }
  return 0;
}

game descente(game g)
{
  int i, j;
  printf("game.c    descente\n");
  for (i = NB_LINES - 1; i >= 0; i--)
  {
    for (j = 0; j < NB_COLS; j++)
    {
      if (g.grid[i][j] > (MAX_COLOR - 1) / 2)
      {
        g.grid[i + 1][j] = g.grid[i][j];
        g.grid[i][j] = 0;
      }
    }
  }
  return g;
}

game mouv_gauche(game g, int y)
{
  int i, j;
  printf("game.c    mouv_gauche\n");
  for (i = 0; i < NB_LINES; i++)
  {
    if (g.grid[i][0] > (MAX_COLOR - 1) / 2)
    {
      return g;
    }
  }
  if (y == 0)
  {
    return g;
  }
  for (i = 0; i < NB_LINES; i++)
  {
    for (j = 1; j < NB_COLS; j++)
    {
      if (g.grid[i][j - 1] > 0 && g.grid[i][j - 1] <= (MAX_COLOR - 1) / 2 && g.grid[i][j] > (MAX_COLOR - 1) / 2)
      {
        return g;
      }
    }
  }
  for (j = 1; j < NB_COLS; j++)
  {
    for (i = 0; i < NB_LINES; i++)
    {
      if (g.grid[i][j] > (MAX_COLOR - 1) / 2)
      {
        g.grid[i][j - 1] = g.grid[i][j];
        g.grid[i][j] = 0;
      }
    }
  }
  print("FIN DE LA FONCTION");
  return g;
}

game mouv_droite(game g, int compteur, int y)
{
  int i, j;
  printf("game.c    mouv_droite\n");
  for (i = 0; i < NB_LINES; i++)
  {
    if (g.grid[i][NB_COLS - 1] > (MAX_COLOR - 1) / 2)
    {
      return g;
    }
  }
  for (i = 0; i < NB_LINES; i++)
  {
    for (j = 0; j < NB_COLS - 1; j++)
    {
      if (g.grid[i][j + 1] > 0 && g.grid[i][j + 1] <= (MAX_COLOR - 1) / 2 && g.grid[i][j] > (MAX_COLOR - 1) / 2)
      {
        return g;
      }
    }
  }
  if (y + FIGURE_SIZE - 1 >= NB_COLS && compteur < FIGURE_SIZE)
  {
    return g;
  }
  for (j = NB_COLS - 2; j >= 0; j--)
  {
    for (i = 0; i < NB_LINES; i++)
    {
      if (g.grid[i][j] > (MAX_COLOR - 1) / 2)
      {
        g.grid[i][j + 1] = g.grid[i][j];
        g.grid[i][j] = 0;
      }
    }
  }
  return g;
}

game mouv_rot(game g, int x, int y)
{
  int i, j;
  int figure[FIGURE_SIZE][FIGURE_SIZE], figure2[FIGURE_SIZE][FIGURE_SIZE];
  printf("game.c    mouv_rot\n");
  x -= 3;
  if (x < 0)
  {
    return g;
  }
  if (y <= 0)
  {
    return g;
  }
  if (y > NB_COLS - 4)
  {
    return g;
  }
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      figure[i][j] = g.grid[i + x][j + y];
      figure2[i][j] = g.grid[i + x][j + y];
      if (figure[i][j] > 0 && figure[i][j] <= (MAX_COLOR - 1) / 2)
      {
        figure[i][j] = 0;
      }
    }
  }
  rot_90(figure);
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      if (figure2[i][j] > 0 && figure2[i][j] <= (MAX_COLOR - 1) / 2 && figure[i][j] != 0)
      {
        return g;
      }
    }
  }
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      g.grid[i + x][j + y] = figure[i][j];
      if (figure2[i][j] > 0 && figure2[i][j] <= (MAX_COLOR - 1) / 2)
      {
        g.grid[i + x][j + y] = figure2[i][j];
      }
    }
  }
  return g;
}

void fixer_bloque(screen *current)
{
  int i, j;
  for (i = 0; i < NB_LINES; i++)
  {
    for (j = 0; j < NB_COLS; j++)
    {
      if (current->jeu.grid[i][j] > (MAX_COLOR - 1) / 2)
      {
        current->jeu.grid[i][j] = current->jeu.grid[i][j] - 10;
      }
    }
  }
  next_turn(current);
  update_figures(current);
}

void init_code_couleur(code_couleur c[MAX_COLOR])
{
  c[0].color = MLV_COLOR_BLACK;
  c[1].color = MLV_COLOR_RED;
  c[2].color = MLV_COLOR_ORANGE;
  c[3].color = MLV_COLOR_YELLOW;
  c[4].color = MLV_COLOR_GREEN;
  c[5].color = MLV_COLOR_CYAN;
  c[6].color = MLV_COLOR_BLUE;
  c[7].color = MLV_COLOR_SEA_GREEN;
  c[8].color = MLV_COLOR_VIOLET;
  c[9].color = MLV_COLOR_DARK_BLUE;
  c[10].color = MLV_COLOR_BLACK;
  c[11].color = MLV_COLOR_RED;
  c[12].color = MLV_COLOR_ORANGE;
  c[13].color = MLV_COLOR_YELLOW;
  c[14].color = MLV_COLOR_GREEN;
  c[15].color = MLV_COLOR_CYAN;
  c[16].color = MLV_COLOR_BLUE;
  c[17].color = MLV_COLOR_SEA_GREEN;
  c[18].color = MLV_COLOR_VIOLET;
  c[19].color = MLV_COLOR_DARK_BLUE;
}

void gen_ligne(int grid[NB_LINES][NB_COLS], int bloque[FIGURE_SIZE][FIGURE_SIZE], int compteur, int y)
{
  int j;
  printf("game.c    gen_ligne\n");
  if (compteur > 3)
  {
    return;
  }
  /*print_blocks(bloque);
   printf("%d\n", compteur);*/
  for (j = 0; j < FIGURE_SIZE; j++)
  {
    grid[0][j + y] = bloque[FIGURE_SIZE - 1 - compteur][j];
    /*printf("%d ", grid[0][j+y]);*/
  }
  /*printf("\n");*/
}
