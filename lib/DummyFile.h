//
// Created by fedemgp on 7/4/22.
//

#ifndef CLASE_EXCEPCIONES_DUMMYFILE_H
#define CLASE_EXCEPCIONES_DUMMYFILE_H

#include <iostream>

/**
 * Clase dummy que se puede copiar y mover (servirá para explicar el concepto de exception safetiness). Cuenta con
 * atributo entero. Solo se puede construir con un valor positivo, y si lo muevo, invalido al anterior seteando un -1.
 */
class DummyFile {
private:
    int fd;

public:
    DummyFile();
    explicit DummyFile(int i);
    /*
     * La copia de un archivo en la realidad involucra abrir un nuevo file descriptor, y la asignación de recursos
     * que eso conlleva, por lo que puede fallar
     */
    DummyFile(DummyFile &copy);
    /*
     * Por otro lado, el constructor por movimiento no puede fallar, ya que no alloca nada nuevo! solo toma prestado
     * lo que ya se creó en otro lado
     */
    DummyFile(DummyFile &&other) noexcept;
    ~DummyFile();

    DummyFile& operator=(const DummyFile &copy);
    DummyFile& operator=(DummyFile &&other)  noexcept;
    /*
     * Simulo un cierre del file descriptor invalidandoló
     */
    void close();
    /*
     * Operador == para poder comparar distintos objetos DummyFile
     */
    bool operator==(const DummyFile& file) const;
};


#endif //CLASE_EXCEPCIONES_DUMMYFILE_H
