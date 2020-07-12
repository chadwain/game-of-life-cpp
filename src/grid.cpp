#include "grid.h"

#include <cassert>>

bool GridPatch::getCell(int8_t x, int8_t y) {
    assert(x < grid_patch_size && y < grid_patch_size);
    const auto index = static_cast<size_t>(x) + grid_patch_size * static_cast<size_t>(y);
    return this.cells[index];
}
