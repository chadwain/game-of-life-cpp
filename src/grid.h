#ifndef GOL_GRID_H
#define GOL_GRID_H

#include <SDL2/SDL.h>

#include <cstdint>
#include <array>
#include <unordered_map>

constexpr int grid_patch_size = 64;

struct GridId {
	int8_t x;
	int8_t y;
};

struct GridPatch {
	std::array<bool, grid_patch_size * grid_patch_size> cells;

	void setCell(uint8_t x, uint8_t y, bool value);
	bool getCell(uint8_t x, uint8_t y) const;
};

struct GridPatchEdges {
	std::array<bool, grid_patch_size> top;
	std::array<bool, grid_patch_size> right;
	std::array<bool, grid_patch_size> bottom;
	std::array<bool, grid_patch_size> left;
	bool topLeftCorner;
	bool topRightCorner;
	bool bottomRightCorner;
	bool bottomLeftCorner;
};

struct Grid {
    std::unordered_map<GridId, GridPatch> patches;
    std::unordered_map<GridId, GridPatchEdges> patchEdges;
};

GridPatch updateGridPatch(const GridPatch& patch, const GridPatchEdges& edges);
SDL_Surface* GridPatchToSurface(const GridPatch& patch, int format, uint8_t square_length, uint8_t border_size);

#endif
