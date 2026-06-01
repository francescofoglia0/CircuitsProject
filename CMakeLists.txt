cmake_minimum_required(VERSION 3.20)
project(ProgettoCircuiti)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla")

# Trova la libreria Eigen3 nel sistema
find_package(Eigen3 REQUIRED)

# Creo il target per l'eseguibile principale del mio progetto software
add_executable(circuiti main.cpp)

add_executable(test_circ test_circuiti.cpp)
# Collega Eigen e imposta le ottimizzazioni su "circuiti"
target_link_libraries(circuiti PRIVATE Eigen3::Eigen)
target_link_libraries(test_circ Eigen3::Eigen)