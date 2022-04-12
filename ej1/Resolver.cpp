//
// Created by fedemgp on 3/4/22.
//


#include <unistd.h>
#include <netdb.h>

#include <cstring>
#include <iostream>

#include "Resolver.h"

Resolver::Resolver(const char *hostname, const char *servicename, bool passive): result(nullptr) {
    struct addrinfo hints;
    /*
     * El servidor al cual me quiero conectar puede tener varios nodos
     * en la lista para probar, esto se puede deber (según la documentación del manual de getaddrinfo):
     *      * el servidor es multihomed (un host se puede traducir en N direcciones IP)
     *      * el servidor puede ser accedido por distintos protocolos (IPv4, IPv6)
     *      * el servidor puede estar disponible en multiploes tipos de sockets (SOCK_STREAM, SOCK_DGRAM)
     *
     * Con la estructura hint podemos reducir algunas de estas posibilidades, pero siempre nos devolverá una lista
     * que debemos iterar
     */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = passive ? AI_PASSIVE : 0;  /* AI_PASSIVE for server; 0 for client */
    int status = getaddrinfo(hostname, servicename, &hints, &result);
    if (status != 0){
        printf("Error al llamar a getaddrinfo: %s\n", gai_strerror(status));
        return ; // TODO lanzar excepción
    }
}

Resolver::~Resolver() {
    /*
     * La lista fue reservada dinamicamente, por lo que requiere ser liberada por nosotros.
     * Puede pasar de que la llamada a getaddrinfo haya fallado, y el puntero <result> no haya allocado nada,
     * en dicho caso debemos protegernos de no hacer un free(NULL).
     */
    if (this->result)
        freeaddrinfo(this->result);
}

bool Resolver::resolve_any(resolver_iterator_cb callback, void *ctx) {
    struct addrinfo *current;
    for (current = this->result; current != nullptr; current = current->ai_next) {
        int fd = socket(current->ai_family, current->ai_socktype, current->ai_protocol);
        /*
        *	si la creación del socket falla, no debo hacer nada mas
        *	en el ciclo (ya que no se abrio ningun fd)
        */
        if (fd == -1)
            continue;
        if (callback(fd, current->ai_addr, current->ai_addrlen, ctx))
            return true;
        /*
         * Si la conexión/bind falla, el file descriptor queda en un estado inválido y debe ser destruido.
         * TODO: esto puede fallar!
         */
        close(fd);
    }
    return false;
}
