cmake_minimum_required(VERSION 3.20)
project(ProgettoCircuiti)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla")
# Creo il target per l'eseguibile principale del mio progetto software
add_executable(circuiti main.cpp)