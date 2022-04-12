//
// Created by fedemgp on 7/4/22.
//

#include "DoubleBuffer.h"

DoubleBuffer::DoubleBuffer(unsigned long count): bufA(nullptr), bufB(nullptr) {
    bufA = (char *) malloc(sizeof(char) * count / 100);
    bufB = (char *) malloc(sizeof(char) * count);

    if (!bufA || !bufB) throw std::runtime_error("Error al reservar memoria para los buffers");
}

DoubleBuffer::~DoubleBuffer() {
    /*
     * Si alguno de los mallocs falla, chequeo en el destructor para liberar el recurso (y evitar un double free)
     * nada puede malir sal, ¿no?
     */
    if (bufA)
        free(bufA);

    if (bufB)
        free(bufB);
}
