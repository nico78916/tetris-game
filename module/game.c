#include <MLV/MLV_all.h>
#include <time.h>
#include "../header/api.h"
#include "../header/const.h"
#include "../header/save.h"
#include "../header/graphics.h"

void copy_blocks(int src[FIGURE_SIZE][FIGURE_SIZE], int dest[FIGURE_SIZE][FIGURE_SIZE])
{
  int i, j;
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      dest[i][j] = src[i][j];
    }
  }
}

void print_blocks(int mat[FIGURE_SIZE][FIGURE_SIZE])
{
  int i, j;
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

void set_colors(int blocks[FIGURE_SIZE][FIGURE_SIZE])
{
  int rnd, i, j;
  rnd = (rand() % ((MAX_COLOR - MAX_COLOR % 2) / 2)) + ((MAX_COLOR - MAX_COLOR % 2) / 2); /*pour la couleur*/
  if (rnd == 0 || rnd == ((MAX_COLOR - MAX_COLOR % 2) / 2))
    rnd = 11;
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      if (blocks[i][j] == 1)
      {
        blocks[i][j] = rnd;
      }
    }
  }
}

int get_empty_figure(game g)
{
  int i, j, k;
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
  copy_blocks(dest, src);
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      dest[FIGURE_SIZE - (i + 1)][j] = src[i][j];
    }
  }
}

void rot_90(int dest[FIGURE_SIZE][FIGURE_SIZE])
{
  transp(dest);
  reverse_row(dest);
}

void rot_m90(int dest[FIGURE_SIZE][FIGURE_SIZE])
{
  transp(dest);
  reverse_col(dest);
}

void gen_blocks(game *g,int index)
{
  int i, j, rnd, x, y, lastrnd = -1;
  if(g->players[0].score > 1000){
    rnd = rand() % 6 + 2;
  }else{
    rnd = rand() % 4 + 1;
  }
  for (i = 0; i < FIGURE_SIZE; i++)
    for (j = 0; j < FIGURE_SIZE; j++)
      g->figures[index].blocks[i][j] = 0;
  /*
    Algo :
    On d??marre au centre de la matrice (x = y = FIGURE_SIZE / 2)
    Tant que i < rnd :
      on met ?? 1 les coords
      puis on tire une direction al??atoire
      puis on met a jour x et y en fonction de cette direction
    fin tant que
    */
  x = FIGURE_SIZE / 2;
  y = x;
  for (i = 0; i < rnd; i++)
  {
    g->figures[index].blocks[x][y] = 1;
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
    rot_90(g->figures[index].blocks);
  }
  set_colors(g->figures[index].blocks);
}

void next_turn(screen *current)
{
  int i, j;
  for (i = 1; i < MAX_FIGURES; i++)
  {
    current->jeu.figures[i - 1] = current->jeu.figures[i];
  }
  gen_blocks(&(current->jeu),MAX_FIGURES-1);
  write_save(&current->jeu);
  for (i = 0; i < MAX_FIGURES; i++)
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      current->jeu.figures[0].blocks[i][j] = current->jeu.figures[0].blocks[i][j] == 0 ? 0 : current->jeu.figures[0].blocks[i][j] + ((MAX_COLOR - MAX_COLOR % 2) / 2);
    }
}

game init_game(game g)
{
  int i, j;
  g.case_size = 0;
  g.slot = 0;
  for (i = 0; i < MAX_FIGURES; i++)
  {
    gen_blocks(&g,i);
  }
  for (i = 0; i < NB_LINES; i++)
    for (j = 0; j < NB_COLS; j++)
    {
      g.grid[i][j] = 0;
    }
  return g;
}

/*Si return 1 alors il y a un bloque sur la premiere ligne, le jeu est fini*/
int est_fini(game g)
{
  int i;
  for (i = 0; i < NB_COLS; i++)
  {
    if (g.grid[0][i] != 0)
    {
      return 1;
    }
  }
  return 0;
}

int verif_sienbas(game g)
{
  int i, j;
  for (j = 0; j < NB_COLS; j++)
  {
    if (g.grid[NB_LINES - 1][j] > (MAX_COLOR) / 2)
    {
      return 1;
    }
  }
  for (i = NB_LINES - 1; i > 0; i--)
  {
    for (j = 0; j < NB_COLS; j++)
    {
      if (g.grid[i][j] > 0 && g.grid[i][j] < ((MAX_COLOR - MAX_COLOR % 2) / 2) && g.grid[i - 1][j] > ((MAX_COLOR - MAX_COLOR % 2) / 2))
      {
        return 1;
      }
    }
  }
  return 0;
}

