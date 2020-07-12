#include "grid.h"

#include <iostream>
#include <cassert>

int main() {
	std::cout << "Hello Game of Life!\n";

	GridPatch gp1{};
	for (bool c : gp1.cells) {
		assert(c == false);
    }
	
	return 0;
}
