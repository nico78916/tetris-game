#define DEFAULT_SCREEN_WIDTH 1024
#define DEFAULT_SCREEN_HEIGHT 576

#define MAX_SCREEN_WIDTH 1920
#define MAX_SCREEN_HEIGHT 1080

#define MAX_BUTTON 10

#define MAX_STR 64

char* valeurs[2] = {"ON","OFF"};
int resolution_16_9_w[7] = {
    3840,
    2560,
    1920,
    1600,
    1366,
    1280,
    1024
};
int resolution_16_9_l[7] = {
    2160,
    1440,
    1080,
    900,
    768,
    720,
    576
};

typedef enum {MENU,OPTIONS}screen_id;

typedef struct {
    int x,y,width,height;
    char* label;
} button;

typedef struct {
    int x,y,width,height;
    char* value,placeholder;
}input;

typedef struct{
    int width,height,btncount;
    screen_id id;
    button* buttons;
    input input;
    MLV_Button_state pressed;
    MLV_Button_state long_press;
} screen;

MLV_Font* default_font; 
MLV_Font* western_font;
MLV_Font* title_font;