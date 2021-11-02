#define DEFAULT_SCREEN_WIDTH 1024
#define DEFAULT_SCREEN_HEIGHT 576

#define MAX_SCREEN_WIDTH 1920
#define MAX_SCREEN_HEIGHT 1080

#define MAX_BUTTON 10

#define MAX_STR 64

#define NB_RESOLUTION 7

#define MIN_CASE_SIZE 10

#define NB_LINES 20
#define NB_COLS 13

#define MAX_PLAYERS 2

#define MAX_FIGURES 5
#define FIGURE_SIZE 4 /* Attention les figures sont des matrices carré*/

int resolution_16_9_w[NB_RESOLUTION];
int resolution_16_9_l[NB_RESOLUTION];


typedef enum {MENU,OPTIONS,GAME,NEWGAME,LOAD}screen_id;

typedef struct {
    int x,y,width,height;
    char* label;
} button;


typedef struct{
    int x,y;
    int blocks[FIGURE_SIZE][FIGURE_SIZE];
}figure;

typedef struct {
    int score;
    char* name;
} player;

typedef struct {
    int grid[NB_LINES][NB_COLS],case_size,slot,ply_count;
    figure figures[MAX_FIGURES];
    player players[MAX_PLAYERS];

}game;

typedef struct{
    int width,height,btncount,max_width,max_height;/* La différence avec MAX_SCREEN_WIDTH c'est que cette variable prend en compte la résolution "maximale" de l'ordinateur et non du jeu */
    screen_id id;
    game jeu;
    button* buttons;
    MLV_Button_state pressed;
    MLV_Button_state long_press;
} screen;

MLV_Font* default_font; 
MLV_Font* western_font;
MLV_Font* title_font;