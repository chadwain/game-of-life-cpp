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

void gridPatchToSurface(const GridPatch& patch, SDL_Surface* surface, uint8_t square_length, uint8_t border_size) noexcept {
    const int boxSize = static_cast<int>(square_length) + static_cast<int>(border_size);

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
                SDL_FillRect(surface, &rect, 0x000000);
            else
                SDL_FillRect(surface, &rect, 0xffffff);
            
            x += boxSize;
        }
        y += boxSize;
    }
}

void renderGridRange(SDL_Surface* surface, const View& view, const Grid& grid, const GridRange& range) noexcept {
    const int32_t patchTotalLength = grid_patch_size * (static_cast<int32_t>(view.square_length) + static_cast<int32_t>(view.border_size));
    assert(patchTotalLength <= 16384); // 2^14

    const int32_t boxScaledWidth = patchTotalLength * patchTotalLength / view.w;
    const int32_t boxScaledHeight = patchTotalLength * patchTotalLength / view.h;

    const int32_t imageWidth = boxScaledWidth * (range.xEnd - range.xBegin);
    const int32_t imageHeight = boxScaledHeight * (range.yEnd - range.yBegin);

    const int32_t surfaceDestHeight = imageHeight * surface->w / imageWidth;

    SDL_Surface* patchSurface = SDL_CreateRGBSurfaceWithFormat(0, patchTotalLength, patchTotalLength, 32, surface->format->format);

    for (auto i = range.xBegin; i < range.xEnd; ++i) {
        for (auto j = range.yBegin; j < range.yEnd; ++j) {
            GridId id;
            id.x = i;
            id.y = j;
            const auto patchOptional = grid.patches.find(id);
            if (patchOptional == grid.patches.end()) continue;
            const GridPatch& patch = patchOptional->second;
            gridPatchToSurface(patch, patchSurface, view.square_length, view.border_size);

            SDL_Rect dest;
            dest.x = ((id.x * patchTotalLength - view.x) * patchTotalLength / view.w) * surface->w / imageWidth;
            dest.y = ((id.y * patchTotalLength - view.y) * patchTotalLength / view.h) * surface->w / imageWidth;
            dest.w = boxScaledWidth * surface->w / imageWidth;
            dest.h = boxScaledHeight * surface->w / imageWidth;
            SDL_BlitScaled(patchSurface, nullptr, surface, &dest);
        }
    }

    SDL_FreeSurface(patchSurface);
}

