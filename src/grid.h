#ifndef GOL_GRID_H
#define GOL_GRID_H

#include <cstdint>
#include <array>
#include <unordered_map>

constexpr int grid_patch_size = 64;

struct GridId {
    int8_t x;
    int8_t y;

    bool operator==(GridId other) const {
        return (this->x == other.x && this->y == other.y);
    }
};

namespace std {
    template <>
    struct hash<GridId> {
        size_t operator()(GridId id) const {
            return static_cast<size_t>(id.x) << 8 + static_cast<size_t>(id.y);
        }
    };
}

struct GridPatch {
    std::array<bool, grid_patch_size * grid_patch_size> cells;

    void setCell(uint8_t x, uint8_t y, bool value) noexcept;
    bool getCell(uint8_t x, uint8_t y) const noexcept;
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

GridPatch updateGridPatch(const GridPatch& patch, const GridPatchEdges& edges) noexcept;
bool areEdgesPopulated(const GridPatchEdges& edges) noexcept;
void updateGrid(Grid& grid) noexcept;

#endif
