//
// Created by fedemgp on 7/4/22.
//

#ifndef CLASE_EXCEPCIONES_STACK_H
#define CLASE_EXCEPCIONES_STACK_H


#include "DummyFile.h"

class Stack {
private:
    DummyFile *buffer;
    size_t size;
    size_t count;

public:
    explicit Stack(size_t size);
    ~Stack();

    void push(DummyFile file);
    DummyFile pop();
    DummyFile& peek();
};


#endif //CLASE_EXCEPCIONES_STACK_H
