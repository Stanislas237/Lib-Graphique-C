#ifndef SHAPES_H
#define SHAPES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


static void color_set(SDL_Renderer *r, SDL_Color c) {
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
}

typedef struct Shape Shape;
typedef struct Button Button;
typedef struct InputField InputField;

static void filledCircleRGBA(SDL_Renderer *renderer, int x0, int y0, int radius);
static void fill_rounded_rect(SDL_Renderer *renderer, SDL_Rect r, int radius);
static int point_in_shape(Shape *s, int px, int py);
static void draw_rect_border(SDL_Renderer *renderer, SDL_Rect r, int bw);
SDL_Texture *render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color text_color, SDL_Rect *out_rect);

/* --- Base --- */
struct Shape {
    int x, y, w, h, radius;
    int visible;
    SDL_Color bg;
    SDL_Color border;
    SDL_Color text_color;
    int border_width;
    char *text;
    TTF_Font *font;
    void *userdata;

    void (*draw)(SDL_Renderer*, Shape*);
    void (*handle_event)(Shape*, SDL_Event*);
    void (*destroy)(Shape*);
    void (*reset)(Shape*);
};

Shape *shape_create(int x,int y,int w,int h, const char *text, SDL_Color color, TTF_Font *font);
static void draw_shape_text(SDL_Renderer *renderer, Shape *s);
static void render_shape(SDL_Renderer *renderer, Shape *s, SDL_Color bg);
static void shape_draw(SDL_Renderer *renderer, Shape *s);
static void shape_destroy(Shape *s);

/* --- Bouton --- */
typedef void (*ButtonCallback)(Button*, void*);

struct Button {
    Shape base;
    int hovered;
    int pressed;
    SDL_Color hoverColor;
    SDL_Color pressedColor;
    ButtonCallback on_click;
    ButtonCallback on_hover;
};

Button* button_create(int x, int y, int w, int h, const char *label, TTF_Font *font);
static void button_draw(SDL_Renderer *renderer, Shape *s);
static void button_reset(Shape* s);
static void button_handle_event(Shape *s, SDL_Event *ev);

/* --- Champ de saisie --- */
struct InputField {
    Shape base;
    int cursor;
    int length;
    int focused;
    SDL_Color color_focused;
};

InputField *inputfield_create(int x,int y,int w,int h,int length,const char *initial, TTF_Font *font);
static void input_draw(SDL_Renderer *renderer, Shape *s_base);
static void input_handle_event(Shape *s_base, SDL_Event *ev);
#endif
