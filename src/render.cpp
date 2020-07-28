#include "render.h"

#include <cassert>

void View::setPositions(int32_t x, int32_t y) noexcept {
    assert(x >= pos_min && x <= pos_max);
    assert(y >= pos_min && y <= pos_max);
    this->x = x;
    this->y = y;
}

void View::setDimensions(uint32_t w, uint32_t h) noexcept {
    assert(w >= dim_min && w <= dim_max);
    assert(h >= dim_min && h <= dim_max);
    this->w = w;
    this->h = h;
}

GridRange View::getGridRange() const noexcept {
    const int32_t patchTotalLength = grid_patch_size * (static_cast<int32_t>(this->square_length) + static_cast<int32_t>(this->border_size));

    const int32_t x1 = this->x;
    const int32_t x2 = x1 + this->w;
    const int32_t y1 = this->y;
    const int32_t y2 = y1 + this->h;
    
    GridRange result;
    result.xBegin = (x1 / patchTotalLength) - static_cast<int32_t>(x1 % patchTotalLength < 0);
    result.xEnd = (x2 / patchTotalLength) + static_cast<int32_t>(x2 % patchTotalLength > 0);
    result.yBegin = (y1 / patchTotalLength) - static_cast<int32_t>(y1 % patchTotalLength < 0);
    result.yEnd = (y2 / patchTotalLength) + static_cast<int32_t>(y2 % patchTotalLength > 0);
    return result;
}

SDL_Surface* gridPatchToSurface(const GridPatch& patch, int format, uint8_t square_length, uint8_t border_size) noexcept {
    const int boxSize = (static_cast<int>(square_length) + static_cast<int>(border_size));
    const int surfaceSize = boxSize * grid_patch_size;
    SDL_Surface* result = SDL_CreateRGBSurfaceWithFormat(0, surfaceSize, surfaceSize, 32, format);
    if (!result) return nullptr;

    int y = 0;
    for (int j = 0; j < grid_patch_size; ++j) {
        int x = 0;
        for (int i = 0; i < grid_patch_size; ++i) {
            SDL_Rect rect;
            rect.x = x;
            rect.y = y;
            rect.w = square_length;
            rect.h = square_length;
            if (patch.getCell(i, j))
                SDL_FillRect(result, &rect, 0x000000);
            else
                SDL_FillRect(result, &rect, 0xffffff);
            
            x += boxSize;
        }
        y += boxSize;
    }

    return result;
}

//SDL_Surface* renderGridRange(const View& view, const Grid& grid, const GridRange& range) noexcept {
//    for (auto i = range.xBegin; i < range.xEnd; ++i) {
//        for (auto j = range.yBegin; j < range.yEnd; ++j) {
//            GridId id;
//            id.x = i;
//            id.y = j;
//            const auto patch = grid.patches.maybeGet(id);
//            if (!patch) continue;
//
//            SDL_Rect dest;
//            dest.x = ((id.x) * patchTotalLength - view.x)i * ...;
//        }
//    }
//}
//
