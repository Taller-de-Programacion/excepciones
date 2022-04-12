cmake_minimum_required(VERSION 3.13)
project(clase-excepciones)

# Defino la version del estandar de C++ a la que se compila
set(CMAKE_CXX_STANDARD 17)

# Defino la carpeta lib como directorio de includes, de esta forma podre hacer llamados
# del estilo '#include "Vector.h"' sin la necesidad de pasar un path relativo al .h
include_directories(lib)

# import del cmake de la carpeta lib
add_subdirectory(lib)
add_subdirectory(ej0)
add_subdirectory(ej1)
add_subdirectory(ej2)
add_subdirectory(ej3)

add_subdirectory(test)

