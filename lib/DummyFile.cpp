//
// Created by fedemgp on 9/4/22.
//

#include "DummyFile.h"

DummyFile::DummyFile(): fd(-1) {
    std::cout << "Constructor default" << std::endl;
}

DummyFile::DummyFile(int i): fd(i) {
    if (i < 0) throw std::runtime_error("File descriptor inválido");
    std::cout << "Constructor con parámetro,  fd = " << i << std::endl;
}

DummyFile::DummyFile(DummyFile &copy): fd(copy.fd) {
    if (this->fd < 0) throw std::runtime_error("File descriptor inválido");

    std::cout << "Constructor copia fd = " << fd << std::endl;
}

DummyFile::DummyFile(DummyFile &&other) noexcept : fd(other.fd) {
    other.fd = -1;
    std::cout << "Constructor por movimiento fd = " << fd << std::endl;
}

DummyFile::~DummyFile() {
    if (fd >= 0) {
        close();
    }
};

DummyFile &DummyFile::operator=(const DummyFile &copy) {
    std::cout << "Asignación copia fd = " << copy.fd << std::endl;
    // Siempre protegerte de copiarte a vos mismo
    if (this == &copy) return *this;
    if (copy.fd < 0) throw std::runtime_error("File descriptor inválido");

    this->fd = copy.fd;
    return *this;
}

DummyFile &DummyFile::operator=(DummyFile &&other) noexcept {
    std::cout << "Asignación movimiento fd = " << other.fd << std::endl;
    // Siempre protegerte de copiarte a vos mismo
    if (this == &other) return *this;

    this->fd = other.fd;
    other.fd = -1;
    return *this;
}

void DummyFile::close() {
    fd = -1;
}

bool DummyFile::operator==(const DummyFile &file) const {
    return this->fd == file.fd;
}
