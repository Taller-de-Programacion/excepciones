//
// Created by fedemgp on 3/4/22.
//

#ifndef CLASE_EXCEPCIONES_RESOLVER_H
#define CLASE_EXCEPCIONES_RESOLVER_H

#include <unistd.h>

/*
 * Este puntero a funcion sirve para abstraer la lógica de como debe ser iterado
 * la lista que devuelve getaddrinfo de para qué quiero iterarla (conectarme o bindear el socket a puerto).
 * Esto es un patrón adapter.
 *
 * Notar que de esta forma se reduce codigo duplicado, ya que no hay que implementar dos veces el mismo loop
 * para conectarse y para hacer un bind, y no es necesario realizar la iteración fuera de la clase (y exponiendo así
 * parte de los atributos pribados de la clase, como el puntero al siguiente elemento `struct addrinfo *`).
 *
 * Notar que la forma del puntero a función recibe los tres parámetros obligatorios de connect y bind, mas un puntero
 * genérico que será el socket por el cual se está intentando conectar/bindear
 *
 * Este método de resolver el problema sigue siendo muy "a la C". C++ provee un feature para resolver este problema
 * de forma mas legible que es el uso de templates, tema que se verá mas adelante.
 *
 */
typedef bool (*resolver_iterator_cb) (int socket_fd,
                                     const struct sockaddr *add,
                                     socklen_t addrlen,
                                     void *ctx);

class Resolver {
    struct addrinfo *result;

public:
    Resolver(const char *hostname, const char *servicename, bool passive);
    ~Resolver();
    /**
     * Para cada elemento de la lista <result>, la itero ejecutando el callback.
     *
     * @param callback
     * @param ctx
     * @return Si la ejecución fue exitosa, detengo la iteracion y retorno true. Sino sigo iterando hasta llegar al
     * final de la lista <result>, y retorno false si la ejecución del callback nunca fue exitosa.
     */
    bool resolve_any(resolver_iterator_cb callback, void *ctx);
    /*
     * En este diseño no voy a dejar que el objeto Resolver sea copiable ni movible. Como se va a usar como objeto
     * de paso, temporal dentro de Socket::connect o Socket::bind no hay la necesidad de cambiar el ownership de esta
     * clase
     */
    Resolver(Resolver& copy) = delete;
    Resolver& operator=(Resolver& copy) = delete;
    Resolver(Resolver&& other) = delete;
    Resolver& operator=(Resolver&& other) = delete;

};


#endif //CLASE_EXCEPCIONES_RESOLVER_H
