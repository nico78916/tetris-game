/**
 * @brief Permet d'avoir l'index du bouton sur lequel se trouve la souris
 * 
 * @param bts la liste des boutons
 * @param x La position X de la souris
 * @param y La position Y de la souris
 * @param btn_size Le nombre de boutons
 * @return int l'index du bouton
 */
int get_hovered_button(button* bts,int x,int y,int btn_size);

/**
 * @brief Génère l'écran du MENU PRINCIPAL
 * 
 * @param current l'écran actuelle
 * @return screen screen l'écran modifié
 */
screen gen_menu(screen current);

/**
 * @brief Génère l'écran des OPTIONS
 * 
 * @param current l'écran actuelle
 * @return screen screen l'écran modifié
 */
screen gen_option(screen current);

/**
 * @brief Génère l'écran du GAME OVER
 * 
 * @param current l'écran actuelle
 * @return screen screen l'écran modifié
 */
screen gen_over(screen current);

/**
 * @brief Génère l'écran du jeu ainsi que le jeu
 * 
 * @param current l'écran actuelle
 * @return screen screen l'écran modifié
 */
screen gen_game(screen current);

/**
 * @brief Génère l'écran du jeu ainsi que le jeu
 * 
 * @param current l'écran actuelle
 * @return screen l'écran modifié
 */
screen gen_load(screen current);

/**
 * @brief Génère l'écran pour sauvegarder la partie
 * 
 * @param current l'écran actuelle
 * @return screen l'écran modifié
 */
screen gen_save(screen current);


/**
 * @brief Change entre le mode plein écran et fenêtré
 * 
 * @param current l'écran actuel
 * @return screen le nouvel écran
 */
screen switch_widow_type(screen current);

/**
 * @brief Change la résolution de l'écran current
 * @param current l'écran
 * @param rw nouvelle resolution de largeur
 * @param rh nouvelle resolution de hauteur
 * @return screen l'écran modifier
*/
screen change_resolution(screen current,int rw,int rh);

/**
 * @brief Get the screen resolution text object
 * 
 * @param current l'écran actuel
 * @param str De longueur 21 exactement
 */
void get_screen_resolution_text(screen current,char* str);


/**
 * @brief Dessine la grille à partir du jeu
 * 
 * @param setup le jeu (current->jeu)
 */
void draw_grid(game setup);

/**
 * @brief Dessine la figure renseigné
 * 
 * @param fig la figure
 * @param case_size la taille de la case
 * @param color_on Les couleurs sont activé ? 0 non/1 oui
 */
void draw_figure(figure fig,int case_size,int color_on);

/**
 * @brief Efface la figure fig
 * 
 * @param fig la figure à effacer
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
 * @return screen l'écran modifier
 */
screen gen_pseudo(screen current);

/**
 * @brief Génère l'écran de PAUSE.
 * 
 * @param current l'écran actuel
 * @return screen l'écran modifier
 */
screen gen_pause(screen current);

/**
 * @brief Active ou désactive le son
 * 
 * @param current 
 */
void toggleSound(screen *current);

/**
 * @brief Active ou désactive les couleurs
 * 
 * @param current l'écran actuel
 */
void toggleColor(screen *current);

/**
 * @brief Met à jour l'affichage des prochaine pièces
 * 
 * @param current l'écran actuel
 */
void update_figures(screen *current);