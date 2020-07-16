#include "grid.h"

#include <cassert>

void GridPatch::setCell(uint8_t x, uint8_t y, bool value) {
    assert(x < grid_patch_size && y < grid_patch_size);
    const auto index = static_cast<size_t>(x) + grid_patch_size * static_cast<size_t>(y);
    this->cells[index] = value;
}

bool GridPatch::getCell(uint8_t x, uint8_t y) const {
    assert(x < grid_patch_size && y < grid_patch_size);
    const auto index = static_cast<size_t>(x) + grid_patch_size * static_cast<size_t>(y);
    return this->cells[index];
}

void updateGridPatchCell(GridPatch& dest, const GridPatch& src, int numNeighbors, uint8_t x, uint8_t y) {
    const bool newValue = (numNeighbors == 3 || (src.getCell(x, y) && numNeighbors == 2));
    dest.setCell(x, y, newValue);
}

GridPatch updateGridPatch(const GridPatch& patch, const GridPatchEdges& edges) {
    GridPatch result{};

    // The inner square of side length (grid_patch_size - 2)
    for (uint8_t y = 1; y < grid_patch_size - 1; ++y) {
	    for (uint8_t x = 1; x < grid_patch_size - 1; ++x) {
    	    const int numNeighbors = 
    	    	static_cast<int>(patch.getCell(x - 1, y - 1)) +
    	    	static_cast<int>(patch.getCell(x    , y - 1)) +
    	    	static_cast<int>(patch.getCell(x + 1, y - 1)) +
    	    	static_cast<int>(patch.getCell(x - 1, y)) +
    	    	static_cast<int>(patch.getCell(x + 1, y)) +
    	    	static_cast<int>(patch.getCell(x - 1, y + 1)) +
    	    	static_cast<int>(patch.getCell(x    , y + 1)) +
    	    	static_cast<int>(patch.getCell(x + 1, y + 1));
    	    updateGridPatchCell(result, patch, numNeighbors, x, y);
	    }
    }

	// The top edge
    for (uint8_t i = 1; i < grid_patch_size - 1; ++i) {
		const int numNeighbors = 
    	  	static_cast<int>(edges.top[i - 1]) +
    	  	static_cast<int>(edges.top[i]) +
    	  	static_cast<int>(edges.top[i + 1]) +
    	  	static_cast<int>(patch.getCell(i - 1, 0)) +
    	  	static_cast<int>(patch.getCell(i + 1, 0)) +
    	  	static_cast<int>(patch.getCell(i - 1, 1)) +
    	  	static_cast<int>(patch.getCell(i    , 1)) +
    	  	static_cast<int>(patch.getCell(i + 1, 1));
    	updateGridPatchCell(result, patch, numNeighbors, i, 0);
    }

	// The right edge
    for (uint8_t i = 1; i < grid_patch_size - 1; ++i) {
		const int numNeighbors = 
    	  	static_cast<int>(patch.getCell(grid_patch_size - 2, i - 1)) +
    	  	static_cast<int>(patch.getCell(grid_patch_size - 1, i - 1)) +
    	  	static_cast<int>(edges.right[i - 1]) +
    	  	static_cast<int>(patch.getCell(grid_patch_size - 2, i)) +
    	  	static_cast<int>(edges.right[i]) +
    	  	static_cast<int>(patch.getCell(grid_patch_size - 2, i + 1)) +
    	  	static_cast<int>(patch.getCell(grid_patch_size - 1, i + 1)) +
    	  	static_cast<int>(edges.right[i + 1]);
    	updateGridPatchCell(result, patch, numNeighbors, grid_patch_size - 1, i);
    }

	// The bottom edge
    for (uint8_t i = 1; i < grid_patch_size - 1; ++i) {
		const int numNeighbors = 
    	  	static_cast<int>(patch.getCell(i - 1 , grid_patch_size - 2)) +
    	  	static_cast<int>(patch.getCell(i     , grid_patch_size - 2)) +
    	  	static_cast<int>(patch.getCell(i + 1 , grid_patch_size - 2)) +
    	  	static_cast<int>(patch.getCell(i - 1 , grid_patch_size - 1)) +
    	  	static_cast<int>(patch.getCell(i + 1 , grid_patch_size - 1)) +
    	  	static_cast<int>(edges.bottom[i - 1]) +
    	  	static_cast<int>(edges.bottom[i]) +
    	  	static_cast<int>(edges.bottom[i + 1]);
    	updateGridPatchCell(result, patch, numNeighbors, i, grid_patch_size - 1);
    }

	// The left edge
    for (uint8_t i = 1; i < grid_patch_size - 1; ++i) {
		const int numNeighbors = 
    	  	static_cast<int>(edges.left[i - 1]) +
    	  	static_cast<int>(patch.getCell(0, i - 1)) +
    	  	static_cast<int>(patch.getCell(1, i - 1)) +
    	  	static_cast<int>(edges.left[i]) +
    	  	static_cast<int>(patch.getCell(1, i)) +
    	  	static_cast<int>(edges.left[i + 1]) +
    	  	static_cast<int>(patch.getCell(0, i + 1)) +
    	  	static_cast<int>(patch.getCell(1, i + 1));
    	updateGridPatchCell(result, patch, numNeighbors, i, grid_patch_size - 1);
    }

	// The top left corner
	{
		const int numNeighbors = 
    	  	static_cast<int>(edges.topLeftCorner) +
    	  	static_cast<int>(edges.top[0]) +
    	  	static_cast<int>(edges.top[1]) +
    	  	static_cast<int>(edges.left[0]) +
    	  	static_cast<int>(patch.getCell(1, 0)) +
    	  	static_cast<int>(edges.left[1]) +
    	  	static_cast<int>(patch.getCell(0, 1)) +
    	  	static_cast<int>(patch.getCell(1, 1));
    	updateGridPatchCell(result, patch, numNeighbors, 0, 0);
    }

    // The top right corner
	{
		const int numNeighbors = 
    	  	static_cast<int>(edges.top[grid_patch_size - 2]) +
    	  	static_cast<int>(edges.top[grid_patch_size - 1]) +
    	  	static_cast<int>(edges.topRightCorner) +
    	  	static_cast<int>(patch.getCell(grid_patch_size - 2, 0)) +
    	  	static_cast<int>(edges.right[0]) +
    	  	static_cast<int>(patch.getCell(grid_patch_size - 2, 1)) +
    	  	static_cast<int>(patch.getCell(grid_patch_size - 1, 1)) +
    	  	static_cast<int>(edges.right[1]);
    	updateGridPatchCell(result, patch, numNeighbors, grid_patch_size - 1, 0);
    }

    // The bottom right corner
	{
		const int numNeighbors = 
    	  	static_cast<int>(patch.getCell(grid_patch_size - 2, grid_patch_size - 2)) +
    	  	static_cast<int>(patch.getCell(grid_patch_size - 1, grid_patch_size - 2)) +
    	  	static_cast<int>(edges.right[grid_patch_size - 2]) +
    	  	static_cast<int>(patch.getCell(grid_patch_size - 2, grid_patch_size - 1)) +
    	  	static_cast<int>(edges.right[grid_patch_size - 1]) +
    	  	static_cast<int>(edges.bottom[grid_patch_size - 2]) +
    	  	static_cast<int>(edges.bottom[grid_patch_size - 1]) +
    	  	static_cast<int>(edges.bottomRightCorner);
    	updateGridPatchCell(result, patch, numNeighbors, grid_patch_size - 1, grid_patch_size - 1);
    }

	// The bottom left corner
	{
		const int numNeighbors = 
    	  	static_cast<int>(edges.left[grid_patch_size - 2]) +
    	  	static_cast<int>(patch.getCell(0, grid_patch_size - 2)) +
    	  	static_cast<int>(patch.getCell(1, grid_patch_size - 2)) +
    	  	static_cast<int>(edges.left[grid_patch_size - 1]) +
    	  	static_cast<int>(patch.getCell(1, grid_patch_size - 1)) +
    	  	static_cast<int>(edges.bottomLeftCorner) +
    	  	static_cast<int>(edges.bottom[0]) +
    	  	static_cast<int>(edges.bottom[1]);
    	updateGridPatchCell(result, patch, numNeighbors, 0, grid_patch_size - 1);
    }

    return result;
}

SDL_Surface* GridPatchToSurface(const GridPatch& patch, int format, uint8_t square_length, uint8_t border_size) {
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

