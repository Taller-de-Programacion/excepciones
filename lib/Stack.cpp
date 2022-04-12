//
// Created by fedemgp on 7/4/22.
//

#include "Stack.h"

Stack::Stack(size_t size): buffer(nullptr), size(0), count(0) {
    buffer = (DummyFile *) malloc(sizeof(DummyFile) * size);
    if (!buffer) throw std::runtime_error("No se pudo crear el buffer");

    this->size = size;
}

Stack::~Stack() {
    if (this->size > 0) free(this->buffer);
    this->size = 0;
    this->count = 0;
}
/*
 * Ejercicio: Esta función es exception safe strong o weak?
 */
void Stack::push(DummyFile file) {
    if (this->count + 1 == this->size) throw std::logic_error("Stack lleno");

    this->buffer[count++] = file;
}
/*
 * Analizar exception safetiness de esta función
 */
DummyFile Stack::pop() {
    if (count == 0) {
        throw std::overflow_error("Stack vacio");
    }

    DummyFile temp;
    temp = this->buffer[--count];
    return temp;
}

DummyFile &Stack::peek() {
    if (count == 0) {
        throw std::overflow_error("Stack vacio");
    }

    return this->buffer[count - 1];
}