/*pour chaque ligne, verifie si la premiere valeur de la ligne est 1*/
/*si elle vaut 1 alors ca check toute les valeurs de la ligne*/
/*j fait le count et compte le nombre d'iteration de 1*/
/*si j est egal au NB_COLS, alors toute la ligne vaut 1*/
game verif_lignecomplete(game g)
{
  int i, j = 0, k, l, verif[NB_LINES] = {0}, counter = 0;
  for (i = 0; i < NB_LINES; i++)
  {
    if (g.grid[i][j] > 0 && g.grid[i][j] <= (MAX_COLOR) / 2)
    {
      for (k = 0; k < NB_COLS; k++)
      {
        if (g.grid[i][k] > 0 && g.grid[i][k] <= (MAX_COLOR) / 2)
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
      counter++;
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
  g.players[0].score += 100 * counter;
  return g;
}

game descente(game g)
{
  int i, j;
  for (i = NB_LINES - 1; i >= 0; i--)
  {
    for (j = 0; j < NB_COLS; j++)
    {
      if (g.grid[i][j] > ((MAX_COLOR - MAX_COLOR % 2) / 2))
      {
        g.grid[i + 1][j] = g.grid[i][j];
        g.grid[i][j] = 0;
      }
    }
  }
  return g;
}

game mouv_gauche(game g, int compteur, int *y)
{
  int i, j;
  for (i = 0; i < NB_LINES; i++)
  {
    if (g.grid[i][0] > (MAX_COLOR - 1) / 2)
    {
      return g;
    }
  }
  if (*y == 0 && compteur < FIGURE_SIZE)
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
  *y = *y -1;
  return g;
}

game mouv_droite(game g, int compteur, int *y)
{
  int i, j;
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
  if (*y + FIGURE_SIZE - 1 >= NB_COLS && compteur < FIGURE_SIZE)
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
  *y = *y + 1;
  return g;
}

void fixer_bloque(screen *current)
{
  int i, j;
  for (i = 0; i < NB_LINES; i++)
  {
    for (j = 0; j < NB_COLS; j++)
    {
      if (current->jeu.grid[i][j] > ((MAX_COLOR - MAX_COLOR % 2) / 2))
      {
        current->jeu.grid[i][j] = current->jeu.grid[i][j] - ((MAX_COLOR - MAX_COLOR % 2) / 2);
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
  if (compteur > 3)
  {
    return;
  }
  for (j = 0; j < FIGURE_SIZE; j++)
  {
    grid[0][j + y] = bloque[FIGURE_SIZE - 1 - compteur][j];
  }
}

void montee_figure(int g[FIGURE_SIZE][FIGURE_SIZE])
{
  int i, j;
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    if (g[0][i] > 0)
    {
      return;
    }
  }
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      if (g[i][j] > 0)
      {
        g[i - 1][j] = g[i][j];
        g[i][j] = 0;
      }
    }
  }
}

void descente_figure(int g[FIGURE_SIZE][FIGURE_SIZE])
{
  int i, j;
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    if (g[FIGURE_SIZE - 1][i] > 0)
    {
      return;
    }
  }
  for (i = FIGURE_SIZE - 1; i >= 0; i--)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      if (g[i][j] > 0)
      {
        g[i + 1][j] = g[i][j];
        g[i][j] = 0;
      }
    }
  }
}

void mouv_gauche_figure(int g[FIGURE_SIZE][FIGURE_SIZE])
{
  int i, j;
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    if (g[i][0] > 0)
    {
      return;
    }
  }
  for (j = 1; j < FIGURE_SIZE; j++)
  {
    for (i = 0; i < FIGURE_SIZE; i++)
    {
      if (g[i][j] > 0)
      {
        g[i][j - 1] = g[i][j];
        g[i][j] = 0;
      }
    }
  }
}

void mouv_droite_figure(int g[FIGURE_SIZE][FIGURE_SIZE])
{
  int i, j;
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    if (g[i][FIGURE_SIZE - 1] > 0)
    {
      return;
    }
  }
  for (j = FIGURE_SIZE - 2; j >= 0; j--)
  {
    for (i = 0; i < FIGURE_SIZE; i++)
    {
      if (g[i][j] > 0)
      {
        g[i][j + 1] = g[i][j];
        g[i][j] = 0;
      }
    }
  }
}

