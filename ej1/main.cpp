//
// Created by fedemgp on 3/4/22.
//
#include <vector>
#include <iostream>
#include <cstring>

#include "Socket.h"

/*
 * Ahora el código es mucho mas robusto, pero, ¿a qué costo? imaginensé si tienen otros
 * objetos dando vueltas, como un FILE *, mas memoria dinámica, dos sockets (uno aceptador otro el aceptado), CAOS.
 */
int main(int argc, const char *argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <hostname> <service_name>" << std::endl;
        return -1;
    }

    Socket skt;
    if (skt.connect(argv[1], argv[2]) == -1) {
        return -1;
    }

    char *buff = (char *) calloc(5, sizeof(char));
    if (buff == nullptr) {
        perror("Error al llamar a calloc");
        return -1;
    }
    strncpy(buff, "hola", 4);

    if (skt.send(buff, 4) == -1) {
        // Debo acordarme de liberar recursos que no haya encapsulado en un objeto RAII
        free(buff);
        return -1;
    }

    char *received = (char *) calloc(5, sizeof(char));
    if (received == nullptr) {
        perror("Error al llamar a calloc");
        free(buff);
        return -1;
    }

    if (skt.receive(received, 5) == -1) {
        free(buff);
        free(received);
        return -1;
    }

    std::string msg(received, 5);
    std::cout << "Recibido: " << msg << std::endl;

    free(received);
    free(buff);
    return 0;
}
