cmake_minimum_required(VERSION 3.20)
project(ProgettoCircuiti)

enable_testing()
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla")

# Trova la libreria Eigen3 nel sistema
find_package(Eigen3 REQUIRED)

# Creo il target per l'eseguibile principale del mio progetto software
set(SRC_progetto timecounter.cpp main.cpp)
add_executable(progetto ${SRC_progetto})

# Collega Eigen
target_link_libraries(progetto PRIVATE Eigen3::Eigen)

#aggiungo i test
add_executable(test_findpath test_findpath.cpp)
target_link_libraries(test_findpath PRIVATE Eigen3::Eigen)
add_test(NAME TestFindPath COMMAND test_findpath)

add_executable(test_dfs_cicli test_dfs_cicli.cpp)
target_link_libraries(test_dfs_cicli PRIVATE Eigen3::Eigen)
add_test(NAME TestDfsCicli COMMAND test_dfs_cicli)

add_executable(test_de_pina test_de_pina.cpp)
target_link_libraries(test_de_pina PRIVATE Eigen3::Eigen)
add_test(NAME TestDePina COMMAND test_de_pina)


add_executable(test_creazione_B_R test_creazione_B_R.cpp)
target_link_libraries(test_creazione_B_R PRIVATE Eigen3::Eigen)
add_test(NAME TestCreazioneBR COMMAND test_creazione_B_R)

add_executable(test_calcola_output test_calcola_output.cpp)
target_link_libraries(test_calcola_output PRIVATE Eigen3::Eigen)
add_test(NAME TestCalcolaOutput COMMAND test_calcola_output)

add_executable(test_gc test_gc.cpp)
target_link_libraries(test_gc PRIVATE Eigen3::Eigen)
add_test(NAME TestGC COMMAND test_gc)

add_executable(test_lifofifo test_lifofifo.cpp)
add_test(NAME TestLifoFifo COMMAND test_lifofifo)

add_executable(test_visite test_visite.cpp)
add_test(NAME TestVisite COMMAND test_visite)

