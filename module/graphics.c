#include <MLV/MLV_all.h>
#include "../header/api.h"
#include "../header/const.h"
#include "../header/game.h"
#include "../header/save.h"
#include "../header/update.h"

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

void get_screen_resolution_text(screen current, char *str)
{
  char res[12] = "RESOLUTION ", stw[5], sth[5]; /* 21 = nb caracteres possible pour un long int */
  sprintf(sth, "%d", current.height);
  sprintf(stw, "%d", current.width);
  strcat(str, res);
  strcat(str, stw);
  strcat(str, "x");
  strcat(str, sth);
}

screen gen_menu(screen current)
{
  int i, lastY, bw, bh, tw, th, width, height;
  char *labels[5] = {"NOUVELLE PARTIE", "CONTINUER", "CHARGER", "OPTIONS", "QUITTER"};
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
    MLV_draw_text_box_with_font(width / 2 - bw / 2, lastY, bw, bh, labels[i], default_font, 20, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GREY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    b.height = bh;
    b.width = bw;
    b.x = width / 2 - bw / 2;
    b.y = lastY;
    b.label = labels[i];
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
  width = current.width;
  height = current.height;
  get_screen_resolution_text(current, chaine);
  labels[0] = chaine;
  bw = width / 2;
  bh = height / 10;
  lastY = height / current.btncount - bh / 2;
  MLV_get_size_of_text_with_font("OPTIONS", &tw, &th, title_font);
  MLV_draw_text_with_font(width / 2 - tw / 2, height / 10 - th / 2, "OPTIONS", title_font, MLV_COLOR_WHITE);
  for (i = 0; i < current.btncount; i++)
  {
    button b;
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
  return current;
}

screen gen_pseudo(screen current)
{
  int i, ix, iy, malus = 0, tw, th, width, height, curTouche = 0, cooldown = 10, cd2 = 100 / 3, pf = 1, mouseX, mouseY;
  char *touches[43] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "\n", "A", "Z", "E", "R", "T", "Y", "U", "I", "O", "P", "\n", "Q", "S", "D", "F", "G", "H", "J", "K", "L", "M", "\n", "W", "X", "C", "V", "B", "N", "\n", "VALIDER", "CORRIGER", "EFFACER"};
  char pseudo[MAX_STR] = "";
  clavier clav;
  player_info infos;
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
  current.id = PSEUDO;
  while (1)
  {
    MLV_delay_according_to_frame_rate();
    if (strlen(pseudo) == 64)
    {
      MLV_draw_text_box_with_font(width / 100 - width / 200, height / 10 + th / 2, width - width / 95, height / 10, pseudo, default_font, 20, MLV_COLOR_RED, MLV_COLOR_RED, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    }
    else
    {
      if (cd2 <= 0)
      {
        cd2 = 110 / 3;
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
        cooldown = 10;
        i = strlen(pseudo);
        if (pseudo[i - 1] == '_')
        {
          i--;
        }
        if (curTouche < 10)
        {
          if(strlen(pseudo) < MAX_STR){
            pseudo[i] = clav.numerique[curTouche].label[0];
          }
        }
        else if (curTouche < 36)
        {
          if(strlen(pseudo) < MAX_STR)
            pseudo[i] = clav.alpha[curTouche - 10].label[0];
        }
        else if (curTouche == 36)/* Valider */
        {
          MLV_draw_filled_rectangle(0,height/4,current.width,current.height,MLV_COLOR_BLACK);
          if ( strlen(pseudo) < 4 || (pseudo[strlen(pseudo)] == '\0' && strlen(pseudo) == 4))
          {
            MLV_draw_text_with_font(0,current.height/4,"Il faut au minimum 4 caract??res",default_font,MLV_COLOR_RED);
          }else if(pseudo[0] < '9' && pseudo[0] > '0'){
            MLV_draw_text_with_font(0,current.height/4,"Le premier caract??re doit ??tre une lettre",default_font,MLV_COLOR_RED);
          }else{
            if(pseudo[strlen(pseudo)-1] == '_'){
              pseudo[strlen(pseudo)-1] = '\0';
            }
            strcpy(infos.name, pseudo);
            infos.score = current.jeu.players[0].score;
            set_score(infos);
            current.last_screen_id = MENU;
            return current;
            break;
          }
        }
        else if (curTouche == 37)
        {
          if(pseudo[strlen(pseudo) - 1] == '_'){
            pseudo[strlen(pseudo) - 2] = '_';
            pseudo[strlen(pseudo) - 1] = '\0';
          }else{
            pseudo[strlen(pseudo) - 1] = '\0';
          }
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

screen gen_over(screen current)
{
  int a, b, d, e, f, g, i;
  char score[256];
  char scored[256];
  player_info scoreboard[10];
  button c;
  MLV_Color col;
  get_scoreboard(scoreboard);
  sprintf(score, "SCORE : %d", current.jeu.players[0].score);
  MLV_get_size_of_text_with_font("GAME OVER", &a, &b, title_font);
  MLV_draw_text_with_font(current.width / 2 - a / 2, b, "GAME OVER", title_font, MLV_COLOR_WHITE);

  MLV_get_size_of_text_with_font(score, &d, &e, default_font);
  MLV_draw_text_with_font(current.width / 2 - d / 2, b * 2 + e, score, default_font, MLV_COLOR_WHITE);
  MLV_draw_text_box_with_font(current.width / 4, current.height - current.height / 8, current.width / 2, current.height / 16, "ENTRER MON PSEUDO", default_font, 20, MLV_COLOR_GREY, MLV_COLOR_BLACK, MLV_COLOR_WHITE, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
  for (i = 0; i < 10; i++)
  {
    if(scoreboard[i].score > current.jeu.players[0].score){
      col = MLV_COLOR_RED;
    }else{
      col = MLV_COLOR_GREEN;
    }
    MLV_get_size_of_text_with_font(scoreboard[i].name, &f, &g, default_font);
    MLV_draw_text_with_font(current.width / 2 - f, b * 2 + e * 2 + g * (i + 1), scoreboard[i].name, default_font, col);
    sprintf(scored, "%d", scoreboard[i].score);
    MLV_get_size_of_text_with_font(scored, &f, &g, default_font);
    MLV_draw_text_with_font(current.width / 2 + 10, b * 2 + e * 2 + g * (i + 1), scored, default_font, col);
  }
  c.height = current.height / 16;
  c.width = current.width / 2;
  if(current.jeu.players[0].score < scoreboard[9].score){
    c.label = "RETOURNER AU MENU";
    current.jeu.players[0].score = 0;
    write_save(&current.jeu);
  }else{
    c.label = "ENTRER MON PSEUDO";
  }
  c.x = current.width / 4;
  c.y = current.height - current.height / 8;
  current.btncount = 1;
  current.buttons[0] = c;
  current.id = OVER;
  return current;
}

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

void update_figures(screen *current)
{
  int i, j, k;
  char score[256] = "";
  code_couleur code_couleur[MAX_COLOR];
  init_code_couleur(code_couleur);
  sprintf(score, "SCORE : %d",current->jeu.players[0].score);
  MLV_draw_text_with_font(current->jeu.width + current->jeu.x + FIGURE_SIZE * current->jeu.case_size,0,score,western_font,MLV_COLOR_GREEN);
  for (k = 1; k < MAX_FIGURES; k++){/* figure[0] c'est la figure qui tombe actuellement donc pas besoin*/
        current->jeu.figures[k].x = current->jeu.width + current->jeu.x;
        current->jeu.figures[k].y = FIGURE_SIZE * current->jeu.case_size * (k-1);
        do{
          j = 0;
          for(i=0;i<FIGURE_SIZE;i++){
            if(current->jeu.figures[k].blocks[i][FIGURE_SIZE-1] > 0){
              j = 1;
            }
          }
          if(j == 0){
            mouv_droite_figure(current->jeu.figures[k].blocks);
          }
        }while(j == 0);
        do{
          j = 0;
          for(i=0;i<FIGURE_SIZE;i++){
            if(current->jeu.figures[k].blocks[FIGURE_SIZE-1][i] > 0){
              j = 1;
            }
          }
          if(j != 1){
            descente_figure(current->jeu.figures[k].blocks);
          }
        }while(j == 0);
        draw_figure(current->jeu.figures[k], current->jeu.case_size, current->jeu.colors);
  }
}

screen gen_pause(screen current)
{
  int i,width, height, bw, bh,start;
  char* label[3] = {"SAUVEGARDER","OPTIONS","RETOUR AU JEU"};
  MLV_clear_window(MLV_COLOR_BLACK);
  current.btncount = 3;
  width = current.width;
  height = current.height;
  bw = width / 2;
  bh = height / 2;
  current.id = PAUSE;
  MLV_get_size_of_text_with_font("PAUSE",&bw,&bh,title_font);
  start = bh + bh/2;
  MLV_draw_text_with_font(width / 2 - bw / 2, bh / 2, "PAUSE", title_font, MLV_COLOR_GREY);
  bw = width/2;
  bh = height / (current.btncount*2);
  for(i=0;i<current.btncount;i++){
    current.buttons[i].x = width / 2 - bw / 2;
    current.buttons[i].y = start +bh*2*i;
    current.buttons[i].width = bw;
    current.buttons[i].height = bh;
    current.buttons[i].label = label[i];
  }
  return current;
}

screen gen_game(screen current)
{
  int height, i, j, finw = 0, count = 0, couleur, compteur, mouv_x, mouv_y,*pmouv_x,*pmouv_y, cooldown = 2, sec_counter = 0;
  code_couleur code_couleur[MAX_COLOR];
  init_code_couleur(code_couleur);
  current.btncount = 0;
  height = current.height;
  height = current.height;
  current.id = GAME;
  current.last_screen_id = OVER;
  current.jeu.case_size = height / NB_LINES;
  current.jeu.x = (current.width / 2 - (NB_COLS * current.jeu.case_size) / 2);
  current.jeu.y = height / 100;
  current.jeu.width = current.jeu.case_size * NB_COLS;
  current.jeu.height = current.jeu.case_size * NB_LINES;
  MLV_change_frame_rate(24);
  while (est_fini(current.jeu) == 0)
  {
    MLV_clear_window(MLV_COLOR_BLACK);
    update_figures(&current);
    compteur = 0;
    mouv_x = 0;
    mouv_y = 5;
    pmouv_x = &mouv_x;
    pmouv_y = &mouv_y;
    gen_ligne(current.jeu.grid, current.jeu.figures[0].blocks, compteur, mouv_y);
    compteur += 1;
    while (finw == 0)
    {
      MLV_delay_according_to_frame_rate();
      sec_counter++;
      if(sec_counter >= 24){
        (current.jeu.players[0]).score++;
        sec_counter = 0;
      }
      cooldown --;
      cooldown = cooldown <= 0 ? 0 : cooldown;
      if (count >= 4)
      {/*descente*/
        if (verif_sienbas(current.jeu) == 0)
        {
          current.jeu = descente(current.jeu);
          count = 0;
          mouv_x += 1;
          gen_ligne(current.jeu.grid, current.jeu.figures[0].blocks, compteur, mouv_y);
          compteur += 1;
        }
      }
      else if (count >= 2 && MLV_get_keyboard_state(MLV_KEYBOARD_DOWN) == MLV_PRESSED)
      { /*descente acc??l??r??e*/
        if (verif_sienbas(current.jeu) == 0)
        {
          current.jeu = descente(current.jeu);
          count = 0;
          mouv_x += 1;
          gen_ligne(current.jeu.grid, current.jeu.figures[0].blocks, compteur, mouv_y);
          compteur += 1;
        }
      }

      if (MLV_get_keyboard_state(MLV_KEYBOARD_LALT) == MLV_PRESSED && MLV_get_keyboard_state(MLV_KEYBOARD_END) == MLV_PRESSED)
      { /* Raccourci pour arr??ter le jeux */
        current.id = GAME;
        current.last_screen_id = OVER;
        return current;
      }
      if (MLV_get_keyboard_state(MLV_KEYBOARD_LALT) == MLV_PRESSED && MLV_get_keyboard_state(MLV_KEYBOARD_F4) == MLV_PRESSED)
      { /* Raccourci pour arr??ter le jeux */
        quit_game(&current);
      }

      /*v??rifier que le coup est valide et le faire le cas ??ch??ant*/
      if (MLV_get_keyboard_state(MLV_KEYBOARD_LEFT) == MLV_PRESSED)
      { /*si fl??che gauche, mouvement ?? gauche*/
        if(cooldown == 0){
          current.jeu = mouv_gauche(current.jeu, compteur, pmouv_y);
          cooldown = 2;
        }
      }
      else if (MLV_get_keyboard_state(MLV_KEYBOARD_RIGHT) == MLV_PRESSED)
      { /*si fl??che droite, mouvement ?? droite*/
        if(cooldown == 0){
          current.jeu = mouv_droite(current.jeu, compteur, pmouv_y);
          cooldown = 2;
        }
      }
      else if (MLV_get_keyboard_state(MLV_KEYBOARD_UP) == MLV_PRESSED && compteur > 3 && current.jeu.players[0].score < 2500)
      { /*si fl??che droite, mouvement ?? droite*/
        if(cooldown == 0){
        current.jeu = mouv_rot(current.jeu, pmouv_x, pmouv_y);
        cooldown = 4;
        }
      }
      else if (MLV_get_keyboard_state(MLV_KEYBOARD_p) == MLV_PRESSED || MLV_get_keyboard_state(MLV_KEYBOARD_PAUSE) == MLV_PRESSED || MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) == MLV_PRESSED)
      {
        write_save(&current.jeu);
        current.id = GAME;
        current.last_screen_id = PAUSE;
        while(MLV_get_keyboard_state(MLV_KEYBOARD_p) == MLV_PRESSED || MLV_get_keyboard_state(MLV_KEYBOARD_PAUSE) == MLV_PRESSED || MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) == MLV_PRESSED);
        return current;
      }
      finw = verif_sienbas(current.jeu);
      draw_grid(current.jeu);
      /* dessine les blocks une fois dans la matrice */
      for (i = 0; i < NB_COLS; i++)
      {
        for (j = 0; j < NB_LINES; j++)
        {
          if (current.jeu.grid[j][i] != 0)
          {
            couleur = current.jeu.grid[j][i];
            if(current.jeu.colors == 1){
              MLV_draw_filled_rectangle(current.jeu.x + i * current.jeu.case_size, current.jeu.y + j * current.jeu.case_size, current.jeu.case_size, current.jeu.case_size, code_couleur[couleur].color);
            }else{
              MLV_draw_filled_rectangle(current.jeu.x + i * current.jeu.case_size, current.jeu.y + j * current.jeu.case_size, current.jeu.case_size, current.jeu.case_size, MLV_rgba(255/couleur,255/couleur,255/couleur,255));
            }
          }
        }
      }
      count++;
      MLV_actualise_window();
    }
    /*il faut que le bloque s'int??gre aux bloques fixes*/
    fixer_bloque(&current);
    current.jeu = verif_lignecomplete(current.jeu);
    finw = 0;
    /*le bloque est descendu en bas, appel fonction pour v??rifier si ligne compl??te et donc effacer*/
    /*appel de la fonction pour v??rifier si le jeu est fini et recommence au premier while*/
  }
  if(current.jeu.sound == 1){
    MLV_stop_music();
    current.cursong = MLV_load_music("ressources/game_over.wav");
    MLV_play_music(current.cursong,1.0,1);
  }
  MLV_change_frame_rate(NB_LINES*NB_COLS/14);
  for(i=NB_LINES-1;i>=0;i--){
    if(i%2 == 0){
      for(j=0;j<NB_COLS;j++){
      MLV_delay_according_to_frame_rate();
      MLV_draw_filled_rectangle(current.jeu.x + j * current.jeu.case_size, current.jeu.y + i * current.jeu.case_size, current.jeu.case_size, current.jeu.case_size, code_couleur[9].color);
      MLV_actualise_window();
      }
    }else{
      for(j=NB_COLS-1;j>=0;j--){
        MLV_delay_according_to_frame_rate();
        MLV_draw_filled_rectangle(current.jeu.x + j * current.jeu.case_size, current.jeu.y + i * current.jeu.case_size, current.jeu.case_size, current.jeu.case_size, code_couleur[9].color);
        MLV_actualise_window();
      }
    }
  }
  current.jeu = init_game(current.jeu);
  write_save(&current.jeu);
  current.id = GAME;
  return current;
}

screen gen_load(screen current)
{
  int i, lastY, bw, bh, tw, th, width, height;
  char *labels[6] = {"SAUVEGRADE 1", "SAUVEGARDE 2", "SAUVEGARDE 3", "SAUVEGARDE 4", "SAUVEGARDE 5", "RETOUR"};
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
    MLV_draw_text_box_with_font(width / 2 - bw / 2, lastY, bw, bh, labels[i], default_font, 20, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GREY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    b.height = bh;
    b.width = bw;
    b.x = width / 2 - bw / 2;
    b.y = lastY;
    b.label = labels[i];
    current.buttons[i] = b;
    lastY += bh + height / 16;
  }
  current.id = LOAD;
  return current;
}

screen gen_save(screen current)
{
  int i, lastY, bw, bh, tw, th, width, height;
  char *labels[6] = {"SAUVEGRADE 1", "SAUVEGARDE 2", "SAUVEGARDE 3", "SAUVEGARDE 4", "SAUVEGARDE 5", "RETOUR"};
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
    MLV_draw_text_box_with_font(width / 2 - bw / 2, lastY, bw, bh, labels[i], default_font, 20, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GREY, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    b.height = bh;
    b.width = bw;
    b.x = width / 2 - bw / 2;
    b.y = lastY;
    b.label = labels[i];
    current.buttons[i] = b;
    lastY += bh + height / 16;
  }
  current.id = SAVE;
  return current;
}

void toggleSound(screen *current)
{
  current->jeu.sound = (current->jeu.sound + 1) % 2;
  if(current->jeu.sound == 1){
    MLV_play_music(current->cursong,1.0,-1);
  }else{
    MLV_stop_music();
  }
}

void toggleColor(screen *current)
{
  current->jeu.colors = (current->jeu.colors + 1) % 2;
}

screen switch_widow_type(screen current)
{
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
  return current;
}

screen change_resolution(screen current, int rw, int rh)
{
  if (MLV_is_full_screen())
  {
    current = switch_widow_type(current);
  }
  current.height = rh;
  current.width = rw;
  MLV_change_window_size(current.width, current.height);
  return current;
}