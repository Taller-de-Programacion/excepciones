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
     * Listo! problema solucionado, volvemos a la senda del camino feliz, y conservamos la robustez del chequeo de
     * errores del ejemplo pasado. No queda nada mas por hacer, ¿o si?
     */
    try {
        Socket skt;
        /*
         * Que pasa si Socket::connect lanza una excepción y Socket::close también? Por estas razones un destructor
         * NUNCA debe lanzar excepciones
         *
         * Este error igual marca otra falencia del diseño de socket, por qué llamamos la destructor de socket, si el
         * objeto no está completamente inicializado? No estamos respetando RAII
         */
        skt.connect(argv[1], argv[2]);

        std::vector<char> buff = {'h', 'o', 'l', 'a'};
        skt.send(buff.data(), 4);

        std::vector<char> received(5, 0);
        skt.receive(received.data(), 5);

        std::string msg(received.begin(), received.end());
        std::cout << "Recibido: " << msg << std::endl;
    /*
     * Está bien lanzar codigos de retorno? es claro esto? desgraciadamente, C++ nos deja lanzar cualquier tipo como
     * excepción, pero nosotros deberiamos siempre lanzar una excepción que herede de std::exception. Puede ser una clase
     * nuestra, o alguna de la extensa variedad que nos provee la STL.
     */
    } catch(int i) {
        fprintf(stderr, "Se lanzó un error de código %i\n", i);
        return -1;
    }

    return 0;
}
