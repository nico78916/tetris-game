typedef struct {
    unsigned int x,y,margeX,margeY,id;
    int width,height;
    bool hovered,hide,drawn;
    MLV_Color color,hovercolor;
    MLV_Font* font;
    char* label;
} button;

typedef struct {
    MLV_Color color;
    int x,y;
} block;

typedef struct {
    int x,y,count;
    block center;
    block* block_list;
} figure;

typedef struct{
    button* values;
    int length;
} button_list;

typedef struct{
    block* values;
    int length;
} block_list;

typedef struct{
    char* string;
    int x,y,width,height;
    MLV_Font *font;
    bool hidden;
}prompt;

typedef struct
{
    prompt** values;
    int length;
    int cur_index;
}prompt_queue;



typedef struct{
    int x,y,width,height,score;
    MLV_Font *font;
    char text[MAX_STR];
    MLV_Color color;
}score;

typedef struct {
    bool hovered,hide,drawn,focused;
    MLV_Font* font;
    int x,y,width,height,margeX,margeY,id;
    char* value,*placeholder;
    MLV_Color color,hovercolor;

} textbox;

typedef struct {
    textbox* values;
    int length;
} textbox_list;

typedef struct{
    unsigned width,height;
    button_list buttons;
    block_list blocks;
    score score;
    prompt_queue prompts;
    textbox_list texts_box;
    char* title;
    MLV_Font *title_font;

} screen;