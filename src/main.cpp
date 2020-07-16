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
	GridPatchEdges gp1Edge{};

	// Oscillator
	gp1.setCell(40, 20, true);
	gp1.setCell(40, 19, true);
	gp1.setCell(40, 21, true);

	// Beacon
	gp1.setCell(25, 40, true);
	gp1.setCell(26, 40, true);
	gp1.setCell(25, 41, true);
	gp1.setCell(27, 43, true);
	gp1.setCell(28, 42, true);
	gp1.setCell(28, 43, true);

	// Glider
	gp1.setCell(7, 10, true);
	gp1.setCell(7, 11, true);
	gp1.setCell(7, 12, true);
	gp1.setCell(6, 12, true);
	gp1.setCell(5, 11, true);

    SDL_Surface* gridSurface = GridPatchToSurface(gp1, pixelFormat, 10, 2);
    if (!gridSurface) return 1;

    bool running = true;
    SDL_Event event{};
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        gp1 = updateGridPatch(gp1, gp1Edge);
                        SDL_FreeSurface(gridSurface);
                        gridSurface = GridPatchToSurface(gp1, pixelFormat, 10, 2);
                        if (!gridSurface) return 1;
                    }
                    break;
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
