#ifndef SHAPES_H
#define SHAPES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/* Basic RGBA color */
typedef struct Color {
    Uint8 r,g,b,a;
} Color;

/* Forward declarations */
typedef struct Shape Shape;
typedef struct Button Button;
typedef struct TextView TextView;
typedef struct InputField InputField;

/* Base "class" */
struct Shape {
    int x,y,w,h;
    int radius; /* coin arrondi optionnel (pixels) */

    Color bg;
    Color border;
    int border_width;

    char *text;
    TTF_Font *font;
    Color text_color;

    /* "virtual" methods */
    void (*draw)(SDL_Renderer *renderer, Shape *s);
    void (*handle_event)(Shape *s, SDL_Event *ev);
    void (*destroy)(Shape *s);
    void (*update)(Shape *s, float dt);

    /* user data */
    void *userdata;
    int visible;
};

/* Create/destroy base shape */
Shape *shape_create(int x,int y,int w,int h);
void shape_destroy(Shape *s);

/* Utilities */
SDL_Texture *render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect *out_rect);

/* Button */
typedef void (*button_cb)(Button *b, void *userdata);
struct Button {
    Shape base;
    int hovered;
    int pressed;
    button_cb on_click;
    button_cb on_hover;
};

Button *button_create(int x,int y,int w,int h, const char *text, TTF_Font *font);
void button_set_onclick(Button *b, button_cb cb);
void button_set_onhover(Button *b, button_cb cb);

/* TextView */
struct TextView {
    Shape base;
    /* could have alignment etc */
};

TextView *textview_create(int x,int y,int w,int h, const char *text, TTF_Font *font);

/* InputField */
struct InputField {
    Shape base;
    char *buffer;
    int bufcap;
    int cursor; /* position in chars */
    int focused;
};

InputField *inputfield_create(int x,int y,int w,int h, const char *initial, TTF_Font *font);

#endif

