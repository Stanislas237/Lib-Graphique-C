#include "app.h"

Screen* screen_file_create(App *app);

/* === Cr�e l'application === */
App* app_create(const char *title, int w, int h) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init: %s\n", SDL_GetError());
        return NULL;
    }
    if (TTF_Init() != 0) {
        printf("Erreur TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return NULL;
    }

    App *app = malloc(sizeof(App));
    app->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
    if (!app->window) {
        printf("Erreur SDL_CreateWindow: %s\n", SDL_GetError());
        app_destroy(app);
        return NULL;
    }

    app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!app->renderer) {
        printf("Erreur SDL_CreateRenderer: %s\n", SDL_GetError());
        app_destroy(app);
        return NULL;
    }

    app->font = TTF_OpenFont("qlarendonbold.otf", 18);
    if (!app->font) {
        printf("Erreur chargement police: %s\n", TTF_GetError());
        app_destroy(app);
        return NULL;
    }

    app->bg = (SDL_Color){240, 240, 240, 255};

    Screen *mainscreen = screen_file_create(app);
    app->screen = mainscreen;
    return app;
}

/* === Boucle principale === */
int app_run(App *app) {
    if (!app)
        return 1;

    SDL_Event ev;
    while (SDL_PollEvent(&ev)){
        switch (ev.type){
            case SDL_QUIT:
                return 1;
            default:
                if (app->screen)
                    screen_handle_event(app->screen, &ev);
                break;
        }
    }

    // Couleur d�arri�re-plan
    color_set(app->renderer, app->bg);
    SDL_RenderClear(app->renderer);

    if (app->screen)
        screen_render(app->screen, app->renderer);
    SDL_RenderPresent(app->renderer);
    SDL_Delay(17); // ~60 FPS
    return 0;
}

/* === D�truit l'application === */
void app_destroy(App *app) {
    if (!app)
        return;

    if (app->screen)
        screen_destroy(app->screen);
    if (app->main_queue)
        queue_destroy(app->main_queue);
    if (app->font)
        TTF_CloseFont(app->font);
    if (app->renderer)
        SDL_DestroyRenderer(app->renderer);
    if (app->window)
        SDL_DestroyWindow(app->window);
    TTF_Quit();
    SDL_Quit();
    free(app);
}
