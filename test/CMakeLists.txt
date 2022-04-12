# Habilito la suite de tests
enable_testing()
# Busco el paquete de Gtest, ver como instalarlo en la documentacion de la herramienta
find_package(GTest REQUIRED)

# defino un ejecutable de nombre main_test compuesto de los archivos cpp que se encuentr en
# la carpeta test
add_executable(
        main_test
        main_test.cpp
)
# Linkeo el ejecutable de tests con las bibliotecas de gtest y la biblioteca a testear
target_link_libraries(main_test GTest::GTest GTest::Main mylib)

include(GoogleTest)
gtest_discover_tests(main_test)