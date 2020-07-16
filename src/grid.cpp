#include "grid.h"

#include <cassert>

void GridPatch::setCell(int8_t x, int8_t y, bool value) {
    assert(x < grid_patch_size && y < grid_patch_size);
    const auto index = static_cast<size_t>(x) + grid_patch_size * static_cast<size_t>(y);
    this->cells[index] = value;
}

bool GridPatch::getCell(int8_t x, int8_t y) const {
    assert(x < grid_patch_size && y < grid_patch_size);
    const auto index = static_cast<size_t>(x) + grid_patch_size * static_cast<size_t>(y);
    return this->cells[index];
}

SDL_Surface* GridPatchToSurface(const GridPatch& patch, int format, uint8_t square_length, uint8_t border_size) {
    const int boxSize = (static_cast<int>(square_length) + static_cast<int>(border_size));
    const int surfaceSize = boxSize * grid_patch_size;
	SDL_Surface* result = SDL_CreateRGBSurfaceWithFormat(0, surfaceSize, surfaceSize, 32, format);
	if (!result) return nullptr;

	int y = 0;
	for  (int i = 0; i < grid_patch_size; ++i) {
    	int x = 0;
    	for (int j = 0; j < grid_patch_size; ++j) {
        	SDL_Rect rect;
        	rect.x = x;
        	rect.y = y;
        	rect.w = square_length;
        	rect.h = square_length;
        	if (patch.getCell(j, i))
	        	SDL_FillRect(result, &rect, 0x000000);
        	else
	        	SDL_FillRect(result, &rect, 0xffffff);
        	
        	x += boxSize;
        }
        y += boxSize;
    }

    return result;
}

