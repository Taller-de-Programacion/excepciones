//
// Created by fedemgp on 7/4/22.
//

#ifndef CLASE_EXCEPCIONES_DOUBLEBUFFER_H
#define CLASE_EXCEPCIONES_DOUBLEBUFFER_H


#include <cstddef>
#include <cstdlib>
#include <stdexcept>

class DoubleBuffer {
private:
    char *bufA;
    char *bufB;

public:
    DoubleBuffer(unsigned long count);
    ~DoubleBuffer();
};


#endif //CLASE_EXCEPCIONES_DOUBLEBUFFER_H
