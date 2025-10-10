#include "app.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* D�claration de la fonction screen_menu_create */
Screen* screen_menu_create(App *app, Screen *Sort);

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
    SortType *type_ptr = NULL;

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

    /* Ex�cute le tri selon le type */
    if (!strcmp(screen->title, "Tri par selection")){
        printf("Performing Selection Sort on %d elements\n", n);
        selection_sort(arr, n);
    }
    else{
        printf("Performing Merge Sort on %d elements\n", n);
        merge_sort(arr, n);
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
    const char *algo_name = (CURRENT_SORT_TYPE == SORT_SELECTION) ? "Tri par selection" : "Tri par fusion";
    Shape *title = shape_create(150, 10, 200, 60, algo_name, app->bg, app->font);
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

    /* Bouton Trier */
    Button *btn_sort = button_create(90, 200, 120, 50, "Trier", app->font);
    btn_sort->base.bg = (SDL_Color){155, 89, 182, 255}; // violet
    btn_sort->base.text_color = (SDL_Color){255,255,255,255};
    btn_sort->hoverColor = (SDL_Color){200, 180, 30, 255}; // orange
    btn_sort->pressedColor = (SDL_Color){150, 130, 0, 255}; // orange sombre
    btn_sort->base.userdata = screen; // passe l'écran en userdata
    btn_sort->on_click = on_sort;

    /* Bouton Retour */
    Button *btn_back = button_create(300, 200, 120, 50, "Retour", app->font);
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
    screen_add(screen, (Shape*)btn_sort);
    screen_add(screen, (Shape*)btn_back);

    return screen;
}
