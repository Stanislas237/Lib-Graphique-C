#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "gui.h"
#include "shapes.h"

int main(int argc, char **argv) {
    GUI *gui = gui_create("SDL GUI Demo", 800, 600);
    if (!gui) return 1;

    /* load font */
    TTF_Font *font = TTF_OpenFont("qlarendonbold.otf", 16);
    if (!font) {
        fprintf(stderr, "TTF_OpenFont failed. Change path to a TTF on your system. Error: %s\n", TTF_GetError());
        gui_destroy(gui);
        return 1;
    }

    /* create a button */
    Button *btn = button_create(50, 50, 200, 50, "Clique-moi", font);
    /* we will set onclick below using normal function pointer */

    /* Because C doesn't support lambdas, do this: */
    void onbtn(Button *b, void *ud) {
        printf("Bouton cliqué !\n");
    }
    void onhover(Button *b, void *ud) {
        /* juste un message */
        /* printf("hover\n"); */
    }
    button_set_onclick(btn, onbtn);
    button_set_onhover(btn, onhover);

    /* create a TextView */
    TextView *tv = textview_create(50, 120, 300, 60, "Bonjour, ceci est une TextView", font);

    /* create an InputField */
    InputField *inp = inputfield_create(50, 200, 300, 40, "Tape ici", font);

    /* add to gui */
    gui_add(gui, (Shape*)btn);
    gui_add(gui, (Shape*)tv);
    gui_add(gui, (Shape*)inp);

    int running = 1;
    Uint32 last = SDL_GetTicks();
    while (running) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) running = 0;
            gui_handle_event(gui, &ev);
        }

        Uint32 now = SDL_GetTicks();
        float dt = (now - last) / 1000.0f;
        last = now;
        gui_update(gui, dt);
        gui_render(gui);
        SDL_Delay(10);
    }

    TTF_CloseFont(font);
    gui_destroy(gui);
    return 0;
}

