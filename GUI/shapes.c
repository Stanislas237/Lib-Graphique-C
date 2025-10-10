#include "app.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/* helpers MAX/MIN */
#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

/* filled circle drawing (na�f) -- used for rounded corners */
static void filledCircleRGBA(SDL_Renderer *renderer, int x0, int y0, int radius) {
    for (int y = -radius; y <= radius; y++) {
        int dx = (int)floor(sqrt(radius * radius - y * y));  // Calcul du rayon pour chaque ligne
        for (int x = -dx; x <= dx; x++) {
            SDL_RenderDrawPoint(renderer, x0 + x, y0 + y);
        }
    }
}

/* filled rounded rect */
static void fill_rounded_rect(SDL_Renderer *renderer, SDL_Rect r, int radius) {
    if (radius <= 0) {
        SDL_RenderFillRect(renderer, &r);
        return;
    }
    /* central rectangle */
    SDL_Rect center = {r.x + radius, r.y, r.w - 2*radius, r.h};
    SDL_RenderFillRect(renderer, &center);
    SDL_Rect left = {r.x, r.y + radius, radius, r.h - 2*radius};
    SDL_RenderFillRect(renderer, &left);
    SDL_Rect right = {r.x + r.w - radius, r.y + radius, radius, r.h - 2*radius};
    SDL_RenderFillRect(renderer, &right);
    /* corners */
    filledCircleRGBA(renderer, r.x + radius, r.y + radius, radius);
    filledCircleRGBA(renderer, r.x + r.w - radius - 1, r.y + radius, radius);
    filledCircleRGBA(renderer, r.x + radius, r.y + r.h - radius - 1, radius);
    filledCircleRGBA(renderer, r.x + r.w - radius - 1, r.y + r.h - radius - 1, radius);
}

/* Draw border (approx) */
static void draw_rect_border(SDL_Renderer *renderer, SDL_Rect r, int bw) {
    if (bw <= 0) return;
    for (int i=0;i<bw;i++) {
        SDL_Rect rr = { r.x + i, r.y + i, r.w - 2*i, r.h - 2*i };
        /* draw rectangle outline */
        SDL_RenderDrawRect(renderer, &rr);
    }
}

/* center text inside shape */
static void draw_shape_text(SDL_Renderer *renderer, Shape *s) {
    if (!s->text || !s->font) return;
    SDL_Rect rect;
    SDL_Texture *tex = render_text(renderer, s->font, s->text, &rect);
    if (!tex) return;
    int tx = s->x + (s->w - rect.w)/2;
    int ty = s->y + (s->h - rect.h)/2;
    SDL_Rect dest = {tx, ty, rect.w, rect.h};
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, tex, NULL, &dest);
    SDL_DestroyTexture(tex);
}

static void shape_draw(SDL_Renderer *renderer, Shape *s) {
    if (!s || !s->visible) return;
    SDL_Rect r = {s->x, s->y, s->w, s->h};
    color_set(renderer, s->bg);
    fill_rounded_rect(renderer, r, s->radius);
    color_set(renderer, s->border);
    draw_rect_border(renderer, r, s->border_width);
    draw_shape_text(renderer, s);
}

static void shape_destroy(Shape *s) {
    if (!s) return;
    if (s->text)
        free(s->text);
    free(s);
}


/* render text to texture and output rect with width/height */
SDL_Texture *render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect *out_rect) {
    if (!text || !font) return NULL;
    SDL_Surface *surf = TTF_RenderUTF8_Blended(font, text, (SDL_Color){0,0,0,255});
    if (!surf) return NULL;
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
    if (out_rect) {
        out_rect->w = surf->w;
        out_rect->h = surf->h;
    }
    SDL_FreeSurface(surf);
    return tex;
}

/* --- Shape base --- */
Shape *shape_create(int x,int y,int w,int h, const char *text, SDL_Color bg_color, TTF_Font *font) {
    Shape *s = malloc(sizeof(Shape));
    s->visible = 1;
    s->x = x; s->y = y; s->w = w; s->h = h;
    s->radius = 0;
    s->bg = bg_color;
    s->border = (SDL_Color){0,0,0,255};
    s->border_width = 0;
    if (text) s->text = strdup(text);
    s->font = font;
    s->text_color = (SDL_Color){0,0,0,255};
    s->draw = shape_draw;
    s->destroy = shape_destroy;
    s->handle_event = NULL;
    return s;
}

/* --- Button --- */
static void button_draw(SDL_Renderer *renderer, Shape *s_base) {
    Button *b = (Button*)s_base;
    Shape *s = &b->base;
    SDL_Rect r = {s->x, s->y, s->w, s->h};

    SDL_Color bg = s->bg;
    if (b->hovered)
        bg = b->hoverColor;
    if (b->pressed)
        bg = b->pressedColor;

    color_set(renderer, bg);
    fill_rounded_rect(renderer, r, s->radius);
    color_set(renderer, s->border);
    draw_rect_border(renderer, r, s->border_width);
    draw_shape_text(renderer, s);
}

static void button_reset(Shape* s_base){
    Button *b = (Button*)s_base;
    b->pressed = 0;
    b->hovered = 0;
}

