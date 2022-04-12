# excepciones

Para compilar el proyecto, primero necesitan tener instalado `cmake`

```shell
sudo apt-get install cmake
```

Luego, deben crear una carpeta build (porque cmake generará muchos
archivos temporales de compilación) y ejecutar lo siguiente.

```shell
mkdir build
cd build
cmake ..
make -j3
```

Mas adelante en la materia se explicará un poco mas en detalle cmake, pero 
se deja un archivo `CMakeLists.txt` comentado para que puedan entender
cual es la magia que se hace detrás de escena.

## Compilar los tests

Para compilar los tests primero se necesita instalar algunas dependencias.
En este ejemplo se usó el framework de testing que ofrece Google
(google test).

```shell
sudo apt-get install googletest googletest-tools
```

Se debe agregar un flag a cmake para indicarle que
debe compilarlos. Para eso, se debe ejecutar lo siguiente (asumiendo que
ya crearon la carpeta build y que se encuentran parados en dicha carpeta)

```shell
cmake -DENABLE_TESTS ..
make -j3
```

Finalmente, para correr los tests unitarios, se debe correr lo siguiente

```shell
cd test # asumiendo que estabamos parado en la carpeta build donde compilamos
ctest
```

Podemos usar una versión mas _verbose_ usando el comando `-V`
