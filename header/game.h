/**
 * Copie src dans dest
*/
void copy_blocks(int src[][4],int dest[][4]);

/**
 * renvoi l'index de la figure vide
*/
int get_empty_figure(game g);
/**
 * Transposé de dest
*/
void transp(int dest[][FIGURE_SIZE]);
/**
 * Inverse les lignes de la matrice dest
*/
void reverse_row(int dest[][FIGURE_SIZE]);
/**
 * Inverse les colonnes de la matrice dest
*/
void reverse_col(int dest[][FIGURE_SIZE]);

/**
 * Rotation de +90° de la matrice
*/
void rot_90(int dest[][FIGURE_SIZE]);

/**
 * Rotation de -90° de la matrice
*/
void rot_m90(int dest[][FIGURE_SIZE]);

/**
 * Genere les blocks de manière aléatoire
*/
void gen_blocks(int block[][FIGURE_SIZE]);

/**
 * Affiche la matrice
*/
void print_blocks(int mat[][FIGURE_SIZE]);

/**
 * supprime la figure 0 et décale les autres blocks
 * et genere une nouvelle figure
*/
void next_turn(screen *current);

/**
 * Initialise le jeu
*/
game init_game(game g);

/*vérifie si le jeu est fini*/

int est_fini(game g);

/*vérifie si le bloque est arrivé en bas*/

int verif_sienbas(game g);

/*vérifie si une ligne est complète contient ligne_complete*/

game verif_lignecomplete(game g);

/*efface la ligne complète*/

/*void ligne_complete(game g, int verif[NB_LINES]);*/

/*vérifie si le jeu est fini*/

int verif_jeufini(game g);

/*fait déscendre le cube*/

game descente(game g);

void montee_figure(int g[FIGURE_SIZE][FIGURE_SIZE]);

void descente_figure(int g[FIGURE_SIZE][FIGURE_SIZE]);

game mouv_droite(game g, int compteur, int y);

game mouv_gauche(game g, int y);

void mouv_gauche_figure(int g[FIGURE_SIZE][FIGURE_SIZE]);

void mouv_droite_figure(int g[FIGURE_SIZE][FIGURE_SIZE]);

game mouv_rot(game g, int *x, int *y);

void fixer_bloque(screen *current);

void init_code_couleur(code_couleur c[MAX_COLOR]);

void gen_ligne(int grid[NB_LINES][NB_COLS], int bloque[FIGURE_SIZE][FIGURE_SIZE], int compteur, int y);

void set_colors(int blocks[FIGURE_SIZE][FIGURE_SIZE]);
