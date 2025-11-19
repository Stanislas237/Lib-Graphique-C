#include "app.h"

static void on_enqueue(Button *b, void *userdata) {
    App *app = (App*)userdata;
    Screen *screen = app->screen;
    Queue *q = app->main_queue;
    InputField *input = app->input;

    if (input->base.text[0] == '\0')
        return;

    int input_value = atoi(input->base.text);
    enqueue(q, input_value);
    queue_draw(app);

    input->base.text[0] = '\0';
    input->cursor = 0;
}

static void on_dequeue(Button *b, void *userdata) {
    App *app = (App*)userdata;
    Queue *q = app->main_queue;
    dequeue(q);
    queue_draw(app);
}

Screen* screen_file_create(App *app) {
    Screen *screen = screen_create();
    app->main_queue = queue_create(10);

    /* Texte du titre */
    Shape *title = shape_create(100, 30, 300, 50, "Gestion d'une file statique", app->bg, app->font);

    /* Champ de saisie */
    InputField *input = inputfield_create(100, 170, 50, 40, 3, "", app->font);
    input->base.bg = (SDL_Color){255,255,255,255};
    input->base.border = (SDL_Color){100,100,100,255};
    input->base.border_width = 1;
    input->base.text_color = (SDL_Color){0,0,0,255};
    input->color_focused = (SDL_Color){200, 200, 150, 255}; // lavande
    input->base.userdata = "input";

    /*Bouton enqueue */
    Button *enqueue = button_create(50, 220, 150, 40, "Enqueue", app->font);
    enqueue->base.bg = (SDL_Color){52, 219, 152, 255}; // vert
    enqueue->base.text_color = (SDL_Color){0,0,0,255};
    enqueue->hoverColor = (SDL_Color){200, 180, 30, 255}; // orange
    enqueue->pressedColor = (SDL_Color){150, 130, 0, 255}; // orange sombre
    enqueue->base.userdata = app;
    enqueue->on_click = on_enqueue;

    /*Bouton dequeue */
    Button *dequeue = button_create(250, 170, 150, 40, "Dequeue", app->font);
    dequeue->base.bg = (SDL_Color){252, 109, 92, 255}; // vert
    dequeue->base.text_color = (SDL_Color){255,255,255,255};
    dequeue->hoverColor = (SDL_Color){200, 180, 30, 255}; // orange
    dequeue->pressedColor = (SDL_Color){150, 130, 0, 255}; // orange sombre
    dequeue->base.userdata = app;
    dequeue->on_click = on_dequeue;

    screen_add(screen, title);
    screen_add(screen, (Shape*)input);
    screen_add(screen, (Shape*)enqueue);
    screen_add(screen, (Shape*)dequeue);

    app->input = input;
    return screen;
}

