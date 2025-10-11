#include "app.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* === Callback : bouton Retour === */
static void on_back(Button *b, void *userdata) {
    App *app = (App*)userdata;
    app_set_screen(app, app->screen->next);
}

/* === Callback : bouton Trier === */
static void on_sort(Button *b, void *userdata) {
    Screen *screen = (Screen*)userdata;

    InputField *input = NULL;
    Shape *output = NULL;

    /* R�cup�re les pointeurs utiles stock�s dans userdata des formes */
    for (int i = 0; i < screen->count; i++) {
        Shape *s = screen->elements[i];
        if (!s) continue;
        if (s->userdata && strcmp((char*)s->userdata, "input") == 0)
            input = (InputField*)s;
        else if (s->userdata && strcmp((char*)s->userdata, "output") == 0)
            output = s;
    }

    if (!input || !output) return;

    /* Parse les nombres */
    int arr[128];
    int n = 0;
    char temp[256];
    strcpy(temp, input->buffer);
    char *tok = strtok(temp, " ,-;+");
    while (tok && n < 128) {
        arr[n++] = atoi(tok);
        tok = strtok(NULL, " ,;+");
    }

    /* Exécute le tri selon le type */
    int type_tri = strcmp(screen->title, "Tri par selection");
    int forme_tri = strcmp(b->base.text, "Tri récursif");
    
    if (type_tri == 0){ // Tri par sélection
        if (forme_tri == 0){ // Tri récursif
            printf("Tri par selection recursif sur %d elements\n", n);
            selection_sort_rec(arr, n);
        } else { // Tri itératif
            printf("Tri par selection iteratif sur %d elements\n", n);
            selection_sort_iter(arr, n);
        }
    } else { // Tri par fusion
        if (forme_tri == 0) { // Tri récursif
            printf("Tri par fusion recursif sur %d elements\n", n);
            
            if (n <= 1) return;
            int *tmp = malloc(n * sizeof(int));
            if (!tmp) return;
            merge_sort_rec(arr, 0, n - 1, tmp);
            free(tmp);
        } else { // Tri itératif
            printf("Tri par fusion iteratif sur %d elements\n", n);
            merge_sort_iter(arr, n);
        }
    }

    /* Construit la chaîne de sortie */
    char buf[512] = "";
    for (int i = 0; i < n; i++) {
        char tmp[16];
        sprintf(tmp, "%d  ", arr[i]);
        strcat(buf, tmp);
    }

    free(output->text);
    output->text = strdup(buf);
}

void set_text(Shape *s, const char *text){
    if (!s) return;
    free(s->text);
    s->text = strdup(text);
}

static void set_title_text(Shape *s){
    if (!s) return;
    Screen *screen = (Screen*)s->userdata;
    set_text(s, screen->title);
}

static void set_output_empty(Shape *s){
    if (!s) return;
    set_text(s, "");
}

/* === Création de l'écran du tri === */
Screen* screen_sort_create(App *app, Screen *Menu) {
    Screen *screen = screen_create(Menu);

    /* Titre */
    Shape *title = shape_create(150, 10, 200, 60, screen->title, app->bg, app->font);
    title->userdata = screen;
    title->reset = set_title_text;

    /* Champ de saisie */
    InputField *input = inputfield_create(50, 75, 400, 40, 30, "5,8,300,-4,15", app->font);
    input->base.bg = (SDL_Color){255,255,255,255};
    input->base.border = (SDL_Color){100,100,100,255};
    input->base.border_width = 1;
    input->base.text_color = (SDL_Color){0,0,0,255};
    input->color_focused = (SDL_Color){200, 200, 150, 255}; // lavande
    input->base.userdata = "input";

    /* Zone de texte pour le résultat */
    Shape *output = shape_create(50, 130, 400, 40, "", (SDL_Color){200,200,200,255}, app->font);
    output->border = (SDL_Color){100,100,100,255};
    output->border_width = 1;
    output->userdata = "output";
    output->reset = set_output_empty;

    /* Bouton Tri itératif */
    Button *btn_sort_iterative = button_create(50, 200, 100, 50, "Tri itératif", app->font);
    btn_sort_iterative->base.bg = (SDL_Color){155, 89, 182, 255}; // violet
    btn_sort_iterative->base.text_color = (SDL_Color){255,255,255,255};
    btn_sort_iterative->hoverColor = (SDL_Color){200, 180, 30, 255}; // orange
    btn_sort_iterative->pressedColor = (SDL_Color){150, 130, 0, 255}; // orange sombre
    btn_sort_iterative->base.userdata = screen; // passe l'écran en userdata
    btn_sort_iterative->on_click = on_sort;

    /* Bouton Tri récursif */
    Button *btn_sort_recursive = button_create(190, 200, 120, 50, "Tri récursif", app->font);
    btn_sort_recursive->base.bg = (SDL_Color){155, 89, 182, 255}; // violet
    btn_sort_recursive->base.text_color = (SDL_Color){255,255,255,255};
    btn_sort_recursive->hoverColor = (SDL_Color){200, 180, 30, 255}; // orange
    btn_sort_recursive->pressedColor = (SDL_Color){150, 130, 0, 255}; // orange sombre
    btn_sort_recursive->base.userdata = screen; // passe l'écran en userdata
    btn_sort_recursive->on_click = on_sort;

    /* Bouton Retour */
    Button *btn_back = button_create(350, 200, 100, 50, "Retour", app->font);
    btn_back->base.bg = (SDL_Color){231, 76, 60, 255}; // rouge
    btn_back->base.text_color = (SDL_Color){255,255,255,255};
    btn_back->hoverColor = (SDL_Color){200, 180, 30, 255}; // orange
    btn_back->pressedColor = (SDL_Color){150, 130, 0, 255}; // orange sombre
    btn_back->base.userdata = app;
    btn_back->on_click = on_back;

    /* Ajout des éléments */
    screen_add(screen, title);
    screen_add(screen, (Shape*)input);
    screen_add(screen, output);
    screen_add(screen, (Shape*)btn_sort_iterative);
    screen_add(screen, (Shape*)btn_sort_recursive);
    screen_add(screen, (Shape*)btn_back);

    return screen;
}
