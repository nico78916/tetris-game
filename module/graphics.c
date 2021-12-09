#include <MLV/MLV_all.h>
#include "../header/api.h"
#include "../header/const.h"
#include "../header/game.h"
#include "../header/save.h"
#include "../header/update.h"

/* renvoie la taille réelle de la figure prend un écran, l'index de la figure et les pointeurs dans lesquels mettre les valeurs de retour */
void get_figure_size(screen *current, int fig_index, int *width, int *height)
{
  int i, j, lineSize, colSize;
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      lineSize += current->jeu.figures[fig_index].blocks[i][j] == 0 ? 0 : current->jeu.case_size;
      colSize += current->jeu.figures[fig_index].blocks[j][i] == 0 ? 0 : current->jeu.case_size;
    }
    if (lineSize > *width)
    {
      *width = lineSize;
    }
    lineSize = 0;
    if (colSize > *height)
    {
      *height = colSize;
    }
    colSize = 0;
  }
}

void prompt(screen cur, char *message, int display_time)
{
  int w, h;
  printf("graphics.c    prompt\n");
  MLV_get_size_of_text_with_font(message, &w, &h, western_font);
  MLV_draw_text_with_font(cur.width / 2 - w, cur.height / 2 - h, message, western_font, MLV_COLOR_WHITE);
  MLV_wait_milliseconds(display_time);
  MLV_draw_text_with_font(cur.width / 2 - w, cur.height / 2 - h, message, western_font, MLV_COLOR_BLACK);
}

/**
 * @return retourne l'index du bouton en cas de réussite ou -1 en cas d'échec
 */
int get_hovered_button(button *bts, int x, int y, int btn_size)
{
  int bx, by, bw, bh;
  while (btn_size > 0)
  {
    btn_size -= 1;
    bx = bts[btn_size].x;
    by = bts[btn_size].y;
    bw = bts[btn_size].width;
    bh = bts[btn_size].height;
    if (x > bx && x < bx + bw && y > by && y < by + bh)
    {
      return btn_size;
    }
  }
  return -1;
}

/**
 * @param str de taille 21 EXACTEMENT
 */
void get_screen_resolution_text(screen current, char *str)
{
  char res[12] = "RESOLUTION ", stw[5], sth[5]; /* 21 = nb caracteres possible pour un long int */
  printf("graphics.c    get_screen_resolution_text\n");
  sprintf(sth, "%d", current.height);
  sprintf(stw, "%d", current.width);
  print(stw);
  print(sth);
  strcat(str, res);
  strcat(str, stw);
  strcat(str, "x");
  strcat(str, sth);
}

