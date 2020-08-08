#include "grid.h"
#include "render.h"

#include <SDL2/SDL.h>

#include <iostream>
#include <cassert>

constexpr int window_width = 1280;
constexpr int window_height = 720;
constexpr int movement_speed = 10;

struct ViewState {
    View view;
    GridRange range;
    SDL_Surface* emptySurface;

    static ViewState create(const View& view, uint32_t pixelFormat) noexcept {
        ViewState result;
        result.view = view;
        result.range = view.getGridRange();
        result.emptySurface = emptyPatchSurface(view, pixelFormat);
        return result;
    }

    ViewState move(int32_t x, int32_t y) const noexcept {
        ViewState result;
        View newView = this->view;
        newView.x = x;
        newView.y = y;
        result.view = newView;
        result.range = newView.getGridRange();
        result.emptySurface = this->emptySurface;
        return result;
    }
};

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
    const uint32_t pixelFormat = SDL_GetWindowPixelFormat(window);

    GridPatch gp1{};
    GridPatchEdges gp1Edge{};
    GridId gp1Id{};
    gp1Id.x = 0;
    gp1Id.y = 0;

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

    GridPatch gp2{};
    GridPatchEdges gp2Edge{};
    GridId gp2Id{1, 0};
    // Glider
    gp2.setCell(7, 10, true);
    gp2.setCell(7, 11, true);
    gp2.setCell(7, 12, true);
    gp2.setCell(6, 12, true);
    gp2.setCell(5, 11, true);

    Grid grid{};
    grid.patches[gp1Id] = gp1;
    grid.patchEdges[gp1Id] = gp1Edge;
    grid.patches[gp2Id] = gp2;
    grid.patchEdges[gp2Id] = gp2Edge;

    SDL_Surface* gridSurface = SDL_CreateRGBSurfaceWithFormat(0, window_width, window_height, 32, pixelFormat);
    if (!gridSurface) return 1;

    View view{};
    view.x = 1300;
    view.y = 0;
    view.w = 1500;
    view.h = 1500;
    view.square_length = 20;
    view.border_size = 2;
    ViewState vs = ViewState::create(view, pixelFormat);
    renderGridRange(gridSurface, vs.view, grid, vs.range, vs.emptySurface);

    bool running = true;
    SDL_Event event{};
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_UP) {
                        vs = vs.move(vs.view.x, vs.view.y - movement_speed);
                    } else if (event.key.keysym.sym == SDLK_DOWN) {
                        vs = vs.move(vs.view.x, vs.view.y + movement_speed);
                    } else if (event.key.keysym.sym == SDLK_LEFT) {
                        vs = vs.move(vs.view.x + movement_speed, vs.view.y);
                    } else if (event.key.keysym.sym == SDLK_RIGHT) {
                        vs = vs.move(vs.view.x - movement_speed, vs.view.y);
                    } else if (event.key.keysym.sym == SDLK_RETURN) {
                        updateGrid(grid);
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

        renderGridRange(gridSurface, vs.view, grid, vs.range, vs.emptySurface);
        SDL_BlitSurface(gridSurface, nullptr, windowSurface, nullptr);
        SDL_UpdateWindowSurface(window);
    }

    SDL_FreeSurface(vs.emptySurface);
    SDL_FreeSurface(gridSurface);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

