#include "app.h"

/* === Crée l'application === */
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

    app_set_screen(app, screen_menu_create(app));
    return app;
}

/* === Change l’écran courant === */
void app_set_screen(App *app, Screen *screen) {
    if (app->screen)
        screen_destroy(app->screen);
    app->screen = screen;
}

/* === Boucle principale === */
int app_run(App *app, SDL_Event *e) {
    if (!app)
        return 1;

    while (SDL_PollEvent(&e))
        if (e->type == SDL_QUIT)
            return 0;
        else if (app->screen)
            screen_handle_event(app->screen, e);

    // Couleur d’arrière-plan
    SDL_SetRenderDrawColor(app->renderer, 240, 240, 240, 255);
    SDL_RenderClear(app->renderer);

    if (app->screen)
        screen_render(app->screen, app->renderer);
    SDL_RenderPresent(app->renderer);
    SDL_Delay(17); // ~60 FPS
    return 1;
}

/* === Détruit l'application === */
void app_destroy(App *app) {
    if (!app)
        return;

    if (app->screen)
        screen_destroy(app->screen);
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