screen gen_menu(screen current)
{
  int i, lastY, bw, bh, tw, th, width, height;
  char *labels[5] = {"NOUVELLE PARTIE", "CONTINUER", "CHARGER", "OPTIONS", "QUITTER"};
  printf("graphics.c    gen_menu\n");
  current.btncount = 5;
  MLV_clear_window(MLV_COLOR_BLACK);
  width = current.width;
  height = current.height;
  bw = width / 2;
  bh = height / 10;
  lastY = height / (current.btncount - 1) - bh / 2;
  MLV_get_size_of_text_with_font("TETE RISSE", &tw, &th, title_font);
  MLV_draw_text_with_font(width / 2 - tw / 2, height / 10 - th / 2, "TETE RISSE", title_font, MLV_COLOR_WHITE);
  for (i = 0; i < current.btncount; i++)
  {
    button b;
    printf("%d\n", i);
    /*char a[MAX_STR];*/
    MLV_draw_text_box_with_font(width / 2 - bw / 2, lastY, bw, bh, labels[i], default_font, 20, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GREY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    b.height = bh;
    b.width = bw;
    b.x = width / 2 - bw / 2;
    b.y = lastY;
    b.label = labels[i];
    /*strcpy(b.label,labels[i]);*/
    current.buttons[i] = b;
    lastY += bh + height / 16;
  }
  current.id = MENU;
  return current;
}

screen gen_option(screen current)
{
  int i, lastY, bw, bh, tw, th, width, height;
  char chaine[MAX_STR] = "";
  char *labels[5];
  printf("graphics.c    gen_option\n");
  current.btncount = 5;
  if (MLV_is_full_screen())
  {
    labels[1] = "MODE FULLSCREEN";
  }
  else
  {
    labels[1] = "MODE WIDOWED";
  }
  labels[2] = current.jeu.sound == 1 ? "SON : ON" : "SON : OFF";
  labels[3] = current.jeu.colors == 1 ? "COULEURS : ON" : "COULEURS : OFF";
  labels[4] = "RETOUR";
  MLV_clear_window(MLV_COLOR_BLACK);
  print("[gen_option] INIT");
  printf("w = %d, h=%d, btn_c = %d, id = %d\n", current.width, current.height, current.btncount, current.id);
  width = current.width;
  height = current.height;
  /* POURQUOI ???? POURQUOI NOUS FAIRE ÇA ? */
  get_screen_resolution_text(current, chaine);
  labels[0] = chaine;
  printf("%s\n", chaine);
  /* Avec un malloc() ça aurait pris 2 lignes : sprintf(chaine,"%dx%d",width,height); ... free(chaine); */
  bw = width / 2;
  bh = height / 10;
  lastY = height / current.btncount - bh / 2;
  MLV_get_size_of_text_with_font("OPTIONS", &tw, &th, title_font);
  MLV_draw_text_with_font(width / 2 - tw / 2, height / 10 - th / 2, "OPTIONS", title_font, MLV_COLOR_WHITE);
  for (i = 0; i < current.btncount; i++)
  {
    button b;
    /*char a[MAX_STR];*/
    MLV_draw_text_box_with_font(width / 2 - bw / 2, lastY, bw, bh, labels[i], default_font, 20, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GREY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    b.height = bh;
    b.width = bw;
    b.x = width / 2 - bw / 2;
    b.y = lastY;
    b.label = labels[i];
    current.buttons[i] = b;
    lastY += bh + height / 16;
  }
  current.id = OPTIONS;
  print("[gen_option] End of function");
  return current;
}

/*
Génère le menu pour entrer son pseudo
La raison qui m'a pousser à ne pas utiliser MLV_input, c'est avant tout car je n'ai pas tout de suite
imaginer comment allier la souris les flèche ainsi que le clavier ensuite j'avais envie d'implémenter
cette fonctionalité à ma manière
*/
screen gen_pseudo(screen current)
{
  int i, ix, iy, malus = 0, tw, th, width, height, curTouche = 0, cooldown = 10, cd2 = 100 / 3, pf = 1, mouseX, mouseY;
  char *touches[43] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "\n", "A", "Z", "E", "R", "T", "Y", "U", "I", "O", "P", "\n", "Q", "S", "D", "F", "G", "H", "J", "K", "L", "M", "\n", "W", "X", "C", "V", "B", "N", "\n", "VALIDER", "CORRIGER", "EFFACER"};
  char pseudo[MAX_STR] = "";
  clavier clav;
  printf("graphics.c    gen_pseudo\n");
  current.btncount = 0;
  MLV_clear_window(MLV_COLOR_BLACK);
  width = current.width;
  height = current.height;
  ix = 2;
  MLV_get_size_of_text_with_font("Entrez votre pseudo", &tw, &th, title_font);
  iy = height / 2;
  MLV_draw_text_with_font(width / 2 - tw / 2, height / 10 - th / 2, "Entrez votre pseudo", title_font, MLV_COLOR_WHITE);
  for (i = 0; i < CLAVIER_NUMERIQUE + CLAVIER_ALPHA + 7; i++) /* 43 */
  {
    button b;
    if (touches[i][0] == '\n')
    {
      malus++;
      if (malus != 4)
      {
        iy += height / 15 + 5;
        ix = 2;
      }
      continue;
    }
    b.x = ix;
    b.y = iy;
    b.label = touches[i];
    b.width = width / 10 - 5;
    b.height = height / 15;
    MLV_draw_text_box_with_font(b.x, b.y, b.width, b.height, touches[i], default_font, 20, MLV_COLOR_GREY, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    if (i < CLAVIER_NUMERIQUE)
    {
      clav.numerique[i] = b;
    }
    else if (i < CLAVIER_NUMERIQUE + CLAVIER_ALPHA + 3) /* 39 */
    {
      clav.alpha[i - (CLAVIER_NUMERIQUE + malus)] = b;
    }
    else
    {
      clav.options[i - 40] = b;
    }
    ix += b.width + 5;
  }
  printf("COUNT : i=%d", i);
  current.id = PSEUDO;
  while (1)
  {
    MLV_wait_milliseconds(100 / 6); /* 60 FPS (peut varier selon les ordinateurs)) */
    if (strlen(pseudo) == 64)
    {
      MLV_draw_text_box_with_font(width / 100 - width / 200, height / 10 + th / 2, width - width / 95, height / 10, pseudo, default_font, 20, MLV_COLOR_RED, MLV_COLOR_RED, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    }
    else
    {
      if (cd2 <= 0)
      {
        cd2 = 110 / 3;
        printf("%d\n", pf);
        if (pf == 1 && pseudo[strlen(pseudo)] == '\0')
        {
          pseudo[strlen(pseudo)] = '_';
          pf = 0;
        }
        else if (pf == 0 && pseudo[strlen(pseudo) - 1] == '_')
        {
          pf = 1;
          pseudo[strlen(pseudo) - 1] = '\0';
        }
        else
        {
          pf = 1;
        }
      }
      else
      {
        cd2--;
      }
      MLV_draw_text_box_with_font(width / 100 - width / 200, height / 10 + th / 2, width - width / 95, height / 10, pseudo, default_font, 20, MLV_COLOR_GREEN, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    }
    for (i = 0; i < 39; i++)
    {
      if (i < 10)
      {
        MLV_draw_text_box_with_font(clav.numerique[i].x, clav.numerique[i].y, clav.numerique[i].width, clav.numerique[i].height, clav.numerique[i].label, default_font, 20, MLV_COLOR_GREY, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
      }
      else if (i < 36)
      {
        MLV_draw_text_box_with_font(clav.alpha[i - (10)].x, clav.alpha[i - (10)].y, clav.alpha[i - (10)].width, clav.alpha[i - (10)].height, clav.alpha[i - (10)].label, default_font, 20, MLV_COLOR_GREY, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
      }
      else
      {
        MLV_draw_text_box_with_font(clav.options[i - (36)].x, clav.options[i - (36)].y, clav.options[i - (36)].width, clav.options[i - (36)].height, clav.options[i - (36)].label, default_font, 20, MLV_COLOR_GREY, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
      }
    }
    MLV_get_mouse_position(&mouseX, &mouseY);
    mouseX /= (width / 10 + 2);
    mouseY = (mouseY - height / 2) / (height / 15 + 5);
    if (mouseY >= 0 && mouseX >= 0 && mouseY < 4)
    {
      curTouche = mouseX + mouseY * 10;
    }
    printf("%d\n", mouseY);
    if (curTouche < 10)
    {
      MLV_draw_text_box_with_font(clav.numerique[curTouche].x, clav.numerique[curTouche].y, clav.numerique[curTouche].width, clav.numerique[curTouche].height, clav.numerique[curTouche].label, default_font, 20, MLV_COLOR_GREY, MLV_COLOR_BLACK, MLV_COLOR_GREY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    }
    else if (curTouche < 36)
    {
      MLV_draw_text_box_with_font(clav.alpha[curTouche - (10)].x, clav.alpha[curTouche - (10)].y, clav.alpha[curTouche - (10)].width, clav.alpha[curTouche - (10)].height, clav.alpha[curTouche - (10)].label, default_font, 20, MLV_COLOR_GREY, MLV_COLOR_BLACK, MLV_COLOR_GREY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    }
    else
    {
      MLV_draw_text_box_with_font(clav.options[curTouche - (36)].x, clav.options[curTouche - (36)].y, clav.options[curTouche - (36)].width, clav.options[curTouche - (36)].height, clav.options[curTouche - (36)].label, default_font, 20, MLV_COLOR_GREY, MLV_COLOR_BLACK, MLV_COLOR_GREY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    }
    if (MLV_get_keyboard_state(MLV_KEYBOARD_LEFT) == MLV_PRESSED)
    {
      if (cooldown <= 0)
      {
        cooldown = 10;
        switch (curTouche)
        {
        case 0:
          curTouche = 38;
          break;
        default:
          curTouche--;
          break;
        }
      }
    }
    else if (MLV_get_keyboard_state(MLV_KEYBOARD_RIGHT) == MLV_PRESSED)
    {
      if (cooldown <= 0)
      {
        switch (curTouche)
        {
        case 38:
          curTouche = 0;
          break;
        default:
          curTouche++;
          break;
        }
        cooldown = 10;
      }
    }
    if (MLV_get_keyboard_state(MLV_KEYBOARD_UP) == MLV_PRESSED)
    {
      if (cooldown <= 0)
      {
        if (curTouche < 10)
        {
          curTouche += 39;
        }
        curTouche = (curTouche - 10) % 39;
        cooldown = 10;
      }
    }
    else if (MLV_get_keyboard_state(MLV_KEYBOARD_DOWN) == MLV_PRESSED)
    {
      if (cooldown <= 0)
      {
        curTouche = (curTouche + 10) % 39;
        cooldown = 10;
      }
    }
    if (MLV_get_keyboard_state(MLV_KEYBOARD_RETURN) == MLV_PRESSED || MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED)
    {
      if (cooldown <= 0)
      {
        printf("curTouche = %d\n", curTouche);
        cooldown = 10;
        i = strlen(pseudo);
        if (pseudo[i - 1] == '_')
        {
          i--;
          printf("i=%d\n", i);
        }
        if (curTouche < 10)
        {
          pseudo[i] = clav.numerique[curTouche].label[0];
        }
        else if (curTouche < 36)
        {
          pseudo[i] = clav.alpha[curTouche - 10].label[0];
        }
        else if (curTouche == 36)
        {
          if (strlen(pseudo) > 4)
          {
          }
          break;
        }
        else if (curTouche == 37)
        {
          pseudo[strlen(pseudo) - 1] = '\0';
        }
        else if (curTouche == 38)
        {
          i = 0;
          while (pseudo[i] != '\0')
          {
            pseudo[i] = '\0';
            i++;
          }
        }
      }
    }
    MLV_actualise_window();
    if (cooldown > -1)
    {
      cooldown--;
    }
  }
  return current;
}

/*MENU GAME OVER !!!
Avec SCORE et autre stats
*/

screen gen_over(screen current)
{
  int a, b, d, e, f, g, i;
  char score[256];
  char scored[256];
  char *names[10];
  int scores[10];
  button c;
  get_scoreboard(names, scores);
  sprintf(score, "SCORE : %d", current.jeu.players[0].score);
  MLV_get_size_of_text_with_font("GAME OVER", &a, &b, title_font);
  MLV_draw_text_with_font(current.width / 2 - a / 2, b, "GAME OVER", title_font, MLV_COLOR_WHITE);

  MLV_get_size_of_text_with_font(score, &d, &e, default_font);
  MLV_draw_text_with_font(current.width / 2 - d / 2, b * 2 + e, score, default_font, MLV_COLOR_WHITE);
  MLV_draw_text_box_with_font(current.width / 4, current.height - current.height / 8, current.width / 2, current.height / 16, "ENTRER MON PSEUDO", default_font, 20, MLV_COLOR_GREY, MLV_COLOR_BLACK, MLV_COLOR_WHITE, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
  for (i = 0; i < 10; i++)
  {
    MLV_get_size_of_text_with_font(names[i], &f, &g, default_font);
    MLV_draw_text_with_font(current.width / 2 - f, b * 2 + e * 2 + g * (i + 1), names[i], default_font, MLV_COLOR_WHITE);
    sprintf(scored, "%d", scores[i]);
    MLV_get_size_of_text_with_font(scored, &f, &g, default_font);
    MLV_draw_text_with_font(current.width / 2 + f, b * 2 + e * 2 + g * (i + 1), scored, default_font, MLV_COLOR_WHITE);
  }
  c.height = current.height / 16;
  c.width = current.width / 2;
  c.label = "ENTRER MON PSEUDO";
  c.x = current.width / 4;
  c.y = current.height - current.height / 8;
  current.btncount = 1;
  current.buttons[0] = c;
  current.id = OVER;
  return current;
}

/* GENERATION DU JEU */

void draw_grid(game setup)
{
  int i, j;
  for (i = 0; i < NB_COLS; i++)
  {
    for (j = 0; j < NB_LINES; j++)
    {

      MLV_draw_filled_rectangle(setup.x + i * setup.case_size, setup.y + j * setup.case_size, setup.case_size, setup.case_size, MLV_COLOR_BLACK);
      MLV_draw_rectangle(setup.x + i * setup.case_size, setup.y + j * setup.case_size, setup.case_size, setup.case_size, MLV_COLOR_GREY);
    }
  }
}

void erase_figure(figure fig, int case_size)
{
  int i, j;
  MLV_draw_filled_rectangle(0, 0, case_size * 100, case_size * 100, MLV_COLOR_BLACK);
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      if (fig.blocks[i][j] != 0)
      {
        MLV_draw_filled_rectangle(fig.x + i * case_size, fig.y + j * case_size, case_size, case_size, MLV_COLOR_BLACK);
      }
    }
  }
}

void draw_figure(figure fig, int case_size, int color_on)
{
  int i, j;
  code_couleur code_couleur[MAX_COLOR];
  int couleur;
  init_code_couleur(code_couleur);
  /*print_blocks(fig.blocks);*/
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      if (fig.blocks[i][j] != 0)
      {
        couleur = color_on == 1 ? code_couleur[fig.blocks[i][j]].color : MLV_rgba(255 / fig.blocks[i][j], 255 / fig.blocks[i][j], 255 / fig.blocks[i][j], 255);
      }
      else
      {
        couleur = MLV_COLOR_BLACK;
      }
      MLV_draw_filled_rectangle(fig.x + i * case_size, fig.y + j * case_size, case_size, case_size, couleur);
    }
  }
}

/* met à jours les prochaines figures qui vont descendre */
void update_figures(screen *current)
{
  int i, j, k, width, height;
  code_couleur code_couleur[MAX_COLOR];
  int couleur;
  init_code_couleur(code_couleur);
  /*print_blocks(fig.blocks);*/
  for (k = 0; k < MAX_FIGURES; k++)
    for (i = 0; i < FIGURE_SIZE; i++)
    {
      for (j = 0; j < FIGURE_SIZE; j++)
      {
        if (current->jeu.figures[k].blocks[i][j] != 0)
        {
          couleur = current->jeu.colors == 1 ? code_couleur[current->jeu.figures[k].blocks[i][j]].color : MLV_rgba(255 / current->jeu.figures[k].blocks[i][j], 255 / current->jeu.figures[k].blocks[i][j], 255 / current->jeu.figures[k].blocks[i][j], 255);
        }
        else
        {
          couleur = MLV_COLOR_BLACK;
        }
        current->jeu.figures[k].x = current->jeu.width + current->jeu.x;
        current->jeu.figures[k].y = FIGURE_SIZE * j + 10;
        draw_figure(current->jeu.figures[k], current->jeu.case_size, current->jeu.colors);
      }
    }
}

void print_block(int blocks[FIGURE_SIZE][FIGURE_SIZE])
{
  int i, j;
  for (i = 0; i < FIGURE_SIZE; i++)
  {
    for (j = 0; j < FIGURE_SIZE; j++)
    {
      printf("%d ", blocks[i][j]);
    }
    printf("\n");
  }
}

screen gen_pause(screen current)
{
  int width, height, bw, bh;
  printf("graphics.c    gen_pause\n");
  width = current.width;
  height = current.height;
  bw = width / 2;
  bh = height / 2;
  current.id = PAUSE;
  MLV_draw_text_with_font(width / 2 - bw / 2, height / 2 - bh / 2, "PAUSE", title_font, MLV_COLOR_GREY);
  return current;
}

screen gen_game(screen current)
{
  int height, i, j, finw = 0, count = 0, couleur, compteur, mouv_x, mouv_y, bloqueencours[FIGURE_SIZE][FIGURE_SIZE];
  char *label = "NO NAME";
  button b;
  code_couleur code_couleur[MAX_COLOR];
  printf("graphics.c    gen_game\n");
  MLV_clear_window(MLV_COLOR_BLACK);
  init_code_couleur(code_couleur);
  current.btncount = 0;
  height = current.height;
  height = current.height;
  current.jeu.case_size = height / NB_LINES;
  current.jeu.x = (current.width / 2 - (NB_COLS * current.jeu.case_size) / 2);
  current.jeu.y = height / 100;
  current.jeu.width = current.jeu.case_size * NB_COLS;
  current.jeu.height = current.jeu.case_size * NB_LINES;

  /*ce qu'on peut faire, c'est mettre une couleur unique pour les cubes déjà posés comme ça on peut intégrer le bloc qui descend dans la matrice
      la valeur 0 pour le noir, 1 pour les cubes en bas, ducoup pour faire un traitement sur le cube qui descend, on dit :
      pour chaque valeur différente de 0 et 1, alors faire traitement*/
  /*ça permet d'avoir une seule matrice à gérer au lieu de 2 matrices supperposées mais il ne serait pas possible de faire les rotations*/
  /*penser à ajouter time.h*/
  MLV_change_frame_rate(24);
  while (est_fini(current.jeu) == 0)
  {
    for (i = 0; i < FIGURE_SIZE; i++)
    {
      for (j = 0; j < FIGURE_SIZE; j++)
      { /*création du bloque*/ /*appeler gen_ligne*/
        bloqueencours[i][j] = current.jeu.figures[0].blocks[i][j];
      }
    }
    compteur = 0;
    mouv_x = 0;
    mouv_y = 5;
    gen_ligne(current.jeu.grid, bloqueencours, compteur, mouv_y);
    compteur += 1;
    while (finw == 0)
    { /*boucle qui fait tomber le bloque et attend les instructions*/
      /*penser à mettre un reset buffer*/
      MLV_delay_according_to_frame_rate();
      MLV_get_keyboard_state(MLV_KEYBOARD_DOWN);
      if (count >= 4)
      {
        if (verif_sienbas(current.jeu) == 0)
        {
          current.jeu = descente(current.jeu); /*si atente d'une seconde, le bloque tombe, modification de la grille*/
          count = 0;
          mouv_x += 1;
          gen_ligne(current.jeu.grid, bloqueencours, compteur, mouv_y);
          compteur += 1;
        }
      }
      else if (count >= 2 && MLV_get_keyboard_state(MLV_KEYBOARD_DOWN) == MLV_PRESSED)
      { /*descente accélérée*/
        if (verif_sienbas(current.jeu) == 0)
        {
          current.jeu = descente(current.jeu);
          count = 0;
          mouv_x += 2;
          gen_ligne(current.jeu.grid, bloqueencours, compteur, mouv_y);
          compteur += 1;
        }
      }

      if (MLV_get_keyboard_state(MLV_KEYBOARD_LALT) == MLV_PRESSED && MLV_get_keyboard_state(MLV_KEYBOARD_END) == MLV_PRESSED)
      { /* Raccourci pour arrêter le jeux */
        current.id = GAME;
        return current;
      }
      if (MLV_get_keyboard_state(MLV_KEYBOARD_LALT) == MLV_PRESSED && MLV_get_keyboard_state(MLV_KEYBOARD_F4) == MLV_PRESSED)
      { /* Raccourci pour arrêter le jeux */
        quit_game(&current);
      }

      /*vérifier que le coup est valide et le faire le cas échéant*/
      if (MLV_get_keyboard_state(MLV_KEYBOARD_LEFT) == MLV_PRESSED)
      { /*si flèche gauche, mouvement à gauche*/
        current.jeu = mouv_gauche(current.jeu, mouv_y);
        mouv_y -= 1;
      }
      else if (MLV_get_keyboard_state(MLV_KEYBOARD_RIGHT) == MLV_PRESSED)
      { /*si flèche droite, mouvement à droite*/
        current.jeu = mouv_droite(current.jeu, compteur, mouv_y);
        mouv_y += 1;
      }
      else if (MLV_get_keyboard_state(MLV_KEYBOARD_UP) == MLV_PRESSED && compteur > 3)
      { /*si flèche droite, mouvement à droite*/
        current.jeu = mouv_rot(current.jeu, mouv_x, mouv_y);
      }
      else if (MLV_get_keyboard_state(MLV_KEYBOARD_p) == MLV_PRESSED || MLV_get_keyboard_state(MLV_KEYBOARD_PAUSE) == MLV_PRESSED || MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) == MLV_PRESSED)
      {
        printf("insérer une fonction pour la pause (appuie sur p, pause ou echap)\n");
        write_save(&current.jeu);
        current.id = PAUSE;
        return current;
      }
      /*}*/
      /*à la fin de chaque while, vérifier que le bloque n'est pas descendu en bas sinon return 1*/
      /*for (i = 0; i < NB_LINES; i++)
      {
        for (j = 0; j < NB_COLS; j++)
        {
         printf("%d ", current.jeu.grid[i][j]);
        }
       printf("\n");
      }*/
      if (verif_sienbas(current.jeu) == 1)
      {
        finw = 1;
      }
      count++;
      /*MLV_clear_window(MLV_COLOR_BLACK);*/
      /* ON génère les blocs après la grille pour quelle soit dessiné par dessus ! */
      draw_grid(current.jeu);
      /*for (i = 0; i < NB_COLS; i++)
      {
        for (j = 0; j < NB_LINES; j++)
        {
          MLV_draw_rectangle(current.jeu.x + i * current.jeu.case_size, current.jeu.y + j * current.jeu.case_size, current.jeu.case_size, current.jeu.case_size, MLV_COLOR_GREY);
        }
      }*/
      /* dessine les blocks une fois dans la matrice */
      for (i = 0; i < NB_COLS; i++)
      {
        for (j = 0; j < NB_LINES; j++)
        {
          if (current.jeu.grid[j][i] != 0)
          {
            couleur = current.jeu.grid[j][i];
            MLV_draw_filled_rectangle(current.jeu.x + i * current.jeu.case_size, current.jeu.y + j * current.jeu.case_size, current.jeu.case_size, current.jeu.case_size, code_couleur[couleur].color);
          }
        }
      }
      MLV_actualise_window();
    }
    /*il faut que le bloque s'intègre aux bloques fixes*/
    fixer_bloque(&current);
    current.jeu = verif_lignecomplete(current.jeu);
    finw = 0;
    /*le bloque est descendu en bas, appel fonction pour vérifier si ligne complète et donc effacer*/
    /*appel de la fonction pour vérifier si le jeu est fini et recommence au premier while*/
  }
  printf("est_fini1\n");
  for (i = 0; i < MAX_FIGURES; i++)
  {
    current.jeu.figures[i] = current.jeu.figures[i];
  }
  printf("est_fini2\n");
  current.id = GAME;
  printf("est_fini3\n");
  return current;
}

screen gen_newgame(screen current)
{
  int i, lastY, bw, bh, tw, th, width, height;
  char *labels[5] = {"SOLO", "2 JOUEURS", "3 JOUEURS", "4 JOUEURS", "RETOUR"};
  printf("graphics.c    gen_newgame\n");
  current.btncount = 5;
  MLV_clear_window(MLV_COLOR_BLACK);
  width = current.width;
  height = current.height;
  bw = width / 2;
  bh = height / 10;
  lastY = height / (current.btncount - 1) - bh / 2;
  MLV_get_size_of_text_with_font("Choisissez le nombre de joueurs", &tw, &th, title_font);
  MLV_draw_text_with_font(width / 2 - tw / 2, height / 10 - th / 2, "Choisissez le nombre de joueurs", title_font, MLV_COLOR_WHITE);
  for (i = 0; i < current.btncount; i++)
  {
    button b;
    printf("%d\n", i);
    /*char a[MAX_STR];*/
    MLV_draw_text_box_with_font(width / 2 - bw / 2, lastY, bw, bh, labels[i], default_font, 20, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GREY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    b.height = bh;
    b.width = bw;
    b.x = width / 2 - bw / 2;
    b.y = lastY;
    b.label = labels[i];
    /*strcpy(b.label,labels[i]);*/
    current.buttons[i] = b;
    print("Pass");
    lastY += bh + height / 16;
  }
  current.id = NEWGAME;
  print("Done");
  return current;
}

screen gen_load(screen current)
{
  int i, lastY, bw, bh, tw, th, width, height;
  char *labels[6] = {"SAUVEGRADE 1", "SAUVEGARDE 2", "SAUVEGARDE 3", "SAUVEGARDE 4", "SAUVEGARDE 5", "RETOUR"};
  printf("graphics.c    gen_load\n");
  current.btncount = 6;
  MLV_clear_window(MLV_COLOR_BLACK);
  width = current.width;
  height = current.height;
  bw = width / 2;
  bh = height / (current.btncount * 2);
  lastY = height / (current.btncount - 1) - bh / 2;
  MLV_get_size_of_text_with_font("SAUVEGARDE", &tw, &th, title_font);
  MLV_draw_text_with_font(width / 2 - tw / 2, height / 10 - th / 2, "SAUVEGARDE", title_font, MLV_COLOR_WHITE);
  for (i = 0; i < current.btncount; i++)
  {
    button b;
    /*char a[MAX_STR];*/
    MLV_draw_text_box_with_font(width / 2 - bw / 2, lastY, bw, bh, labels[i], default_font, 20, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GREY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    b.height = bh;
    b.width = bw;
    b.x = width / 2 - bw / 2;
    b.y = lastY;
    b.label = labels[i];
    /*strcpy(b.label,labels[i]);*/
    current.buttons[i] = b;
    lastY += bh + height / 16;
  }
  current.id = LOAD;
  return current;
}
void toggleSound(screen *current)
{
  current->jeu.sound = (current->jeu.sound + 1) % 2;
}

void toggleColor(screen *current)
{
  current->jeu.colors = (current->jeu.colors + 1) % 2;
}

screen switch_widow_type(screen current)
{
  print("SWITCHING STRING TYPE");
  if (MLV_is_full_screen())
  {
    MLV_change_window_size(current.max_width, current.max_height);
    MLV_disable_full_screen();
    MLV_change_window_size(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
    current.width = DEFAULT_SCREEN_WIDTH;
    current.height = DEFAULT_SCREEN_HEIGHT;
  }
  else
  {
    MLV_change_window_size(current.max_width, current.max_height);
    MLV_enable_full_screen();
    current.width = current.max_width;
    current.height = current.max_height;
  }
  print("END");
  return gen_option(current);
}

screen change_resolution(screen current, int rw, int rh)
{
  printf("\nRésolution demandée : %dx%d\n", rw, rh);
  if (MLV_is_full_screen())
  {
    current = switch_widow_type(current);
  }
  current.height = rh;
  current.width = rw;
  MLV_change_window_size(current.width, current.height);
  return gen_option(current);
}

int htoi(char hex)
{
  printf("graphics.c    htoi\n");
  if (hex > '0' && hex < '9')
  {
    return hex - '0';
  }
  else
  {
    return hex - 'A' + 9;
  }
}

MLV_Color convert_hex_to_color(const char *x)
{
  printf("graphics.c    convert_hex_to_color\n");
  if (x[0] != '#')
  {
    return MLV_COLOR_BLACK;
  }
  return MLV_rgba(htoi(x[1]) * 16 + htoi(x[2]), htoi(x[3]) * 16 + htoi(x[4]), htoi(x[5]) * 16 + htoi(x[6]), 255);
}

void draw_game(screen current)
{
  game setup;
  int i, j;
  printf("graphics.c    draw_game\n");
  setup = current.jeu;
  for (i = 0; i < NB_COLS; i++)
  {
    for (j = 0; j < NB_LINES; j++)
    {
      MLV_draw_rectangle(setup.x + i * setup.case_size, setup.y + j * setup.case_size, setup.case_size, setup.case_size, convert_hex_to_color(convertions[setup.grid[i][j]]));
    }
  }
}