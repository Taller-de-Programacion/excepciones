//
// Created by fedemgp on 3/4/22.
//
#include <vector>
#include <iostream>
#include <cstring>

#include "Socket.h"

int main(int argc, const char *argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <hostname> <service_name>" << std::endl;
        return -1;
    }
    /*
     * Ahora si, volvemos al camino feliz definitivamente
     */
    try {
        Socket skt(argv[1], argv[2]);

        std::vector<char> buff = {'h', 'o', 'l', 'a'};
        skt.send(buff.data(), 4);

        std::vector<char> received(5, 0);
        skt.receive(received.data(), 5);

        std::string msg(received.begin(), received.end());
        std::cout << "Recibido: " << msg << std::endl;
    } catch(const std::exception &e) {
        /*
         * Atrapamos una referncia constante para no estar haciendo copias indeseadas del objeto y para asegurarnos
         * de que nadie lo modifique. ¿Por qué no capturamos nuestra excepción OSError?
         */
        std::cerr << e.what() << std::endl;
        return -1;
    } catch(...) {
        /*
         * Todo lo que no sea un objeto que herede de std::exception (la clase  base de todas las excepciones en C++)
         * lo atrapamos en estas elipsis. Realizamos un best effort y imprimimos toda la información que podemos acá.
         */
        fprintf(stderr, "Error desconocido en la función main\n");
        return -1;
    }

    return 0;
}
