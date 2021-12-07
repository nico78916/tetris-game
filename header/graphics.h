/**
 * Affiche le texte message pendant display_time pour l'écran cur
 * @param cur L'écran actuelle
 * @param message le texte à afficher
 * @param display_time temps d'affichage en milliseconde
*/
void prompt(screen cur,char* message,int display_time);

/**
 * Permet d'avoir le bouton pour lequel le curseur est au dessus
 * @return retourne l'index du bouton en cas de réussite ou -1 en cas d'échec
*/
int get_hovered_button(button* bts,int x,int y,int btn_size);

/**
 * Génère le menu principale à partir de current
 * @param current l'écran avec les données de résolution
*/
screen gen_menu(screen current);

/**
 * Génère l'écran des options à partir de current
 * @param current l'écran avec les données de résolution
*/
screen gen_option(screen current);

/**
 * Génère l'écran de selection du mode de jeu à partir de current
 * @param current l'écran avec les données de résolution
*/
screen gen_newgame(screen current);

screen gen_over(screen current);

/**
 * Génère l'écran de jeu à partir de current
 * @param current l'écran avec les données de résolution
*/
screen gen_game(screen current);

/**
 * Génère l'écran de sauvegarde à partir de current
 * @param current l'écran avec les données de résolution
*/
screen gen_load(screen current);

/**
 * Passe de plein écran et fenêtré
 * @param current l'écran avec les données de résolution
*/
screen switch_widow_type(screen current);

/**
 * Change la résolution de l'écran current
 * @param current l'écran
 * @param rw nouvelle resolution de largeur
 * @param rh nouvelle resolution de hauteur
*/
screen change_resolution(screen current,int rw,int rh);

/**
 * @param str de taille 21 EXACTEMENT
*/
void get_screen_resolution_text(screen current,char* str);


/**
* Dessine la grille du jeu
* @param current L'écran actuelle
* @param setup le jeu
*/
void draw_grid(game setup);

/**
* Dessine la figure donnée en fonction de la taille des cases
* @param fig la figure
* @param case_size la taille des cases
*/
void draw_figure(figure fig,int case_size,int color_on);

/**
* Efface la figure donnée en fonction de la taille des cases
* @param fig la figure
* @param case_size la taille des cases
*/
void erase_figure(figure fig,int case_size);

/**
 * @brief Initialise le clavier
 * 
 * @param clav le clavier
 * @param starty Le debut du clavier
 * @param size la taille d'une touche
 * @return clavier 
 */
clavier init_clavier(clavier clav,int starty,int size);

/**
 * @brief Génère l'écran des pseudo !
 * 
 * @param current l'écran
 * @return screen 
 */
screen gen_pseudo(screen current);

screen gen_pause(screen current);

void toggleSound(screen *current);

void toggleColor(screen *current);