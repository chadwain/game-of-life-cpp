#include "grid.h"

#include <SDL2/SDL.h>

#include <iostream>
#include <cassert>

constexpr int window_width = 1280;
constexpr int window_height = 720;

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
    SDL_Window* window = SDL_CreateWindow(
        "Game Of Life",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if (!window) return 1;

    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);
    int pixelFormat = SDL_GetWindowPixelFormat(window);

	GridPatch gp1{};
	gp1.setCell(40, 20, true);
    SDL_Surface* gridSurface = GridPatchToSurface(gp1, pixelFormat, 10, 2);
    if (!gridSurface) return 1;

    bool running = true;
    SDL_Event event{};
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_CLOSE) running = false;
                    break;
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }

		SDL_BlitSurface(gridSurface, nullptr, windowSurface, nullptr);
        SDL_UpdateWindowSurface(window);
    }

	SDL_FreeSurface(gridSurface);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