game mouv_rot(game g, int *x, int *y)
{
  int i, j, x1, y1, tabx[FIGURE_SIZE] = {0}, taby[FIGURE_SIZE] = {0}, rotx[FIGURE_SIZE] = {0}, roty[FIGURE_SIZE] = {0}, tx = 0, ty = 0, rx = 0, ry = 0, cx = 0, cy = 0, crx = 0, cry = 0, decalage1 = 0, decalage2 = 0, y2, y3, count = 0;
  int figure[FIGURE_SIZE][FIGURE_SIZE], figure2[FIGURE_SIZE][FIGURE_SIZE];
  y2 = *y;
  y3 = *y;
  x1 = *x - 3;
  y1 = *y;
  if (x1 < 0)
  {
    return g;
  }
  while (y2 < 0)
  {
    y2 += 1;
    decalage1 += 1;
  }
  while (y3 > NB_COLS - FIGURE_SIZE)
  {
    y3 -= 1;
    decalage2 += 1;
  }
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      figure[i][j] = g.grid[i + x1][j + y1];
      figure2[i][j] = g.grid[i + x1][j + y1];
      if (figure[i][j] > 0 && figure[i][j] <= (MAX_COLOR - MAX_COLOR % 2) / 2)
      {
        figure[i][j] = 0;
      }
      if (figure[i][j] >= MAX_COLOR)
      {
        figure[i][j] = 0;
      }
    }
  }
  if (decalage1 != 0)
  {
    for (i = 0; i < FIGURE_SIZE; i++)
    {
      for (j = 0; j < decalage1; j++)
      {
        figure[i][j] = 0;
      }
    }
  }
  if (decalage2 != 0)
  {
    for (i = 0; i < FIGURE_SIZE; i++)
    {
      for (j = FIGURE_SIZE - decalage2; j < FIGURE_SIZE; j++)
      {
        figure[i][j] = 0;
      }
    }
  }
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      if (figure[i][j] > (MAX_COLOR - MAX_COLOR % 2) / 2)
      {
        tabx[i] = i + 1;
        taby[j] = j + 1;
      }
    }
  }
  rot_90(figure);
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      if (figure[i][j] > (MAX_COLOR - MAX_COLOR % 2) / 2)
      {
        rotx[i] = i + 1;
        roty[j] = j + 1;
      }
    }
  }
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      if (tabx[i] != 0)
      {
        tx += tabx[i];
        cx += 1;
      }
      if (taby[j] != 0)
      {
        ty += taby[j];
        cy += 1;
      }
      if (rotx[i] != 0)
      {
        rx += rotx[i];
        crx += 1;
      }
      if (roty[j] != 0)
      {
        ry += roty[j];
        cry += 1;
      }
    }
  }
  tx = tx / cx;
  ty = ty / cy;
  rx = rx / crx;
  ry = ry / cry;
  while ((tx != rx && count < 20) || (ty != ry && count < 20))
  {
    if (tx < rx && rotx[0] == 0)
    {
      rx -= 1;
      montee_figure(figure);
    }
    else if (tx > rx && rotx[3] == 0)
    {
      rx += 1;
      descente_figure(figure);
    }
    if (ty < ry && roty[0] == 0)
    {
      ry -= 1;
      mouv_gauche_figure(figure);
    }
    else if (ty > ry && roty[3] == 0)
    {
      ry += 1;
      mouv_droite_figure(figure);
    }
    count += 1;
  }
  y2 = *y;
  y3 = *y;
  decalage1 = 0;
  decalage2 = 0;
  while (y2 < 0)
  {
    y2 += 1;
    decalage1 += 1;
  }
  if (decalage1 != 0)
  {
    for (i = 0; i < FIGURE_SIZE; i++)
    {
      for (j = 0; j < decalage1; j++)
      {
        if (figure[i][j] > (MAX_COLOR - MAX_COLOR % 2) / 2)
        {
          return g;
        }
      }
    }
  }
  while (y3 > NB_COLS - FIGURE_SIZE)
  {
    y3 -= 1;
    decalage2 += 1;
  }
  if (decalage2 != 0)
  {
    for (i = 0; i < FIGURE_SIZE; i++)
    {
      for (j = FIGURE_SIZE - decalage2; j < FIGURE_SIZE; j++)
      {
        if (figure[i][j] > (MAX_COLOR - MAX_COLOR % 2) / 2)
        {
          return g;
        }
      }
    }
  }
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      if (figure2[i][j] > 0 && figure2[i][j] <= (MAX_COLOR - MAX_COLOR % 2) / 2 && figure[i][j] != 0)
      {
        return g;
      }
    }
  }
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      g.grid[i + x1][j + y1] = figure[i][j];
      if (figure2[i][j] > 0 && figure2[i][j] <= (MAX_COLOR - MAX_COLOR % 2) / 2)
      {
        g.grid[i + x1][j + y1] = figure2[i][j];
      }
    }
  }
  return g;
}
