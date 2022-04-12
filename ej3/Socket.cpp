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
#include "OSError.h"

Socket::Socket(const char *hostname, const char *service): fd(-1) {
    Resolver resolver(hostname, service, false);
    if (!resolver.resolve_any(Socket::_try_connect, this)) {
        /*
         * Notar toda la información que brindamos y que nos ayuda a nosotros los programadores
         * a comprender el error y poder debuggearlo!
         */
        throw OSError("Error al instanciar el socket al host '%s' y al servicio '%s'", hostname, service);
    }
}

Socket::~Socket() {
    try {
        /*
         * Nuestro diseño es RAII, por lo que el destructor se va a llamar SOLO si pudo conectarse correctamente.
         * SIN EMBARGO, es posible que close falle (ver documentación de ::close) por lo que debemos asegurarnos de
         * catchear toda excepción que pueda lanzar este método.
         */
        this->close();
    } catch (const std::exception &e) {
        fprintf(stderr, "Error en Socket::~Socket(): %s\n", e.what());
    } catch (...) {
        fprintf(stderr, "Error desconocido en Socket::~Socket\n");
    }
}

bool Socket::_try_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen, void *ctx) {
    if (::connect(sockfd, addr, addrlen) == 0) {
        ((Socket *) ctx)->fd = sockfd;
        return true;
    } else {
        return false;
    }
}

void Socket::close() {
    if (fd != -1) {
        if (::shutdown(fd, SHUT_RDWR) == -1)
            throw OSError("Error al silenciar el socket de fd = %i", fd);
        if (::close(fd) == -1)
            throw OSError("Error al cerrar el socket de fd = %i", fd);;
        fd = -1;
    }
}

/**
 * Envía un stream de bytes delemitado por el tamaño del buffer <buff> al socket de file descriptor <skt>
 *
 * @param skt un file descriptor de un socket con la conexión realizada
 * @param buff el mensaje a enviar
 */
void Socket::send(const char *buff, size_t length){
    int sent = 0;
    int status = 0;

    while (sent < length){
        status = ::send(this->fd, &buff[sent], length - sent,MSG_NOSIGNAL);
        /*
         * status < 0:	hubo un error, salgo del ciclo con un error.
         * status > 0:	se enviaron status bytes, continuo en el ciclo.
         */
        if (status < 0) {
            /*
             * Notar que no printeo el buffer. Un socket envía un stream de datos, estos datos
             * pueden no estar formateados de forma tal que terminen con un \0 final. Si quisiera
             * printearlo puedo causar un buffer overfloe (invalid read).
             *
             * Tampoco se deberia printear el buffer por cuestiones de seguridad, ya que podriamos filtrar información
             * sensible (imaginensé que se envia por el socket una clave para hacer un handshake y durante ese envio
             * el send/recv falla, el cliente de nuestra aplicación va a poder ver parte o toda la clave del handshake
             * en un log!!)
             */
            throw OSError("Error al intentar enviar %lu bytes. Se lograron enviar %i bytes", length, sent);
        } else{
            sent += status;
        }
    }
}
/**
 * Recibe un stream de bytes delimitado por el tamaño del buffer <buff> del socket de file descriptor <skt>
 *
 * @param skt un file descriptor de un socket con la conexión realizada
 * @param buff el mensaje a recibir
 * @return la cantidad de bytes recibidps, 0 si el socket del cliente fue cerrado y no se le pudo enviar nada, -1 si falla
 */
void Socket::receive(char *buff, size_t length){
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
            /*
             * Notar que no printeo el buffer. Un socket envía un stream de datos, estos datos
             * pueden no estar formateados de forma tal que terminen con un \0 final. Si quisiera
             * printearlo puedo causar un buffer overfloe (invalid read).
             */
            throw OSError("Error al intentar recibir %lu bytes. Se lograron recibir %i bytes", length, received);
        } else{
            received += status;
        }
    }
}
