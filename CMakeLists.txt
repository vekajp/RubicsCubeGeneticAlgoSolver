cmake_minimum_required(VERSION 3.17)
project(RubicsCubeGeneticAlgoSolver)

set(CMAKE_CXX_STANDARD 14)
add_executable(RubicsCubeGeneticAlgoSolver main.cpp cubic.cpp genetic.cpp cubic.hpp genetic.hpp)
include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(RubicsCubeGeneticAlgoSolver sfml-system sfml-window sfml-graphics sfml-audio sfml-network)