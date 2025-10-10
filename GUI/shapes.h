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
};

Shape *shape_create(int x,int y,int w,int h, const char *text, SDL_Color color, TTF_Font *font);

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

SDL_Texture *render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect *out_rect);

/* --- Champ de saisie --- */
struct InputField {
    Shape base;
    char *buffer;
    int cursor;
    int length;
    int focused;
    SDL_Color color_focused;
};

InputField *inputfield_create(int x,int y,int w,int h,int length,const char *initial, TTF_Font *font);
#endif
