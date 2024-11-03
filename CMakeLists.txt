cmake_minimum_required(VERSION 3.26)

project(oop)

#set(CMAKE_CXX_STANDARD 11)

# Setează calea către SFML
#set(SFML_DIR "C:/Users/Andreea/Desktop/Proiect POO/SFML-2.6.1/lib/cmake/SFML")

# Caută pachetele SFML necesare
#find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# Crează executabilul
add_executable(${PROJECT_NAME} Flappy1.cpp)

# Leagă bibliotecile SFML
#target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

# Include directoarele necesare
#target_include_directories(${PROJECT_NAME} PRIVATE ${SFML_DIR}/include)
