# Busco todos los archivos de extension cpp y h que se encuentren en el
# directorio "lib" y los asigno a la variable LIB_SOURCES
FILE(GLOB LIB_SOURCES
        Date.cpp
        Date.h
        DoubleBuffer.cpp
        DoubleBuffer.h
        DummyFile.cpp
        DummyFile.h
        Stack.cpp
        Stack.h
        )

message("Archivos pertenecientes a mi biblioteca. ${LIB_SOURCES}")
# Defino una biblioteca de nombre mylib, estatica, compuesta de los fuentes encontrados en LIB_SOURCES
add_library(mylib STATIC ${LIB_SOURCES})