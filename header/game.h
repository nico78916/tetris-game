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
void transp(int dest[FIGURE_SIZE][FIGURE_SIZE]);
/**
 * Inverse les lignes de la matrice dest
*/
void reverse_row(int dest[FIGURE_SIZE][FIGURE_SIZE]);
/**
 * Inverse les colonnes de la matrice dest
*/
void reverse_col(int dest[FIGURE_SIZE][FIGURE_SIZE]);

/**
 * Rotation de +90° de la matrice
*/
void rot_90(int dest[FIGURE_SIZE][FIGURE_SIZE]);

/**
 * Rotation de -90° de la matrice
*/
void rot_m90(int dest[FIGURE_SIZE][FIGURE_SIZE]);

/**
 * Genere les blocks de manière aléatoire
*/
void gen_blocks(game *g,int index);

/**
 * Affiche la matrice
*/
void print_blocks(int mat[FIGURE_SIZE][FIGURE_SIZE]);

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

/*vérifie si une des lignes est complète et l'efface le cas échant en déplaçant d'une case vers le bas tous les bloques au dessus de la ligne complète*/

game verif_lignecomplete(game g);

/*fait descendre le bloque dans la grille*/

game descente(game g);

/*permet de faire monter la figure dans la fonction de rotation*/

void montee_figure(int g[FIGURE_SIZE][FIGURE_SIZE]);

/*permet de faire descendre la figure dans la fonction de rotation*/

void descente_figure(int g[FIGURE_SIZE][FIGURE_SIZE]);

/*vérifie si le mouvement est permis et déplace à droite le bloque dans la grille*/

game mouv_droite(game g, int compteur, int *y);

/*vérifie si le mouvement est permis et déplace à gauche le bloque dans la grille*/

game mouv_gauche(game g, int compteur, int *y);

/*permet de déplacer la figure à gauche dans la fonction de rotation*/

void mouv_gauche_figure(int g[FIGURE_SIZE][FIGURE_SIZE]);

/*permet de déplacer la figure à droite dans la fonction de rotation*/

void mouv_droite_figure(int g[FIGURE_SIZE][FIGURE_SIZE]);

/*fonction permettant de vérifier que la rotation est possible et de l'effectuer le cas échéant*/

game mouv_rot(game g, int *x, int *y);

/*fixe le bloque dans la grille si la fonction verif_sienbas indique que le bloque est arrivé en bas*/

void fixer_bloque(screen *current);

/*initialise les couleurs des bloques*/

void init_code_couleur(code_couleur c[MAX_COLOR]);

/*génère la première ligne au fur et à mesure de la descente du bloque lorsqu'il est initialisé*/

void gen_ligne(int grid[NB_LINES][NB_COLS], int bloque[FIGURE_SIZE][FIGURE_SIZE], int compteur, int y);

/*permet de définir la couleur d'un bloque*/

void set_colors(int blocks[FIGURE_SIZE][FIGURE_SIZE]);
