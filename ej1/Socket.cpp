//
// Created by fedemgp on 3/4/22.
//

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstdio>

#include "Socket.h"
#include "Resolver.h"

Socket::Socket(): fd(-1) {}

Socket::~Socket() {
    /*
     * Notar que ignoramos todos los errores que pueda haber al llamar a este método. Esto se debe
     * a que un destructor no debe fallar.
     */
    this->close();
}

int Socket::connect(const char *hostname, const char *service) {
    Resolver resolver(hostname, service, false);
    if (!resolver.resolve_any(Socket::_try_connect, this)) {
        perror("Socket falló al querer conectarse");
        return -1;
    }
    return 0;
}

bool Socket::_try_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen, void *ctx) {
    if (::connect(sockfd, addr, addrlen) == 0) {
        ((Socket *) ctx)->fd = sockfd;
        return true;
    } else {
        return false;
    }
}

int Socket::close() {
    if (fd != -1) {
        if (::shutdown(fd, SHUT_RDWR) == -1) {
            perror("Socket::close falló al hacer un shutdown");
            fd = -1;
            return -1;
        }
        if (::close(fd) == -1) {
            perror("Socket::close falló al hacer un close");
            fd = -1;
            return -1;
        }
        fd = -1;
    }
    return 0;
}
/**
 * Envía un stream de bytes delemitado por el tamaño del buffer <buff> al socket de file descriptor <skt>
 *
 * @param skt un file descriptor de un socket con la conexión realizada
 * @param buff el mensaje a enviar
 * @return la cantidad de bytes enviados, o -1 si falla
 */
int Socket::send(const char *buff, size_t length){
    int sent = 0;
    int status = 0;

    while (sent < length){
        status = ::send(this->fd, &buff[sent], length - sent,MSG_NOSIGNAL);
        /*
         * status < 0:	hubo un error, salgo del ciclo con un error.
         * status > 0:	se enviaron status bytes, continuo en el ciclo.
         */
        if (status < 0) {
            perror("Socket:send falló");
            return -1;
        } else{
            sent += status;
        }
    }
    return sent;
}
/**
 * Recibe un stream de bytes delimitado por el tamaño del buffer <buff> del socket de file descriptor <skt>
 *
 * @param skt un file descriptor de un socket con la conexión realizada
 * @param buff el mensaje a recibir
 * @return la cantidad de bytes recibidps, 0 si el socket del cliente fue cerrado y no se le pudo enviar nada, -1 si falla
 */
int Socket::receive(char *buff, size_t length){
    int received = 0;
    int status = 0;

    while (received < length){
        status = recv(this->fd, &buff[received], length - received, 0);
        /*
         * status == 0: cerraron el socket (puede o no ser un error dependiendo del protocolo)
         * status < 0:	hubo un error
         * status > 0:	se enviaron status bytes
         */
        if (status == 0) {
            break;
        } else if (status < 0){
            perror("Socket:receive falló");
            return -1;
        } else{
            received += status;
        }
    }
    return received;
}