static int point_in_shape(Shape *s, int px, int py) {
    return (px >= s->x && px <= s->x + s->w && py >= s->y && py <= s->y + s->h);
}

static void button_handle_event(Shape *s_base, SDL_Event *ev) {
    Button *b = (Button*)s_base;
    Shape *s = &b->base;
    if (ev->type == SDL_MOUSEMOTION) {
        int mx = ev->motion.x, my = ev->motion.y;
        b->hovered = point_in_shape(s, mx, my);
    } else if (ev->type == SDL_MOUSEBUTTONDOWN) {
        int mx = ev->button.x, my = ev->button.y;
        if (ev->button.button == SDL_BUTTON_LEFT && point_in_shape(s, mx, my))
            b->pressed = 1;
    } else if (ev->type == SDL_MOUSEBUTTONUP) {
        int mx = ev->button.x, my = ev->button.y;
        if (ev->button.button == SDL_BUTTON_LEFT) {
            if (b->pressed && point_in_shape(s, mx, my))
                if (b->on_click)
                    b->on_click(b, b->base.userdata);
                }
            b->pressed = 0;
    }
}

Button *button_create(int x,int y,int w,int h, const char *text, TTF_Font *font) {
    Button *b = malloc(sizeof(Button));
    Shape *s = &b->base;
    s->visible = 1;
    s->x=x; s->y=y; s->w=w; s->h=h;
    s->radius = 10;
    s->bg = (SDL_Color){70,130,180,255}; /* steelblue */
    s->border = (SDL_Color){50,50,50,255};
    s->border_width = 0;
    if (text) s->text = strdup(text);
    s->font = font;
    s->text_color = (SDL_Color){255,255,255,255};
    s->draw = button_draw;
    s->handle_event = button_handle_event;
    s->reset = button_reset;
    s->destroy = shape_destroy;
    s->userdata = NULL;
    b->hovered = 0;
    b->pressed = 0;
    b->on_click = NULL;
    return b;
}

/* --- InputField --- */

static void input_draw(SDL_Renderer *renderer, Shape *s_base) {
    InputField *in = (InputField*)s_base;
    Shape *s = &in->base;
    SDL_Rect r = {s->x, s->y, s->w, s->h};
    color_set(renderer, in->focused ? in->color_focused : s->bg);
    fill_rounded_rect(renderer, r, s->radius);
    color_set(renderer, s->border);
    draw_rect_border(renderer, r, s->border_width);

    /* render text left-aligned with padding */
    if (in->buffer && s->font) {
        SDL_Surface *surf = TTF_RenderUTF8_Blended(s->font, in->buffer, (SDL_Color){s->text_color.r, s->text_color.g, s->text_color.b, s->text_color.a});
        if (surf) {
            SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
            SDL_Rect dest = {s->x + 6, s->y + (s->h - surf->h)/2, surf->w, surf->h};
            SDL_RenderCopy(renderer, tex, NULL, &dest);
            SDL_DestroyTexture(tex);
            SDL_FreeSurface(surf);
        }
    }
}

static void input_handle_event(Shape *s_base, SDL_Event *ev) {
    InputField *in = (InputField*)s_base;
    Shape *s = &in->base;
    if (ev->type == SDL_MOUSEBUTTONDOWN) {
        int mx = ev->button.x, my = ev->button.y;
        if (point_in_shape(s, mx, my)) {
            if (in->focused == 0){
                if (INPUT_ACTIVE_COUNT == 0)
                    SDL_StartTextInput();
                INPUT_ACTIVE_COUNT++;
            }
            in->focused = 1;
        } else {
            if (in->focused == 1){
                INPUT_ACTIVE_COUNT--;
                if (INPUT_ACTIVE_COUNT == 0)
                    SDL_StopTextInput();
            }
            in->focused = 0;
        }
    } else if (ev->type == SDL_TEXTINPUT && in->focused) {
        if (in->cursor < in->length){
            in->buffer[in->cursor++] = ev->text.text[0];
            in->buffer[in->cursor] = '\0';
        }
    } else if (ev->type == SDL_KEYDOWN && in->focused) {
        if (ev->key.keysym.sym == SDLK_BACKSPACE) {
            if (in->cursor > 0) {
                in->buffer[--in->cursor] = '\0';
            }
        }
    }
}

InputField *inputfield_create(int x,int y,int w,int h,int length,const char *initial, TTF_Font *font) {
    InputField *in = malloc(sizeof(InputField));
    Shape *s = &in->base;
    s->visible = 1;
    s->x=x; s->y=y; s->w=w; s->h=h;
    s->radius = 4;
    s->bg = (SDL_Color){255,255,255,255};
    s->border = (SDL_Color){120,120,120,255};
    s->border_width = 1;
    s->font = font;
    s->text_color = (SDL_Color){0,0,0,255};
    s->draw = input_draw;
    s->handle_event = input_handle_event;
    s->destroy = shape_destroy;

    in->length = length;
    in->buffer = malloc(length * sizeof(char));
    if (initial) {
        strncpy(in->buffer, initial, length-1);
        in->cursor = strlen(in->buffer);
    } else in->cursor = 0;
    in->focused = 0;
    return in;
}
