#ifndef GOC_RENDER_H
#define GOC_RENDER_H

#include "grid.h"

#include <SDL2/SDL.h>

struct GridRange {
    int8_t xBegin, xEnd, yBegin, yEnd;
};

struct View {
    static constexpr int32_t pos_min = -1073741824; // -2^30
    static constexpr int32_t pos_max = 1073741823; // 2^30 - 1
    static constexpr uint32_t dim_min = 0;
    static constexpr uint32_t dim_max = 1073741823; // 2^30 - 1
    
    int32_t x, y;
    uint32_t w, h;
    uint8_t square_length;
    uint8_t border_size;

    void setPositions(int32_t x, int32_t y) noexcept;
    void setDimensions(uint32_t w, uint32_t h) noexcept;
    GridRange getGridRange() const noexcept;
};

SDL_Surface* gridPatchToSurface(const GridPatch& patch, int format, uint8_t square_length, uint8_t border_size) noexcept;
//SDL_Surface* renderGridRange(const View& view, const Grid& grid, const GridRange& range) noexcept;

#endif
