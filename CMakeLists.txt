cmake_minimum_required(VERSION 2.8)
project(game-of-life)
add_executable(game-of-life)
target_sources(game-of-life
	PUBLIC "${CMAKE_SOURCE_DIR}/src/main.cpp"
	PUBLIC "${CMAKE_SOURCE_DIR}/src/grid.cpp"
)
add_compile_options(-Wall -Wextra -Wpedantic)

find_package(SDL2 REQUIRED)
include_directories(game-of-life ${SDL2_INCLUDE_DIRS})
target_link_libraries(game-of-life SDL2) 

