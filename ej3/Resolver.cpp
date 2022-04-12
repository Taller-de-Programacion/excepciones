//
// Created by fedemgp on 3/4/22.
//


#include <unistd.h>
#include <netdb.h>

#include <cstring>
#include <iostream>

#include "Resolver.h"
#include "OSError.h"

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
        // Notar cuanta información brindamos para ayudarnos a debuggear ante fallas!!
        throw OSError("Error al intentar obtener la información para el host '%s' y servicio '%s'. "
                      "gai_strerror = '%s', passive = %i",
                      hostname, servicename, gai_strerror(status), passive);
    }
}

Resolver::~Resolver() {
    /*
     * Notar que ahora no hago un chequeo de si el puntero es nulo. ¡No es necesario! Como el objeto resolver
     * es no copiable y no movible, y como respetamos el concepto de RAII, siempre será distinto de nullptr el puntero
     * result (ya que si getaddrinfo falla, el destructor no será llamado porque nunca se construyó correctamente).
     */
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
